/**
 * 轻量扁平 JSON 读取器，只服务 LTW-Turbo 自己的共享配置文件：
 *   /sdcard/LTW-Turbo/config.json（可用 LTW_CONFIG_DIR 覆盖目录）
 * 文件由设置 App 写入，格式为 { "dlMerge": true, ... }。
 * 不引入 cJSON，解析器只支持本应用会生成的 int/bool/string 字段。
 */
#include "ltw_config.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LTW_CFG_MAX_KEYS     32
#define LTW_CFG_MAX_FILE     (64 * 1024)
#define LTW_CFG_KEY_MAX      47
#define LTW_CFG_STR_MAX      127
#define LTW_CFG_DEFAULT_PATH "/sdcard/LTW-Turbo/config.json"

typedef enum {
    LTW_CFG_INT,
    LTW_CFG_BOOL,
    LTW_CFG_STRING
} ltw_cfg_type_t;

typedef struct {
    char key[LTW_CFG_KEY_MAX + 1];
    ltw_cfg_type_t type;
    long num;
    char str[LTW_CFG_STR_MAX + 1];
    bool present;
} ltw_cfg_entry_t;

static ltw_cfg_entry_t ltw_cfg_entries[LTW_CFG_MAX_KEYS];
static int ltw_cfg_count = 0;
static bool ltw_cfg_loaded = false;
static char ltw_cfg_buf[LTW_CFG_MAX_FILE];

static void ltw_cfg_reset(void) {
    memset(ltw_cfg_entries, 0, sizeof(ltw_cfg_entries));
    ltw_cfg_count = 0;
    ltw_cfg_loaded = false;
}

static const char* ltw_cfg_skip_ws(const char* p) {
    while(*p && isspace((unsigned char)*p)) p++;
    return p;
}

static bool ltw_cfg_parse_quoted(const char** pp, char* out, size_t outcap) {
    const char* p = *pp;
    if(*p != '"') return false;
    p++;
    size_t n = 0;
    while(*p && *p != '"') {
        if(*p == '\\' && p[1]) p++;   // 只跳过转义符，键/值都是简单 ASCII
        if(n + 1 < outcap) out[n++] = *p;
        p++;
    }
    if(*p != '"') return false;
    out[n] = '\0';
    *pp = p + 1;
    return true;
}

static void ltw_cfg_parse(const char* text) {
    const char* p = ltw_cfg_skip_ws(text);
    if(*p != '{') return;
    p++;

    while(*p) {
        p = ltw_cfg_skip_ws(p);
        if(*p == '}') break;
        if(*p == ',') { p++; continue; }

        char key[LTW_CFG_KEY_MAX + 1];
        if(!ltw_cfg_parse_quoted(&p, key, sizeof(key))) break;
        p = ltw_cfg_skip_ws(p);
        if(*p != ':') break;
        p = ltw_cfg_skip_ws(p + 1);
        if(ltw_cfg_count >= LTW_CFG_MAX_KEYS) break;

        ltw_cfg_entry_t* e = &ltw_cfg_entries[ltw_cfg_count];
        snprintf(e->key, sizeof(e->key), "%s", key);
        if(*p == '"') {
            if(!ltw_cfg_parse_quoted(&p, e->str, sizeof(e->str))) break;
            e->type = LTW_CFG_STRING;
        } else if(strncmp(p, "true", 4) == 0 && !isalnum((unsigned char)p[4])) {
            e->type = LTW_CFG_BOOL;
            e->num = 1;
            p += 4;
        } else if(strncmp(p, "false", 5) == 0 && !isalnum((unsigned char)p[5])) {
            e->type = LTW_CFG_BOOL;
            e->num = 0;
            p += 5;
        } else {
            char* end = NULL;
            long v = strtol(p, &end, 10);
            if(end == p) break;
            e->type = LTW_CFG_INT;
            e->num = v;
            p = end;
        }
        e->present = true;
        ltw_cfg_count++;
        p = ltw_cfg_skip_ws(p);
        if(*p == ',') p++;
    }
}

bool ltw_config_init(void) {
    if(ltw_cfg_loaded) return ltw_cfg_count > 0;
    ltw_cfg_loaded = true;

    char path[512];
    const char* dir = getenv("LTW_CONFIG_DIR");
    if(dir && *dir) {
        snprintf(path, sizeof(path), "%s/config.json", dir);
    } else {
        snprintf(path, sizeof(path), "%s", LTW_CFG_DEFAULT_PATH);
    }

    FILE* f = fopen(path, "r");
    if(!f) return false;
    size_t n = fread(ltw_cfg_buf, 1, sizeof(ltw_cfg_buf) - 1, f);
    fclose(f);
    ltw_cfg_buf[n] = '\0';
    ltw_cfg_parse(ltw_cfg_buf);
    return ltw_cfg_count > 0;
}

static const ltw_cfg_entry_t* ltw_cfg_find(const char* key) {
    if(!ltw_config_init()) return NULL;
    for(int i = 0; i < ltw_cfg_count; i++) {
        if(ltw_cfg_entries[i].present && strcmp(ltw_cfg_entries[i].key, key) == 0) {
            return &ltw_cfg_entries[i];
        }
    }
    return NULL;
}

bool ltw_config_get_bool(const char* key, bool def) {
    const ltw_cfg_entry_t* e = ltw_cfg_find(key);
    if(!e) return def;
    if(e->type == LTW_CFG_BOOL || e->type == LTW_CFG_INT) return e->num != 0;
    return def;
}

int ltw_config_get_int(const char* key, int def) {
    const ltw_cfg_entry_t* e = ltw_cfg_find(key);
    if(!e || e->type != LTW_CFG_INT) return def;
    return (int)e->num;
}

const char* ltw_config_get_string(const char* key, const char* def) {
    const ltw_cfg_entry_t* e = ltw_cfg_find(key);
    if(!e || e->type != LTW_CFG_STRING) return def;
    return e->str;
}

void ltw_config_cleanup(void) {
    ltw_cfg_reset();
}
