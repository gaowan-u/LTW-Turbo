/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

/**
 * 文件功能：内部符号可见性宏。
 *
 * INTERNAL = __attribute__((visibility("hidden")))，用于把纯内部
 * 函数从动态库导出表隐藏。
 */
#ifndef POJAVLAUNCHER_LIBRARYINTERNAL_H
#define POJAVLAUNCHER_LIBRARYINTERNAL_H

#define INTERNAL __attribute__((visibility("hidden")))

#endif //POJAVLAUNCHER_LIBRARYINTERNAL_H
