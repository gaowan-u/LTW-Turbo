/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

/**
 * 文件功能：main.c 内部接口声明。
 *
 * 提供 GL 缓冲目标的索引映射（get_buffer_index 等）与
 * glLTWBeginBatchUpdate/EndBatchUpdate 批量更新开关。
 */
#ifndef POJAVLAUNCHER_MAIN_H
#define POJAVLAUNCHER_MAIN_H

int get_buffer_index(GLenum buffer);
int get_base_buffer_index(GLenum buffer);
GLenum get_base_buffer_enum(int buffer_index);

// 批量更新相关函数
void glLTWBeginBatchUpdate(void);
void glLTWEndBatchUpdate(void);

#endif //POJAVLAUNCHER_MAIN_H
