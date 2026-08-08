/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

/**
 * 文件功能：整数键哈希表便捷接口。
 *
 * 提供以 (void*)(intptr_t) 为键的哈希/相等函数与分配器
 * （alloc_intmap / alloc_intmap_safe），供 context 内各映射表使用。
 */
#ifndef POJAVLAUNCHER_INT_HASH_H
#define POJAVLAUNCHER_INT_HASH_H
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "unordered_map.h"
unordered_map* alloc_intmap_safe();
unordered_map* alloc_intmap();

#endif //POJAVLAUNCHER_INT_HASH_H
