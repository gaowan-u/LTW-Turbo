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
    size_t source_hash;
    GLenum shader_type;
    GLchar* optimized_source;
    size_t source_size;  // 源代码大小，用于内存管理
    uint32_t access_count;  // 访问计数，用于 LRU
    uint64_t last_access;   // 最后访问时间
} shader_cache_entry_t;

static shader_cache_entry_t shader_cache[SHADER_CACHE_SIZE] = {0};
static int shader_cache_count = 0;
static uint64_t cache_hits = 0;
static uint64_t cache_misses = 0;
static size_t cache_total_memory = 0;  // 缓存总内存使用量

static inline size_t hash_string(const char* str) {
    size_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

static GLchar* get_cached_shader(size_t hash, GLenum shader_type) {
    for (int i = 0; i < shader_cache_count; i++) {
        if (shader_cache[i].source_hash == hash && 
            shader_cache[i].shader_type == shader_type) {
            // 更新访问统计
            shader_cache[i].access_count++;
            shader_cache[i].last_access = get_timestamp();
            #ifdef SHADER_CACHE_STATS
            cache_hits++;
            #endif
            return shader_cache[i].optimized_source;
        }
    }
    #ifdef SHADER_CACHE_STATS
    cache_misses++;
    #endif
    return NULL;
}

static void cache_shader(size_t hash, GLenum shader_type, const GLchar* optimized_source) {
    size_t source_size = strlen(optimized_source) + 1;

    // 检查单个着色器是否过大
    if (source_size > SHADER_CACHE_MAX_MEMORY / 2) {
        LTW_DEBUG_PRINTF("LTW: shader source too large for cache: %zu bytes", source_size);
        return;
    }

    if (shader_cache_count >= SHADER_CACHE_SIZE || cache_total_memory + source_size > SHADER_CACHE_MAX_MEMORY) {
        // LRU 策略：替换访问次数最少且最久未使用的条目
        int lru_index = 0;
        uint32_t min_access = shader_cache[0].access_count;
        uint64_t oldest_access = shader_cache[0].last_access;

        for (int i = 1; i < shader_cache_count; i++) {
            if (shader_cache[i].access_count < min_access ||
                (shader_cache[i].access_count == min_access &&
                 shader_cache[i].last_access < oldest_access)) {
                min_access = shader_cache[i].access_count;
                oldest_access = shader_cache[i].last_access;
                lru_index = i;
            }
        }

        // 释放被替换的条目
        if (shader_cache[lru_index].optimized_source) {
            cache_total_memory -= shader_cache[lru_index].source_size;
            free(shader_cache[lru_index].optimized_source);
        }

        // 插入新条目
        shader_cache[lru_index].source_hash = hash;
        shader_cache[lru_index].shader_type = shader_type;
        shader_cache[lru_index].optimized_source = strdup(optimized_source);
        shader_cache[lru_index].source_size = source_size;
        shader_cache[lru_index].access_count = 1;
        shader_cache[lru_index].last_access = get_timestamp();
        cache_total_memory += source_size;
    } else {
        // 缓存未满，直接添加
        shader_cache[shader_cache_count].source_hash = hash;
        shader_cache[shader_cache_count].shader_type = shader_type;
        shader_cache[shader_cache_count].optimized_source = strdup(optimized_source);
        shader_cache[shader_cache_count].source_size = source_size;
        shader_cache[shader_cache_count].access_count = 1;
        shader_cache[shader_cache_count].last_access = get_timestamp();
        cache_total_memory += source_size;
        shader_cache_count++;
    }
}

#ifdef SHADER_CACHE_STATS
static void print_cache_stats(void) {
    if (cache_hits + cache_misses > 0) {
        float hit_rate = (float)cache_hits / (cache_hits + cache_misses) * 100.0f;
        LTW_DEBUG_PRINTF("Shader cache stats: hits=%llu, misses=%llu, hit_rate=%.2f%%, size=%d/%d, memory=%.2fMB/%.2fMB",
                        cache_hits, cache_misses, hit_rate, shader_cache_count, SHADER_CACHE_SIZE,
                        cache_total_memory / (1024.0f * 1024.0f), SHADER_CACHE_MAX_MEMORY / (1024.0f * 1024.0f));
    }
}
#endif

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
    es3_functions.glDeleteProgram(program);
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
    es3_functions.glAttachShader(program, shader);
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
    es3_functions.glGetShaderiv(shader, pname, params);
}

static void insert_fragout_pos(char* source, int* size, const char* name, GLuint pos) {
    char src_string[256] = { 0 };
    char dst_string[256] = { 0 };
    snprintf(src_string, sizeof(src_string), "/* LTW INSERT LOCATION %s LTW */", name);
    snprintf(dst_string, sizeof(dst_string), "layout(location = %u) ", pos);
    gl4es_inplace_replace_simple(source, size, src_string, dst_string);
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
        GLchar log[logSize];
        es3_functions.glGetShaderInfoLog(patched_shader, logSize, NULL, log);
        LTW_ERROR_PRINTF("LTWShdrWp: failed to compile patched fragment shader, using default. Log:\n\n%s\n\nShader content:\n\n%s\n\n", log, const_source);
        goto fallthrough;
    }
    es3_functions.glDetachShader(program, program_info->frag_shader);
    es3_functions.glAttachShader(program, patched_shader);
    es3_functions.glLinkProgram(program);
    es3_functions.glDeleteShader(patched_shader);
    return;
    fallthrough:
    es3_functions.glLinkProgram(program);
}

GLuint glCreateShader(GLenum shaderType) {
    if(!current_context) return 0;
    GLuint phys_shader = es3_functions.glCreateShader(shaderType);
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
    es3_functions.glDeleteShader(shader);
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
    GLchar* cached_source = get_cached_shader(source_hash, shader_info->shader_type);

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
    cache_shader(source_hash, shader_info->shader_type, new_source);
    //printf("\n\n\nShader Result\n%s\n\n\n", new_source);
    if(shader_info->source != NULL) free((void*)shader_info->source);
    shader_info->source = new_source;
    es3_functions.glShaderSource(shader, 1, &shader_info->source, 0);
    free(target_string);
}
