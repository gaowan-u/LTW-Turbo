/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

#ifndef POJAVLAUNCHER_DEBUG_H
#define POJAVLAUNCHER_DEBUG_H

#include <stdbool.h>

extern bool debug;

#ifdef LTW_DEBUG
    #define LTW_DEBUG_PRINTF(fmt, ...) printf("[LTW DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
    #define LTW_DEBUG_PRINTF(fmt, ...) ((void)0)
#endif

#define LTW_ERROR_PRINTF(fmt, ...) printf("[LTW ERROR] " fmt "\n", ##__VA_ARGS__)

#endif //POJAVLAUNCHER_DEBUG_H