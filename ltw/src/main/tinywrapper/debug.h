/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

#ifndef POJAVLAUNCHER_DEBUG_H
#define POJAVLAUNCHER_DEBUG_H

#include <stdbool.h>

extern bool debug;
// GL error queue tracing. Default ON while debugging; flip to false once
// the 1280 INVALID_ENUM hunt is done (no launcher-side env vars available).
extern bool glerr_trace;
// Name of the LTW wrapper function currently executing on this thread
// (set by LTW_ENTER), printed by the GL debug callback to attribute errors.
extern _Thread_local const char* ltw_last_glfn;
#define LTW_ENTER(fn) (ltw_last_glfn = (fn))
#define LTW_EXIT() (ltw_last_glfn = NULL)

#define LTW_DEBUG_PRINTF(fmt, ...) do { if(debug) printf("[LTW DEBUG] " fmt "\n", ##__VA_ARGS__); } while(0)

#define LTW_ERROR_PRINTF(fmt, ...) printf("[LTW ERROR] " fmt "\n", ##__VA_ARGS__)

// Sample the driver error queue after a call (every 1024th call, zero
// overhead otherwise). Clears pending errors it reads, so the caller must
// not rely on glGetError() after a trace point. Optional extra args are
// printf-style, e.g. GLERR_CHECK("glTexParameteri p=0x%x", pname).
#define GLERR_CHECK(fn, ...) do { if(glerr_trace) { \
    static unsigned int _glerr_n = 0; \
    if((++_glerr_n & 0x3FFu) == 0) { \
        GLenum _e = es3_functions.glGetError(); \
        if(_e != GL_NO_ERROR) printf("[LTW ERROR] GL error 0x%x after " fn "\n", (unsigned)_e, ##__VA_ARGS__); \
    } \
} } while(0)

// Double-drain trace wrapper: drains stale errors, runs the call, then
// checks for fresh errors - proves whether THIS call produced them.
// Samples 1/16 of the time; zero overhead when glerr_trace is off.
#define GLTRACE_CALL(fn, call_stmt) do { \
    if(glerr_trace) { \
        static unsigned int _g_n = 0; \
        if((++_g_n & 0xF) == 0) { \
            GLenum _s = es3_functions.glGetError(); \
            call_stmt; \
            GLenum _f = es3_functions.glGetError(); \
            if(_s != GL_NO_ERROR) printf("[LTW ERROR] stale 0x%x before " #fn "\n", (unsigned)_s); \
            if(_f != GL_NO_ERROR) printf("[LTW ERROR] " #fn " produced 0x%x\n", (unsigned)_f); \
            break; \
        } \
    } \
    call_stmt; \
} while(0)

#endif //POJAVLAUNCHER_DEBUG_H