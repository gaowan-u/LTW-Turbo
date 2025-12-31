/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

#ifndef POJAVLAUNCHER_MEMPOOL_H
#define POJAVLAUNCHER_MEMPOOL_H

#include <stddef.h>
#include <stdbool.h>

#define MEMPOOL_MAX_POOLS 8
#define MEMPOOL_CHUNK_SIZE 64
#define MEMPOOL_ALIGNMENT 16

typedef struct mempool_block {
    struct mempool_block* next;
} mempool_block_t;

typedef struct mempool {
    size_t object_size;
    size_t chunk_size;
    mempool_block_t* free_list;
    void* chunks;
    size_t total_chunks;
    size_t used_count;
    size_t free_count;
} mempool_t;

// 创建内存池
mempool_t* mempool_create(size_t object_size, size_t chunk_size);

// 销毁内存池
void mempool_destroy(mempool_t* pool);

// 从内存池分配
void* mempool_alloc(mempool_t* pool);

// 释放到内存池
void mempool_free(mempool_t* pool, void* ptr);

// 获取内存池统计信息
void mempool_get_stats(mempool_t* pool, size_t* total, size_t* used, size_t* free);

// 重置内存池（释放所有分配的对象）
void mempool_reset(mempool_t* pool);

#endif //POJAVLAUNCHER_MEMPOOL_H