/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

#include "unordered_map/unordered_map.h"
#include "vgpu_shaderconv/shaderconv.h"
#include "glsl_optimizer/src/code/c_wrapper.h"
#include <GLES3/gl3.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include "string_utils.h"
#include "egl.h"
#include "proc.h"
#include "debug.h"
#include "mempool.h"

#define SHADER_CACHE_SIZE 256
#define SHADER_CACHE_STATS 1
#define SHADER_CACHE_MAX_MEMORY (32 * 1024 * 1024) // 32MB 最大内存限制

// 获取高精度时间戳（跨平台兼容）
static inline uint64_t get_timestamp() {
#if defined(__GNUC__) || defined(__clang__)
#if defined(__x86_64__) || defined(__i386__)
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
#elif defined(__aarch64__)
    uint64_t t;
    __asm__ __volatile__ ("mrs %0, cntvct_el0" : "=r" (t));
    return t;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
#endif
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
#endif
}

typedef struct {
    size_t source_hash;      // djb2, primary hash
    size_t source_hash2;     // FNV-1a 64, secondary hash: two hashes make collisions practically impossible
    GLenum shader_type;
    GLchar* optimized_source;
    size_t source_size;      // optimized source size, for memory accounting
    uint32_t access_count;   // LRU accounting
    uint64_t last_access;
} shader_cache_entry_t;

static shader_cache_entry_t shader_cache[SHADER_CACHE_SIZE] = {0};
static int shader_cache_count = 0;
static uint64_t cache_hits = 0;
static uint64_t cache_misses = 0;
static size_t cache_total_memory = 0;

static inline size_t hash_string(const char* str) {
    size_t hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) + (unsigned char)*str++;
    }
    return hash;
}

static inline size_t hash_fnv1a(const char* str) {
    size_t hash = 1469598103934665603ULL;
    while (*str) {
        hash ^= (unsigned char)*str++;
        hash *= 1099511628211ULL;
    }
    return hash;
}

static GLchar* cache_lookup(size_t hash1, size_t hash2, GLenum shader_type) {
    for (int i = 0; i < shader_cache_count; i++) {
        shader_cache_entry_t* e = &shader_cache[i];
        if (e->source_hash == hash1 && e->source_hash2 == hash2 && e->shader_type == shader_type) {
            e->access_count++;
            e->last_access = get_timestamp();
            cache_hits++;
            return e->optimized_source;
        }
    }
    cache_misses++;
    return NULL;
}

static int cache_evict_slot(void) {
    int lru = 0;
    for (int i = 1; i < shader_cache_count; i++) {
        shader_cache_entry_t* a = &shader_cache[i];
        shader_cache_entry_t* b = &shader_cache[lru];
        if (a->access_count < b->access_count ||
            (a->access_count == b->access_count && a->last_access < b->last_access)) {
            lru = i;
        }
    }
    return lru;
}

static void cache_store(size_t hash1, size_t hash2, GLenum shader_type, const GLchar* optimized_source) {
    if (optimized_source == NULL) return;
    size_t source_size = strlen(optimized_source) + 1;

    // Skip shaders too large to be worth caching
    if (source_size > SHADER_CACHE_MAX_MEMORY / 2) {
        LTW_DEBUG_PRINTF("LTW: shader source too large for cache: %zu bytes", source_size);
        return;
    }

    int slot;
    if (shader_cache_count < SHADER_CACHE_SIZE) {
        slot = shader_cache_count++;
    } else {
        // Full: evict the least recently used entry
        slot = cache_evict_slot();
        cache_total_memory -= shader_cache[slot].source_size;
        free(shader_cache[slot].optimized_source);
    }

    shader_cache_entry_t* e = &shader_cache[slot];
    e->source_hash = hash1;
    e->source_hash2 = hash2;
    e->shader_type = shader_type;
    e->optimized_source = strdup(optimized_source);
    e->source_size = source_size;
    e->access_count = 1;
    e->last_access = get_timestamp();
    cache_total_memory += source_size;
}

