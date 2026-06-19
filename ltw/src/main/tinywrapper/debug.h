/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

#ifndef POJAVLAUNCHER_DEBUG_H
#define POJAVLAUNCHER_DEBUG_H

#include <stdbool.h>

extern bool debug;

#define LTW_DEBUG_PRINTF(fmt, ...) do { if(debug) printf("[LTW DEBUG] " fmt "\n", ##__VA_ARGS__); } while(0)

#define LTW_ERROR_PRINTF(fmt, ...) printf("[LTW ERROR] " fmt "\n", ##__VA_ARGS__)

#endif //POJAVLAUNCHER_DEBUG_H