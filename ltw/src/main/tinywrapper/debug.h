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

// Sample the driver error queue after a call (LTW_DEBUG only, every 1024th
// call, zero overhead otherwise). Clears pending errors it reads, so the
// caller must not rely on glGetError() after a trace point.
#define GLERR_CHECK(fn) do { if(debug) { \
    static unsigned int _glerr_n = 0; \
    if((++_glerr_n & 0x3FFu) == 0) { \
        GLenum _e = es3_functions.glGetError(); \
        if(_e != GL_NO_ERROR) printf("[LTW ERROR] GL error 0x%x after " fn "\n", (unsigned)_e); \
    } \
} } while(0)

#endif //POJAVLAUNCHER_DEBUG_H