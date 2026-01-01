/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */
#include "int_hash.h"
#include "../libraryinternal.h"
#include "../debug.h"

static inline size_t rotl64(size_t x, int r) {
    return (x << r) | (x >> (64 - r));
}

static size_t intmap_hash(void* key) {
    size_t k = (size_t)key;
    k ^= k >> 33;
    k *= 0xff51afd7ed558ccdULL;
    k ^= k >> 33;
    k *= 0xc4ceb9fe1a85ec53ULL;
    k ^= k >> 33;
    return k;
}

static bool intmap_equals(void* v1, void* v2) {
    return v1 == v2;
}

INTERNAL unordered_map* alloc_intmap_safe() {
    return unordered_map_alloc(0, 1, intmap_hash, intmap_equals);
}

INTERNAL unordered_map* alloc_intmap() {
    unordered_map* map = alloc_intmap_safe();
    if(map == NULL) {
        LTW_ERROR_PRINTF("failed to alloc_intmap");
        abort();
    }
    return map;
}