/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

#include "mempool.h"
#include "debug.h"
#include <stdlib.h>
#include <string.h>

// 将大小对齐到指定边界
static inline size_t align_size(size_t size, size_t alignment) {
    return (size + alignment - 1) & ~(alignment - 1);
}

mempool_t* mempool_create(size_t object_size, size_t chunk_size) {
    mempool_t* pool = calloc(1, sizeof(mempool_t));
    if (!pool) {
        LTW_ERROR_PRINTF("mempool: failed to allocate pool structure");
        return NULL;
    }

    // 对齐对象大小，确保至少能存储指针
    pool->object_size = align_size(object_size, MEMPOOL_ALIGNMENT);
    if (pool->object_size < sizeof(void*)) {
        pool->object_size = align_size(sizeof(void*), MEMPOOL_ALIGNMENT);
    }

    pool->chunk_size = chunk_size > 0 ? chunk_size : MEMPOOL_CHUNK_SIZE;
    pool->free_list = NULL;
    pool->chunks = NULL;
    pool->total_chunks = 0;
    pool->used_count = 0;
    pool->free_count = 0;

    return pool;
}

void mempool_destroy(mempool_t* pool) {
    if (!pool) return;

    // 释放所有块
    void* chunk = pool->chunks;
    while (chunk) {
        void* next = *((void**)chunk);
        free(chunk);
        chunk = next;
    }

    free(pool);
}

static bool mempool_expand(mempool_t* pool) {
    // 检查乘法溢出
    if(pool->chunk_size > SIZE_MAX / pool->object_size) {
        LTW_ERROR_PRINTF("mempool: integer overflow in chunk size calculation (multiplication)");
        return false;
    }
    size_t objects_size = pool->object_size * pool->chunk_size;
    // 检查加法溢出
    if(objects_size > SIZE_MAX - sizeof(void*)) {
        LTW_ERROR_PRINTF("mempool: integer overflow in chunk size calculation (addition)");
        return false;
    }
    size_t chunk_size = objects_size + sizeof(void*);
    void* chunk = malloc(chunk_size);
    if (!chunk) {
        LTW_ERROR_PRINTF("mempool: failed to expand pool");
        return false;
    }

    // 将新块添加到块链表
    *((void**)chunk) = pool->chunks;
    pool->chunks = chunk;

    // 将新块中的所有对象添加到空闲列表
    char* data = (char*)chunk + sizeof(void*);
    for (size_t i = 0; i < pool->chunk_size; i++) {
        mempool_block_t* block = (mempool_block_t*)(data + i * pool->object_size);
        block->next = pool->free_list;
        pool->free_list = block;
    }

    pool->total_chunks++;
    pool->free_count += pool->chunk_size;

    return true;
}

void* mempool_alloc(mempool_t* pool) {
    if (!pool) return NULL;

    // 如果空闲列表为空，扩展内存池
    if (!pool->free_list) {
        if (!mempool_expand(pool)) {
            return NULL;
        }
    }

    // 从空闲列表取出一个块
    mempool_block_t* block = pool->free_list;
    pool->free_list = block->next;
    pool->used_count++;
    pool->free_count--;

    return block;
}

void mempool_free(mempool_t* pool, void* ptr) {
    if (!pool || !ptr) return;

    // 将块添加回空闲列表
    mempool_block_t* block = (mempool_block_t*)ptr;
    block->next = pool->free_list;
    pool->free_list = block;
    pool->used_count--;
    pool->free_count++;
}

void mempool_get_stats(mempool_t* pool, size_t* total, size_t* used, size_t* free) {
    if (!pool) return;

    if (total) *total = pool->total_chunks * pool->chunk_size;
    if (used) *used = pool->used_count;
    if (free) *free = pool->free_count;
}

void mempool_reset(mempool_t* pool) {
    if (!pool) return;

    // 重建空闲列表
    pool->free_list = NULL;
    pool->used_count = 0;
    pool->free_count = 0;

    void* chunk = pool->chunks;
    while (chunk) {
        char* data = (char*)chunk + sizeof(void*);
        for (size_t i = 0; i < pool->chunk_size; i++) {
            mempool_block_t* block = (mempool_block_t*)(data + i * pool->object_size);
            block->next = pool->free_list;
            pool->free_list = block;
        }
        chunk = *((void**)chunk);
    }

    pool->free_count = pool->total_chunks * pool->chunk_size;
}