#ifdef SHADER_CACHE_STATS
static void print_cache_stats(void) {
    if (cache_hits + cache_misses > 0) {
        float hit_rate = (float)cache_hits / (cache_hits + cache_misses) * 100.0f;
        LTW_DEBUG_PRINTF("Shader cache stats: hits=%" PRIu64 ", misses=%" PRIu64 ", hit_rate=%.2f%%, size=%d/%d, memory=%.2fMB/%.2fMB",
                        cache_hits, cache_misses, hit_rate, shader_cache_count, SHADER_CACHE_SIZE,
                        cache_total_memory / (1024.0f * 1024.0f), SHADER_CACHE_MAX_MEMORY / (1024.0f * 1024.0f));
    }
}
#endif

/*
 * ESSL 3.0 removed the built-in gl_FragColor / gl_FragData fragment outputs.
 * The Mesa-based optimizer downgrades desktop GLSL to ESSL but leaves those
 * identifiers untouched, which breaks fragment shaders written in the
 * GLSL 1.10/1.20 style (e.g. Minecraft <=1.16 post-processing shaders).
 * Rewrite them into explicit output variables with a single-pass scan.
 * Returns a newly allocated string, or NULL if no rewrite was needed.
 */
#define MAX_FRAGDATA_SLOTS 16
#define MAX_FRAG_REPLACES 64

typedef struct {
    size_t off;      // offset of the match in the source
    uint8_t kind;    // 0: gl_FragColor, 1: gl_FragData[N]
    uint8_t slot;    // draw buffer index for gl_FragData
    size_t old_len;
} frag_match_t;

static inline bool is_ident_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') || c == '_';
}

static int slot_in_list(const int* slots, int n, int slot) {
    for (int i = 0; i < n; i++) if (slots[i] == slot) return 1;
    return 0;
}

// Collect every gl_FragColor / gl_FragData[N] occurrence with proper
// identifier boundaries. Returns the number of matches.
static int collect_frag_matches(const GLchar* src, frag_match_t* m, int max_m,
                                int* slots, int* nslots) {
    const GLchar* p = src;
    int n = 0;
    while (n < max_m && (p = strstr(p, "gl_Frag")) != NULL) {
        bool valid = (p == src || !is_ident_char(p[-1]));
        if (p[7] == 'C' && strncmp(p, "gl_FragColor", 12) == 0 &&
            valid && !is_ident_char(p[12])) {
            m[n++] = (frag_match_t){ (size_t)(p - src), 0, 0, 12 };
            p += 12;
        } else if (p[7] == 'D' && strncmp(p, "gl_FragData", 11) == 0 &&
                   valid && p[11] == '[') {
            const GLchar* q = p + 12;
            int slot = 0;
            while (q[0] >= '0' && q[0] <= '9') { slot = slot * 10 + (q[0] - '0'); q++; }
            if (q > p + 12 && q[0] == ']' && slot <= 255) {
                if (*nslots < MAX_FRAGDATA_SLOTS && !slot_in_list(slots, *nslots, slot))
                    slots[(*nslots)++] = slot;
                m[n++] = (frag_match_t){ (size_t)(p - src), 1, (uint8_t)slot, (size_t)(q + 1 - p) };
                p = q + 1;
                continue;
            }
            p += 11;
        } else {
            p += 8;
        }
    }
    return n;
}

// Byte offset right after the #version/#extension directive lines.
static int find_insert_point(const GLchar* src) {
    const GLchar* line = src;
    while (*line == '#') {
        line = strchr(line, '\n');
        if (line == NULL) return -1;
        line++;
    }
    return (int)(line - src);
}

