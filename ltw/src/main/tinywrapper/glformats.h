/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

/**
 * 文件功能：纹理格式选择接口声明（见 glformats.c）。
 */
#ifndef POJAVLAUNCHER_GLFORMATS_H
#define POJAVLAUNCHER_GLFORMATS_H

#include <GLES3/gl3.h>

extern void pick_internalformat(GLint *internalformat, GLenum* type, GLenum* format, GLvoid const** data);

#endif //POJAVLAUNCHER_GLFORMATS_H
