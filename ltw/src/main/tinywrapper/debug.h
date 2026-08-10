/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

/**
 * 文件功能：调试输出宏。
 *
 * 提供 LTW_DEBUG_PRINTF（LTW_DEBUG 开关）与 LTW_ERROR_PRINTF（运行时
 * 错误提示）。GL 错误双排空追踪（GLTRACE_CALL / glerr_trace / LTW_ENTER）
 * 已在 Post render 1282 定位完成后移除。
 */
#ifndef POJAVLAUNCHER_DEBUG_H
#define POJAVLAUNCHER_DEBUG_H

#include <stdbool.h>

extern bool debug;

#define LTW_DEBUG_PRINTF(fmt, ...) do { if(debug) printf("[LTW DEBUG] " fmt "\n", ##__VA_ARGS__); } while(0)

#define LTW_ERROR_PRINTF(fmt, ...) printf("[LTW ERROR] " fmt "\n", ##__VA_ARGS__)

#endif //POJAVLAUNCHER_DEBUG_H