static GLchar* rewrite_frag_outputs(const GLchar* src) {
    frag_match_t m[MAX_FRAG_REPLACES];
    int slots[MAX_FRAGDATA_SLOTS];
    int nslots = 0;
    int n = collect_frag_matches(src, m, MAX_FRAG_REPLACES, slots, &nslots);
    if (n == 0) return NULL;

    int insert = find_insert_point(src);
    if (insert < 0) insert = (int)strlen(src);

    // Build the output variable declarations.
    char decl[1024];
    char* d = decl;
    int n_color = 0;
    for (int i = 0; i < n; i++) if (m[i].kind == 0) n_color++;
    if (n_color > 0) {
        memcpy(d, "out highp vec4 ltw_FragColor;\n", 30);
        d += 30;
    }
    for (int i = 0; i < nslots; i++) {
        int l = snprintf(d, sizeof(decl) - (size_t)(d - decl),
                         "layout(location = %d) out highp vec4 ltw_FragData%d;\n", slots[i], slots[i]);
        d += l;
    }
    size_t decl_len = (size_t)(d - decl);

    // gl_FragColor grows by 1 byte; gl_FragData[N] keeps the same length.
    size_t new_len = strlen(src) + (size_t)n_color + decl_len;
    GLchar* out = malloc(new_len + 1);
    if (out == NULL) return NULL;

    // src[0..insert) | declarations | src[insert..] with replacements.
    size_t o = 0, s = (size_t)insert;
    memcpy(out + o, src, (size_t)insert);
    o += (size_t)insert;
    memcpy(out + o, decl, decl_len);
    o += decl_len;
    for (int i = 0; i < n; i++) {
        memcpy(out + o, src + s, m[i].off - s);
        o += m[i].off - s;
        if (m[i].kind == 0) {
            memcpy(out + o, "ltw_FragColor", 13);
            o += 13;
        } else {
            int l = snprintf((char*)out + o, 32, "ltw_FragData%d", m[i].slot);
            o += (size_t)l;
        }
        s = m[i].off + m[i].old_len;
    }
    memcpy(out + o, src + s, strlen(src) - s);
    o += strlen(src) - s;
    out[o] = 0;
    return out;
}

GLuint glCreateProgram(void) {
    if(!current_context) return 0;
    GLuint phys_program = es3_functions.glCreateProgram();
    if(phys_program == 0) return phys_program;
    program_info_t *prog_info = mempool_alloc(current_context->program_info_pool);
    if(prog_info == NULL) {
        LTW_ERROR_PRINTF("LTWShdrWp: failed to allocate program_info from pool");
        abort();
    }
    memset(prog_info, 0, sizeof(program_info_t));
    unordered_map_put(current_context->program_map, (void*)phys_program, prog_info);
    return phys_program;
}

void glDeleteProgram(GLuint program) {
    if(!current_context) return;
    GLTRACE_CALL(glDeleteProgram, es3_functions.glDeleteProgram(program));
    program_info_t *old_programinfo = unordered_map_remove(current_context->program_map, (void*)program);
    if(old_programinfo == NULL) return;
    for(GLuint i = 0; i < MAX_DRAWBUFFERS; i++) {
        const GLchar* binding = old_programinfo->colorbindings[i];
        if(binding != NULL) free((void*)binding);
    }
    mempool_free(current_context->program_info_pool, old_programinfo);
}

void glAttachShader( 	GLuint program,
                        GLuint shader) {
    if(!current_context) return;
    GLTRACE_CALL(glAttachShader, es3_functions.glAttachShader(program, shader));
    program_info_t* program_info = unordered_map_get(current_context->program_map, (void*)program);
    shader_info_t* shader_info = unordered_map_get(current_context->shader_map, (void*)shader);
    if(program_info == NULL || shader_info == NULL || shader_info->shader_type != GL_FRAGMENT_SHADER) return;
    program_info->frag_shader = shader;
}

void glBindFragDataLocation( 	GLuint program,
                                GLuint colorNumber,
                                const char * name) {
    if(!current_context) return;
    program_info_t *program_info = unordered_map_get(current_context->program_map, (void*)program);
    if(program_info == NULL || colorNumber >= MAX_DRAWBUFFERS) return;
    // Insert binding name at the specific index
    GLchar** pname = &program_info->colorbindings[colorNumber];
    if(asprintf(pname, "%s", name) == -1) {
        *pname = NULL;
    }
}

void glGetShaderiv(GLuint shader, GLenum pname, GLint* params) {
    if(!current_context) return;
    shader_info_t* shader_info = unordered_map_get(current_context->shader_map, (void*)shader);
    if(shader_info != NULL && shader_info->shader_type == GL_FRAGMENT_SHADER && pname == GL_COMPILE_STATUS) {
        // HACK: ignore compile results for frag shaders, as some drivers may not compile them without explicit fragouts
        // (which we add at link-time)
        *params = GL_TRUE;
        return;
    }
    GLTRACE_CALL(glGetShaderiv, es3_functions.glGetShaderiv(shader, pname, params));
}

