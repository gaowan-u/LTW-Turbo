//
// Created by maks on 12.07.2025.
//
#include "env.h"
#include <stdlib.h>
#include "libraryinternal.h"
#include <stdio.h>

INTERNAL bool env_istrue_d(const char* name, bool _default) {
    const char* env = getenv(name);
    if(env == NULL) return _default;
    return *env == '1';
}

INTERNAL bool env_istrue(const char* name) {
    const char* env = getenv(name);
    return env != NULL && *env == '1';
}

// 检测设备总内存（单位：MB）
INTERNAL size_t detect_device_memory_mb(void) {
    FILE* meminfo = fopen("/proc/meminfo", "r");
    if(!meminfo) {
        LTW_ERROR_PRINTF("LTW: Failed to open /proc/meminfo, using default memory size");
        return 4096; // 默认 4GB
    }

    char line[256];
    size_t total_mem_kb = 0;

    while(fgets(line, sizeof(line), meminfo)) {
        if(sscanf(line, "MemTotal: %zu kB", &total_mem_kb) == 1) {
            break;
        }
    }

    fclose(meminfo);

    if(total_mem_kb == 0) {
        LTW_ERROR_PRINTF("LTW: Failed to read MemTotal from /proc/meminfo, using default");
        return 4096;
    }

    size_t total_mem_mb = total_mem_kb / 1024;
    LTW_ERROR_PRINTF("LTW: Detected device memory: %zu MB", total_mem_mb);

    return total_mem_mb;
}