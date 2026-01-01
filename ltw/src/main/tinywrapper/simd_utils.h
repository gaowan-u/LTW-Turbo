/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

#ifndef LTW_SIMD_UTILS_H
#define LTW_SIMD_UTILS_H

#include <stdint.h>

// ARM NEON SIMD 优化
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>

// 使用 NEON 进行批量归一化转换
static inline void normalize_4floats_neon(const float* src, float* dst, float scale) {
    float32x4_t v = vld1q_f32(src);
    v = vmulq_n_f32(v, scale);
    vst1q_f32(dst, v);
}

// 使用 NEON 进行 BGRA 到 RGBA 转换
static inline void bgra_to_rgba_neon(const uint8_t* src, uint8_t* dst, int count) {
    int i = 0;
    for (; i + 16 <= count; i += 16) {
        uint8x16x4_t v = vld4q_u8(src + i * 4);
        // 交换 R 和 B 通道
        uint8x16_t temp = v.val[0];
        v.val[0] = v.val[2];
        v.val[2] = temp;
        vst4q_u8(dst + i * 4, v);
    }
    // 处理剩余数据
    for (; i < count; i++) {
        dst[i * 4 + 0] = src[i * 4 + 2]; // R <- B
        dst[i * 4 + 1] = src[i * 4 + 1]; // G <- G
        dst[i * 4 + 2] = src[i * 4 + 0]; // B <- R
        dst[i * 4 + 3] = src[i * 4 + 3]; // A <- A
    }
}

// 使用 NEON 进行字节序翻转
static inline void swap_endian_4bytes_neon(const uint8_t* src, uint8_t* dst, int count) {
    int i = 0;
    for (; i + 16 <= count; i += 16) {
        uint8x16_t v = vld1q_u8(src + i);
        // 反转字节序
        uint8x16_t reversed = vrev64q_u8(v);
        vst1q_u8(dst + i, reversed);
    }
    // 处理剩余数据
    for (; i < count; i++) {
        dst[i] = src[count - 1 - i];
    }
}

#define LTW_HAS_NEON 1

#else

// 非 NEON 平台的回退实现
static inline void normalize_4floats_neon(const float* src, float* dst, float scale) {
    dst[0] = src[0] * scale;
    dst[1] = src[1] * scale;
    dst[2] = src[2] * scale;
    dst[3] = src[3] * scale;
}

static inline void bgra_to_rgba_neon(const uint8_t* src, uint8_t* dst, int count) {
    for (int i = 0; i < count; i++) {
        dst[i * 4 + 0] = src[i * 4 + 2]; // R <- B
        dst[i * 4 + 1] = src[i * 4 + 1]; // G <- G
        dst[i * 4 + 2] = src[i * 4 + 0]; // B <- R
        dst[i * 4 + 3] = src[i * 4 + 3]; // A <- A
    }
}

static inline void swap_endian_4bytes_neon(const uint8_t* src, uint8_t* dst, int count) {
    for (int i = 0; i < count; i++) {
        dst[i] = src[count - 1 - i];
    }
}

#define LTW_HAS_NEON 0

#endif // __ARM_NEON__

#endif // LTW_SIMD_UTILS_H