static void insert_fragout_pos(char* source, int* size, const char* name, GLuint pos) {
    if (!source || !size || !name) {
        LTW_ERROR_PRINTF("LTWShdrWp: Invalid parameters in insert_fragout_pos (NULL pointer)");
        return;
    }
    char src_string[256] = { 0 };
    char dst_string[256] = { 0 };
    snprintf(src_string, sizeof(src_string), "/* LTW INSERT LOCATION %s LTW */", name);
    snprintf(dst_string, sizeof(dst_string), "layout(location = %u) ", pos);
    char* result = gl4es_inplace_replace_simple(source, size, src_string, dst_string);
    if (!result) {
        LTW_ERROR_PRINTF("LTWShdrWp: gl4es_inplace_replace_simple failed in insert_fragout_pos");
    }
}

void glLinkProgram(GLuint program) {
    if(!current_context) return;
    program_info_t* program_info = unordered_map_get(current_context->program_map, (void*)program);
    if(program_info == NULL || program_info->frag_shader == 0) {
        // Don't have any fragment shader to patch the locations in, fall through.
        goto fallthrough;
    }
    shader_info_t *shader = unordered_map_get(current_context->shader_map, (void*)program_info->frag_shader);
    if(shader == NULL) {
        LTW_ERROR_PRINTF("LTWShdrWp: failed to patch frag data location due to missing shader info");
        goto fallthrough;
    }
    int nsrc_size = (int)(strlen(shader->source) + 1);
    char* new_source = (char*)malloc(nsrc_size);
    memcpy(new_source, shader->source, nsrc_size);
    bool changesMade = false;
    for(GLuint i = 0; i < MAX_DRAWBUFFERS; i++) {
        const char* colorbind = program_info->colorbindings[i];
        if(colorbind == NULL) continue;
        insert_fragout_pos(new_source, &nsrc_size, colorbind, i);
        changesMade = true;
    }
    if(!changesMade) {
        free(new_source);
        goto fallthrough;
    }else {
        //printf("\n\n\nShader Result POST PATCH\n%s\n\n\n", new_source);
    }
    const GLchar* const_source = (const GLchar*)new_source;
    GLuint patched_shader = es3_functions.glCreateShader(GL_FRAGMENT_SHADER);
    if(patched_shader == 0) {
        free(new_source);
        LTW_ERROR_PRINTF("LTWShdrWp: failed to initialize patched shader");
        goto fallthrough;
    }
    es3_functions.glShaderSource(patched_shader, 1, &const_source, NULL);
    es3_functions.glCompileShader(patched_shader);
    free(new_source);
    GLint compileStatus;
    es3_functions.glGetShaderiv(patched_shader, GL_COMPILE_STATUS, &compileStatus);
    if(compileStatus != GL_TRUE) {
        GLint logSize;
        es3_functions.glGetShaderiv(patched_shader, GL_INFO_LOG_LENGTH, &logSize);
        if(logSize > 0) {
            GLchar* log = (GLchar*)malloc(logSize + 1);
            if(log) {
                es3_functions.glGetShaderInfoLog(patched_shader, logSize, NULL, log);
                LTW_ERROR_PRINTF("LTWShdrWp: failed to compile patched fragment shader, using default. Log:\n\n%s\n\nShader content:\n\n%s\n\n", log, const_source);
                free(log);
            }
        }
        es3_functions.glDeleteShader(patched_shader);
        goto fallthrough;
    }
    es3_functions.glDetachShader(program, program_info->frag_shader);
    es3_functions.glAttachShader(program, patched_shader);
    es3_functions.glLinkProgram(program);
    es3_functions.glDeleteShader(patched_shader);
    return;
    fallthrough:
    GLTRACE_CALL(glLinkProgram, es3_functions.glLinkProgram(program));
}

GLuint glCreateShader(GLenum shaderType) {
    if(!current_context) return 0;
    GLuint phys_shader;
    GLTRACE_CALL(glCreateShader, phys_shader = es3_functions.glCreateShader(shaderType));
    if(phys_shader == 0) return 0;
    shader_info_t* info_struct = mempool_alloc(current_context->shader_info_pool);
    if(info_struct == NULL) {
        LTW_ERROR_PRINTF("LTWShdrWp: failed to allocate shader_info from pool");
        abort();
    }
    memset(info_struct, 0, sizeof(shader_info_t));
    info_struct->shader_type = shaderType;
    unordered_map_put(current_context->shader_map, (void*)phys_shader, info_struct);
    return phys_shader;
}

void glDeleteShader(GLuint shader) {
    if(!current_context) return;
    GLTRACE_CALL(glDeleteShader, es3_functions.glDeleteShader(shader));
    shader_info_t * old_shaderinfo = unordered_map_remove(current_context->shader_map, (void*)shader);
    if(old_shaderinfo == NULL) return;
    if(old_shaderinfo->source != NULL) free((void*)old_shaderinfo->source);
    mempool_free(current_context->shader_info_pool, old_shaderinfo);
}

void glShaderSource(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length) {
    if(!current_context) return;
    shader_info_t* shader_info = unordered_map_get(current_context->shader_map, (void*)shader);
    if(shader_info == NULL) {
        LTW_ERROR_PRINTF("LTWShdrWp: shader_info missing for shader %u", shader);
        es3_functions.glShaderSource(shader, count, string, length);
        return;
    }

    size_t target_length = 0;
#define SRC_LEN(x) length != NULL ? length[x] : strlen(string[x])
    for(GLsizei i = 0; i < count; i++) target_length += SRC_LEN(i);
    GLchar* target_string = malloc((target_length + 1) * sizeof(GLchar));
    size_t offset = 0;
    for(GLsizei i = 0; i < count; i++) {
        memcpy(&target_string[offset], string[i], SRC_LEN(i));
        offset += SRC_LEN(i);
    }
    target_string[target_length] = 0;

#undef SRC_LEN

    size_t source_hash = hash_string(target_string);
    size_t source_hash2 = hash_fnv1a(target_string);
    GLchar* cached_source = cache_lookup(source_hash, source_hash2, shader_info->shader_type);

    if (cached_source != NULL) {
        if(shader_info->source != NULL) free((void*)shader_info->source);
        char* new_source = strdup(cached_source);
        if(!new_source) {
            LTW_ERROR_PRINTF("LTWShdrWp: failed to duplicate cached shader source");
            free(target_string);
            return;
        }
        shader_info->source = new_source;
        es3_functions.glShaderSource(shader, 1, &shader_info->source, 0);
        free(target_string);

        #ifdef SHADER_CACHE_STATS
        // 每100次编译后打印统计信息
        static int compile_count = 0;
        if (++compile_count % 100 == 0) {
            print_cache_stats();
        }
        #endif

        return;
    }

    GLchar* new_source = optimize_shader(target_string, shader_info->shader_type, 460, current_context->shader_version);
    if (new_source == NULL) {
        // Conversion failed: fall back to the unmodified source instead of
        // handing the driver a NULL pointer.
        LTW_ERROR_PRINTF("LTWShdrWp: shader conversion failed for shader %u, using unmodified source", shader);
        new_source = target_string;
        target_string = NULL; // ownership transferred to new_source
    } else {
        // ESSL 3.0 removed the gl_FragColor/gl_FragData built-ins, rewrite
        // them into explicit outputs (e.g. Minecraft <=1.16 post shaders).
        GLchar* rewritten = rewrite_frag_outputs(new_source);
        if (rewritten != NULL) {
            free(new_source);
            new_source = rewritten;
        }
        cache_store(source_hash, source_hash2, shader_info->shader_type, new_source);
    }
    if(shader_info->source != NULL) free((void*)shader_info->source);
    shader_info->source = new_source;
    GLTRACE_CALL(glShaderSource, es3_functions.glShaderSource(shader, 1, &shader_info->source, 0));
    free(target_string);
}

// ---------------------------------------------------------------------------
// GL_ARB_shader_objects / GL_ARB_vertex_shader / GL_ARB_fragment_shader entry
// points. LWJGL2-era games (Minecraft <=1.16) probe for these extensions and,
// when found, route all shader work through the ObjectARB/ARB-named APIs. The
// GLES driver does not export them, so mirror gl4es' approach: register them
// here and translate to the already-wrapped GL20 entry points. GLhandleARB is
// just a GLuint, and the ARB pnames (GL_OBJECT_*_ARB) share values with the
// core GL ones, so no extra value translation is needed.
// ---------------------------------------------------------------------------
static bool ltws_is_shader(GLuint obj) {
    return current_context && unordered_map_get(current_context->shader_map, (void*)obj) != NULL;
}
static bool ltws_is_program(GLuint obj) {
    return current_context && unordered_map_get(current_context->program_map, (void*)obj) != NULL;
}

GLhandleARB glCreateShaderObjectARB(GLenum shaderType) { return glCreateShader(shaderType); }
GLhandleARB glCreateProgramObjectARB(void) { return glCreateProgram(); }
void glShaderSourceARB(GLhandleARB shaderObj, GLsizei count, const GLcharARB **string, const GLint *length) {
    glShaderSource((GLuint)shaderObj, count, (const GLchar *const*)string, length);
}
void glCompileShaderARB(GLhandleARB shaderObj) { glCompileShader((GLuint)shaderObj); }
void glAttachObjectARB(GLhandleARB containerObj, GLhandleARB obj) { glAttachShader((GLuint)containerObj, (GLuint)obj); }
void glDetachObjectARB(GLhandleARB containerObj, GLhandleARB obj) { glDetachShader((GLuint)containerObj, (GLuint)obj); }
void glLinkProgramARB(GLhandleARB programObj) { glLinkProgram((GLuint)programObj); }
void glUseProgramObjectARB(GLhandleARB programObj) { glUseProgram((GLuint)programObj); }
void glValidateProgramARB(GLhandleARB programObj) { glValidateProgram((GLuint)programObj); }
GLint glGetUniformLocationARB(GLhandleARB programObj, const GLcharARB *name) { return glGetUniformLocation((GLuint)programObj, (const GLchar*)name); }
GLint glGetAttribLocationARB(GLhandleARB programObj, const GLcharARB *name) { return glGetAttribLocation((GLuint)programObj, (const GLchar*)name); }
void glBindAttribLocationARB(GLhandleARB programObj, GLuint index, const GLcharARB *name) { glBindAttribLocation((GLuint)programObj, index, (const GLchar*)name); }

void glDeleteObjectARB(GLhandleARB obj) {
    if(ltws_is_program((GLuint)obj)) glDeleteProgram((GLuint)obj);
    else if(ltws_is_shader((GLuint)obj)) glDeleteShader((GLuint)obj);
}

void glGetObjectParameterivARB(GLhandleARB obj, GLenum pname, GLint *params) {
    if(ltws_is_program((GLuint)obj)) {
        es3_functions.glGetProgramiv((GLuint)obj, pname, params);
    } else if(ltws_is_shader((GLuint)obj)) {
        glGetShaderiv((GLuint)obj, pname, params);
    }
}

void glGetObjectParameterfvARB(GLhandleARB obj, GLenum pname, GLfloat *params) {
    GLint p = 0;
    glGetObjectParameterivARB(obj, pname, &p);
    params[0] = (GLfloat)p;
}

void glGetInfoLogARB(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog) {
    if(ltws_is_program(obj)) es3_functions.glGetProgramInfoLog((GLuint)obj, maxLength, length, (GLchar*)infoLog);
    else if(ltws_is_shader(obj)) es3_functions.glGetShaderInfoLog((GLuint)obj, maxLength, length, (GLchar*)infoLog);
}

void glGetAttachedObjectsARB(GLhandleARB containerObj, GLsizei maxCount, GLsizei *count, GLhandleARB *obj) {
    es3_functions.glGetAttachedShaders((GLuint)containerObj, maxCount, count, (GLuint*)obj);
}
