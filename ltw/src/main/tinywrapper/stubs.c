/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

//run:
#include <stdbool.h>
#include <stdio.h>
static bool trigger_glCullFace = false;
void stub_glCullFace() {
    if(trigger_glCullFace) return;
    trigger_glCullFace = true;
    printf("Stub: glCullFace\n");
}
static bool trigger_glFrontFace = false;
void stub_glFrontFace() {
    if(trigger_glFrontFace) return;
    trigger_glFrontFace = true;
    printf("Stub: glFrontFace\n");
}
static bool trigger_glHint = false;
void stub_glHint() {
    if(trigger_glHint) return;
    trigger_glHint = true;
    printf("Stub: glHint\n");
}
static bool trigger_glLineWidth = false;
void stub_glLineWidth() {
    if(trigger_glLineWidth) return;
    trigger_glLineWidth = true;
    printf("Stub: glLineWidth\n");
}
static bool trigger_glPointSize = false;
void stub_glPointSize() {
    if(trigger_glPointSize) return;
    trigger_glPointSize = true;
    printf("Stub: glPointSize\n");
}
static bool trigger_glPolygonMode = false;
void stub_glPolygonMode() {
    if(trigger_glPolygonMode) return;
    trigger_glPolygonMode = true;
    printf("Stub: glPolygonMode\n");
}
static bool trigger_glScissor = false;
void stub_glScissor() {
    if(trigger_glScissor) return;
    trigger_glScissor = true;
    printf("Stub: glScissor\n");
}
static bool trigger_glTexParameterf = false;
void stub_glTexParameterf() {
    if(trigger_glTexParameterf) return;
    trigger_glTexParameterf = true;
    printf("Stub: glTexParameterf\n");
}
static bool trigger_glTexParameterfv = false;
void stub_glTexParameterfv() {
    if(trigger_glTexParameterfv) return;
    trigger_glTexParameterfv = true;
    printf("Stub: glTexParameterfv\n");
}
static bool trigger_glTexParameteri = false;
void stub_glTexParameteri() {
    if(trigger_glTexParameteri) return;
    trigger_glTexParameteri = true;
    printf("Stub: glTexParameteri\n");
}
static bool trigger_glTexParameteriv = false;
void stub_glTexParameteriv() {
    if(trigger_glTexParameteriv) return;
    trigger_glTexParameteriv = true;
    printf("Stub: glTexParameteriv\n");
}
static bool trigger_glTexImage1D = false;
void stub_glTexImage1D() {
    if(trigger_glTexImage1D) return;
    trigger_glTexImage1D = true;
    printf("Stub: glTexImage1D\n");
}
static bool trigger_glTexImage2D = false;
void stub_glTexImage2D() {
    if(trigger_glTexImage2D) return;
    trigger_glTexImage2D = true;
    printf("Stub: glTexImage2D\n");
}
static bool trigger_glDrawBuffer = false;
void stub_glDrawBuffer() {
    if(trigger_glDrawBuffer) return;
    trigger_glDrawBuffer = true;
    printf("Stub: glDrawBuffer\n");
}
static bool trigger_glClear = false;
void stub_glClear() {
    if(trigger_glClear) return;
    trigger_glClear = true;
    printf("Stub: glClear\n");
}
static bool trigger_glClearColor = false;
void stub_glClearColor() {
    if(trigger_glClearColor) return;
    trigger_glClearColor = true;
    printf("Stub: glClearColor\n");
}
static bool trigger_glClearStencil = false;
void stub_glClearStencil() {
    if(trigger_glClearStencil) return;
    trigger_glClearStencil = true;
    printf("Stub: glClearStencil\n");
}
static bool trigger_glClearDepth = false;
void stub_glClearDepth() {
    if(trigger_glClearDepth) return;
    trigger_glClearDepth = true;
    printf("Stub: glClearDepth\n");
}
static bool trigger_glStencilMask = false;
void stub_glStencilMask() {
    if(trigger_glStencilMask) return;
    trigger_glStencilMask = true;
    printf("Stub: glStencilMask\n");
}
static bool trigger_glColorMask = false;
void stub_glColorMask() {
    if(trigger_glColorMask) return;
    trigger_glColorMask = true;
    printf("Stub: glColorMask\n");
}
static bool trigger_glDepthMask = false;
void stub_glDepthMask() {
    if(trigger_glDepthMask) return;
    trigger_glDepthMask = true;
    printf("Stub: glDepthMask\n");
}
static bool trigger_glDisable = false;
void stub_glDisable() {
    if(trigger_glDisable) return;
    trigger_glDisable = true;
    printf("Stub: glDisable\n");
}
static bool trigger_glEnable = false;
void stub_glEnable() {
    if(trigger_glEnable) return;
    trigger_glEnable = true;
    printf("Stub: glEnable\n");
}
static bool trigger_glFinish = false;
void stub_glFinish() {
    if(trigger_glFinish) return;
    trigger_glFinish = true;
    printf("Stub: glFinish\n");
}
static bool trigger_glFlush = false;
void stub_glFlush() {
    if(trigger_glFlush) return;
    trigger_glFlush = true;
    printf("Stub: glFlush\n");
}
static bool trigger_glBlendFunc = false;
void stub_glBlendFunc() {
    if(trigger_glBlendFunc) return;
    trigger_glBlendFunc = true;
    printf("Stub: glBlendFunc\n");
}
static bool trigger_glLogicOp = false;
void stub_glLogicOp() {
    if(trigger_glLogicOp) return;
    trigger_glLogicOp = true;
    printf("Stub: glLogicOp\n");
}
static bool trigger_glStencilFunc = false;
void stub_glStencilFunc() {
    if(trigger_glStencilFunc) return;
    trigger_glStencilFunc = true;
    printf("Stub: glStencilFunc\n");
}
static bool trigger_glStencilOp = false;
void stub_glStencilOp() {
    if(trigger_glStencilOp) return;
    trigger_glStencilOp = true;
    printf("Stub: glStencilOp\n");
}
static bool trigger_glDepthFunc = false;
void stub_glDepthFunc() {
    if(trigger_glDepthFunc) return;
    trigger_glDepthFunc = true;
    printf("Stub: glDepthFunc\n");
}
static bool trigger_glPixelStoref = false;
void stub_glPixelStoref() {
    if(trigger_glPixelStoref) return;
    trigger_glPixelStoref = true;
    printf("Stub: glPixelStoref\n");
}
static bool trigger_glPixelStorei = false;
void stub_glPixelStorei() {
    if(trigger_glPixelStorei) return;
    trigger_glPixelStorei = true;
    printf("Stub: glPixelStorei\n");
}
static bool trigger_glReadBuffer = false;
void stub_glReadBuffer() {
    if(trigger_glReadBuffer) return;
    trigger_glReadBuffer = true;
    printf("Stub: glReadBuffer\n");
}
static bool trigger_glReadPixels = false;
void stub_glReadPixels() {
    if(trigger_glReadPixels) return;
    trigger_glReadPixels = true;
    printf("Stub: glReadPixels\n");
}
static bool trigger_glGetBooleanv = false;
void stub_glGetBooleanv() {
    if(trigger_glGetBooleanv) return;
    trigger_glGetBooleanv = true;
    printf("Stub: glGetBooleanv\n");
}
static bool trigger_glGetDoublev = false;
void stub_glGetDoublev() {
    if(trigger_glGetDoublev) return;
    trigger_glGetDoublev = true;
    printf("Stub: glGetDoublev\n");
}
static bool trigger_glGetError = false;
void stub_glGetError() {
    if(trigger_glGetError) return;
    trigger_glGetError = true;
    printf("Stub: glGetError\n");
}
static bool trigger_glGetFloatv = false;
void stub_glGetFloatv() {
    if(trigger_glGetFloatv) return;
    trigger_glGetFloatv = true;
    printf("Stub: glGetFloatv\n");
}
static bool trigger_glGetIntegerv = false;
void stub_glGetIntegerv() {
    if(trigger_glGetIntegerv) return;
    trigger_glGetIntegerv = true;
    printf("Stub: glGetIntegerv\n");
}
static bool trigger_glGetString = false;
void stub_glGetString() {
    if(trigger_glGetString) return;
    trigger_glGetString = true;
    printf("Stub: glGetString\n");
}
static bool trigger_glGetTexImage = false;
void stub_glGetTexImage() {
    if(trigger_glGetTexImage) return;
    trigger_glGetTexImage = true;
    printf("Stub: glGetTexImage\n");
}
static bool trigger_glGetTexParameterfv = false;
void stub_glGetTexParameterfv() {
    if(trigger_glGetTexParameterfv) return;
    trigger_glGetTexParameterfv = true;
    printf("Stub: glGetTexParameterfv\n");
}
static bool trigger_glGetTexParameteriv = false;
void stub_glGetTexParameteriv() {
    if(trigger_glGetTexParameteriv) return;
    trigger_glGetTexParameteriv = true;
    printf("Stub: glGetTexParameteriv\n");
}
static bool trigger_glGetTexLevelParameterfv = false;
void stub_glGetTexLevelParameterfv() {
    if(trigger_glGetTexLevelParameterfv) return;
    trigger_glGetTexLevelParameterfv = true;
    printf("Stub: glGetTexLevelParameterfv\n");
}
static bool trigger_glGetTexLevelParameteriv = false;
void stub_glGetTexLevelParameteriv() {
    if(trigger_glGetTexLevelParameteriv) return;
    trigger_glGetTexLevelParameteriv = true;
    printf("Stub: glGetTexLevelParameteriv\n");
}
static bool trigger_glIsEnabled = false;
void stub_glIsEnabled() {
    if(trigger_glIsEnabled) return;
    trigger_glIsEnabled = true;
    printf("Stub: glIsEnabled\n");
}
static bool trigger_glDepthRange = false;
void stub_glDepthRange() {
    if(trigger_glDepthRange) return;
    trigger_glDepthRange = true;
    printf("Stub: glDepthRange\n");
}
static bool trigger_glViewport = false;
void stub_glViewport() {
    if(trigger_glViewport) return;
    trigger_glViewport = true;
    printf("Stub: glViewport\n");
}
static bool trigger_glDrawArrays = false;
void stub_glDrawArrays() {
    if(trigger_glDrawArrays) return;
    trigger_glDrawArrays = true;
    printf("Stub: glDrawArrays\n");
}
static bool trigger_glDrawElements = false;
void stub_glDrawElements() {
    if(trigger_glDrawElements) return;
    trigger_glDrawElements = true;
    printf("Stub: glDrawElements\n");
}
static bool trigger_glGetPointerv = false;
void stub_glGetPointerv() {
    if(trigger_glGetPointerv) return;
    trigger_glGetPointerv = true;
    printf("Stub: glGetPointerv\n");
}
static bool trigger_glPolygonOffset = false;
void stub_glPolygonOffset() {
    if(trigger_glPolygonOffset) return;
    trigger_glPolygonOffset = true;
    printf("Stub: glPolygonOffset\n");
}
static bool trigger_glCopyTexImage1D = false;
void stub_glCopyTexImage1D() {
    if(trigger_glCopyTexImage1D) return;
    trigger_glCopyTexImage1D = true;
    printf("Stub: glCopyTexImage1D\n");
}
static bool trigger_glCopyTexImage2D = false;
void stub_glCopyTexImage2D() {
    if(trigger_glCopyTexImage2D) return;
    trigger_glCopyTexImage2D = true;
    printf("Stub: glCopyTexImage2D\n");
}
static bool trigger_glCopyTexSubImage1D = false;
void stub_glCopyTexSubImage1D() {
    if(trigger_glCopyTexSubImage1D) return;
    trigger_glCopyTexSubImage1D = true;
    printf("Stub: glCopyTexSubImage1D\n");
}
static bool trigger_glCopyTexSubImage2D = false;
void stub_glCopyTexSubImage2D() {
    if(trigger_glCopyTexSubImage2D) return;
    trigger_glCopyTexSubImage2D = true;
    printf("Stub: glCopyTexSubImage2D\n");
}
static bool trigger_glTexSubImage1D = false;
void stub_glTexSubImage1D() {
    if(trigger_glTexSubImage1D) return;
    trigger_glTexSubImage1D = true;
    printf("Stub: glTexSubImage1D\n");
}
static bool trigger_glTexSubImage2D = false;
void stub_glTexSubImage2D() {
    if(trigger_glTexSubImage2D) return;
    trigger_glTexSubImage2D = true;
    printf("Stub: glTexSubImage2D\n");
}
static bool trigger_glBindTexture = false;
void stub_glBindTexture() {
    if(trigger_glBindTexture) return;
    trigger_glBindTexture = true;
    printf("Stub: glBindTexture\n");
}
static bool trigger_glDeleteTextures = false;
void stub_glDeleteTextures() {
    if(trigger_glDeleteTextures) return;
    trigger_glDeleteTextures = true;
    printf("Stub: glDeleteTextures\n");
}
static bool trigger_glGenTextures = false;
void stub_glGenTextures() {
    if(trigger_glGenTextures) return;
    trigger_glGenTextures = true;
    printf("Stub: glGenTextures\n");
}
static bool trigger_glIsTexture = false;
void stub_glIsTexture() {
    if(trigger_glIsTexture) return;
    trigger_glIsTexture = true;
    printf("Stub: glIsTexture\n");
}
static bool trigger_glDrawRangeElements = false;
void stub_glDrawRangeElements() {
    if(trigger_glDrawRangeElements) return;
    trigger_glDrawRangeElements = true;
    printf("Stub: glDrawRangeElements\n");
}
static bool trigger_glTexImage3D = false;
void stub_glTexImage3D() {
    if(trigger_glTexImage3D) return;
    trigger_glTexImage3D = true;
    printf("Stub: glTexImage3D\n");
}
static bool trigger_glTexSubImage3D = false;
void stub_glTexSubImage3D() {
    if(trigger_glTexSubImage3D) return;
    trigger_glTexSubImage3D = true;
    printf("Stub: glTexSubImage3D\n");
}
static bool trigger_glCopyTexSubImage3D = false;
void stub_glCopyTexSubImage3D() {
    if(trigger_glCopyTexSubImage3D) return;
    trigger_glCopyTexSubImage3D = true;
    printf("Stub: glCopyTexSubImage3D\n");
}
static bool trigger_glActiveTexture = false;
void stub_glActiveTexture() {
    if(trigger_glActiveTexture) return;
    trigger_glActiveTexture = true;
    printf("Stub: glActiveTexture\n");
}
static bool trigger_glSampleCoverage = false;
void stub_glSampleCoverage() {
    if(trigger_glSampleCoverage) return;
    trigger_glSampleCoverage = true;
    printf("Stub: glSampleCoverage\n");
}
static bool trigger_glCompressedTexImage3D = false;
void stub_glCompressedTexImage3D() {
    if(trigger_glCompressedTexImage3D) return;
    trigger_glCompressedTexImage3D = true;
    printf("Stub: glCompressedTexImage3D\n");
}
static bool trigger_glCompressedTexImage2D = false;
void stub_glCompressedTexImage2D() {
    if(trigger_glCompressedTexImage2D) return;
    trigger_glCompressedTexImage2D = true;
    printf("Stub: glCompressedTexImage2D\n");
}
static bool trigger_glCompressedTexImage1D = false;
void stub_glCompressedTexImage1D() {
    if(trigger_glCompressedTexImage1D) return;
    trigger_glCompressedTexImage1D = true;
    printf("Stub: glCompressedTexImage1D\n");
}
static bool trigger_glCompressedTexSubImage3D = false;
void stub_glCompressedTexSubImage3D() {
    if(trigger_glCompressedTexSubImage3D) return;
    trigger_glCompressedTexSubImage3D = true;
    printf("Stub: glCompressedTexSubImage3D\n");
}
static bool trigger_glCompressedTexSubImage2D = false;
void stub_glCompressedTexSubImage2D() {
    if(trigger_glCompressedTexSubImage2D) return;
    trigger_glCompressedTexSubImage2D = true;
    printf("Stub: glCompressedTexSubImage2D\n");
}
static bool trigger_glCompressedTexSubImage1D = false;
void stub_glCompressedTexSubImage1D() {
    if(trigger_glCompressedTexSubImage1D) return;
    trigger_glCompressedTexSubImage1D = true;
    printf("Stub: glCompressedTexSubImage1D\n");
}
static bool trigger_glGetCompressedTexImage = false;
void stub_glGetCompressedTexImage() {
    if(trigger_glGetCompressedTexImage) return;
    trigger_glGetCompressedTexImage = true;
    printf("Stub: glGetCompressedTexImage\n");
}
static bool trigger_glBlendFuncSeparate = false;
void stub_glBlendFuncSeparate() {
    if(trigger_glBlendFuncSeparate) return;
    trigger_glBlendFuncSeparate = true;
    printf("Stub: glBlendFuncSeparate\n");
}
static bool trigger_glMultiDrawArrays = false;
void stub_glMultiDrawArrays() {
    if(trigger_glMultiDrawArrays) return;
    trigger_glMultiDrawArrays = true;
    printf("Stub: glMultiDrawArrays\n");
}
static bool trigger_glMultiDrawElements = false;
void stub_glMultiDrawElements() {
    if(trigger_glMultiDrawElements) return;
    trigger_glMultiDrawElements = true;
    printf("Stub: glMultiDrawElements\n");
}
static bool trigger_glPointParameterf = false;
void stub_glPointParameterf() {
    if(trigger_glPointParameterf) return;
    trigger_glPointParameterf = true;
    printf("Stub: glPointParameterf\n");
}
static bool trigger_glPointParameterfv = false;
void stub_glPointParameterfv() {
    if(trigger_glPointParameterfv) return;
    trigger_glPointParameterfv = true;
    printf("Stub: glPointParameterfv\n");
}
static bool trigger_glPointParameteri = false;
void stub_glPointParameteri() {
    if(trigger_glPointParameteri) return;
    trigger_glPointParameteri = true;
    printf("Stub: glPointParameteri\n");
}
static bool trigger_glPointParameteriv = false;
void stub_glPointParameteriv() {
    if(trigger_glPointParameteriv) return;
    trigger_glPointParameteriv = true;
    printf("Stub: glPointParameteriv\n");
}
static bool trigger_glBlendColor = false;
void stub_glBlendColor() {
    if(trigger_glBlendColor) return;
    trigger_glBlendColor = true;
    printf("Stub: glBlendColor\n");
}
static bool trigger_glBlendEquation = false;
void stub_glBlendEquation() {
    if(trigger_glBlendEquation) return;
    trigger_glBlendEquation = true;
    printf("Stub: glBlendEquation\n");
}
static bool trigger_glGenQueries = false;
void stub_glGenQueries() {
    if(trigger_glGenQueries) return;
    trigger_glGenQueries = true;
    printf("Stub: glGenQueries\n");
}
static bool trigger_glDeleteQueries = false;
void stub_glDeleteQueries() {
    if(trigger_glDeleteQueries) return;
    trigger_glDeleteQueries = true;
    printf("Stub: glDeleteQueries\n");
}
static bool trigger_glIsQuery = false;
void stub_glIsQuery() {
    if(trigger_glIsQuery) return;
    trigger_glIsQuery = true;
    printf("Stub: glIsQuery\n");
}
static bool trigger_glBeginQuery = false;
void stub_glBeginQuery() {
    if(trigger_glBeginQuery) return;
    trigger_glBeginQuery = true;
    printf("Stub: glBeginQuery\n");
}
static bool trigger_glEndQuery = false;
void stub_glEndQuery() {
    if(trigger_glEndQuery) return;
    trigger_glEndQuery = true;
    printf("Stub: glEndQuery\n");
}
static bool trigger_glGetQueryiv = false;
void stub_glGetQueryiv() {
    if(trigger_glGetQueryiv) return;
    trigger_glGetQueryiv = true;
    printf("Stub: glGetQueryiv\n");
}
static bool trigger_glGetQueryObjectiv = false;
void stub_glGetQueryObjectiv() {
    if(trigger_glGetQueryObjectiv) return;
    trigger_glGetQueryObjectiv = true;
    printf("Stub: glGetQueryObjectiv\n");
}
static bool trigger_glGetQueryObjectuiv = false;
void stub_glGetQueryObjectuiv() {
    if(trigger_glGetQueryObjectuiv) return;
    trigger_glGetQueryObjectuiv = true;
    printf("Stub: glGetQueryObjectuiv\n");
}
static bool trigger_glBindBuffer = false;
void stub_glBindBuffer() {
    if(trigger_glBindBuffer) return;
    trigger_glBindBuffer = true;
    printf("Stub: glBindBuffer\n");
}
static bool trigger_glDeleteBuffers = false;
void stub_glDeleteBuffers() {
    if(trigger_glDeleteBuffers) return;
    trigger_glDeleteBuffers = true;
    printf("Stub: glDeleteBuffers\n");
}
static bool trigger_glGenBuffers = false;
void stub_glGenBuffers() {
    if(trigger_glGenBuffers) return;
    trigger_glGenBuffers = true;
    printf("Stub: glGenBuffers\n");
}
static bool trigger_glIsBuffer = false;
void stub_glIsBuffer() {
    if(trigger_glIsBuffer) return;
    trigger_glIsBuffer = true;
    printf("Stub: glIsBuffer\n");
}
static bool trigger_glBufferData = false;
void stub_glBufferData() {
    if(trigger_glBufferData) return;
    trigger_glBufferData = true;
    printf("Stub: glBufferData\n");
}
static bool trigger_glBufferSubData = false;
void stub_glBufferSubData() {
    if(trigger_glBufferSubData) return;
    trigger_glBufferSubData = true;
    printf("Stub: glBufferSubData\n");
}
static bool trigger_glGetBufferSubData = false;
void stub_glGetBufferSubData() {
    if(trigger_glGetBufferSubData) return;
    trigger_glGetBufferSubData = true;
    printf("Stub: glGetBufferSubData\n");
}
static bool trigger_glMapBuffer = false;
void stub_glMapBuffer() {
    if(trigger_glMapBuffer) return;
    trigger_glMapBuffer = true;
    printf("Stub: glMapBuffer\n");
}
static bool trigger_glUnmapBuffer = false;
void stub_glUnmapBuffer() {
    if(trigger_glUnmapBuffer) return;
    trigger_glUnmapBuffer = true;
    printf("Stub: glUnmapBuffer\n");
}
static bool trigger_glGetBufferParameteriv = false;
void stub_glGetBufferParameteriv() {
    if(trigger_glGetBufferParameteriv) return;
    trigger_glGetBufferParameteriv = true;
    printf("Stub: glGetBufferParameteriv\n");
}
static bool trigger_glGetBufferPointerv = false;
void stub_glGetBufferPointerv() {
    if(trigger_glGetBufferPointerv) return;
    trigger_glGetBufferPointerv = true;
    printf("Stub: glGetBufferPointerv\n");
}
static bool trigger_glBlendEquationSeparate = false;
void stub_glBlendEquationSeparate() {
    if(trigger_glBlendEquationSeparate) return;
    trigger_glBlendEquationSeparate = true;
    printf("Stub: glBlendEquationSeparate\n");
}
static bool trigger_glDrawBuffers = false;
void stub_glDrawBuffers() {
    if(trigger_glDrawBuffers) return;
    trigger_glDrawBuffers = true;
    printf("Stub: glDrawBuffers\n");
}
static bool trigger_glStencilOpSeparate = false;
void stub_glStencilOpSeparate() {
    if(trigger_glStencilOpSeparate) return;
    trigger_glStencilOpSeparate = true;
    printf("Stub: glStencilOpSeparate\n");
}
static bool trigger_glStencilFuncSeparate = false;
void stub_glStencilFuncSeparate() {
    if(trigger_glStencilFuncSeparate) return;
    trigger_glStencilFuncSeparate = true;
    printf("Stub: glStencilFuncSeparate\n");
}
static bool trigger_glStencilMaskSeparate = false;
void stub_glStencilMaskSeparate() {
    if(trigger_glStencilMaskSeparate) return;
    trigger_glStencilMaskSeparate = true;
    printf("Stub: glStencilMaskSeparate\n");
}
static bool trigger_glAttachShader = false;
void stub_glAttachShader() {
    if(trigger_glAttachShader) return;
    trigger_glAttachShader = true;
    printf("Stub: glAttachShader\n");
}
static bool trigger_glBindAttribLocation = false;
void stub_glBindAttribLocation() {
    if(trigger_glBindAttribLocation) return;
    trigger_glBindAttribLocation = true;
    printf("Stub: glBindAttribLocation\n");
}
static bool trigger_glCompileShader = false;
void stub_glCompileShader() {
    if(trigger_glCompileShader) return;
    trigger_glCompileShader = true;
    printf("Stub: glCompileShader\n");
}
static bool trigger_glCreateProgram = false;
void stub_glCreateProgram() {
    if(trigger_glCreateProgram) return;
    trigger_glCreateProgram = true;
    printf("Stub: glCreateProgram\n");
}
static bool trigger_glCreateShader = false;
void stub_glCreateShader() {
    if(trigger_glCreateShader) return;
    trigger_glCreateShader = true;
    printf("Stub: glCreateShader\n");
}
static bool trigger_glDeleteProgram = false;
void stub_glDeleteProgram() {
    if(trigger_glDeleteProgram) return;
    trigger_glDeleteProgram = true;
    printf("Stub: glDeleteProgram\n");
}
static bool trigger_glDeleteShader = false;
void stub_glDeleteShader() {
    if(trigger_glDeleteShader) return;
    trigger_glDeleteShader = true;
    printf("Stub: glDeleteShader\n");
}
static bool trigger_glDetachShader = false;
void stub_glDetachShader() {
    if(trigger_glDetachShader) return;
    trigger_glDetachShader = true;
    printf("Stub: glDetachShader\n");
}
static bool trigger_glDisableVertexAttribArray = false;
void stub_glDisableVertexAttribArray() {
    if(trigger_glDisableVertexAttribArray) return;
    trigger_glDisableVertexAttribArray = true;
    printf("Stub: glDisableVertexAttribArray\n");
}
static bool trigger_glEnableVertexAttribArray = false;
void stub_glEnableVertexAttribArray() {
    if(trigger_glEnableVertexAttribArray) return;
    trigger_glEnableVertexAttribArray = true;
    printf("Stub: glEnableVertexAttribArray\n");
}
static bool trigger_glGetActiveAttrib = false;
void stub_glGetActiveAttrib() {
    if(trigger_glGetActiveAttrib) return;
    trigger_glGetActiveAttrib = true;
    printf("Stub: glGetActiveAttrib\n");
}
static bool trigger_glGetActiveUniform = false;
void stub_glGetActiveUniform() {
    if(trigger_glGetActiveUniform) return;
    trigger_glGetActiveUniform = true;
    printf("Stub: glGetActiveUniform\n");
}
static bool trigger_glGetAttachedShaders = false;
void stub_glGetAttachedShaders() {
    if(trigger_glGetAttachedShaders) return;
    trigger_glGetAttachedShaders = true;
    printf("Stub: glGetAttachedShaders\n");
}
static bool trigger_glGetAttribLocation = false;
void stub_glGetAttribLocation() {
    if(trigger_glGetAttribLocation) return;
    trigger_glGetAttribLocation = true;
    printf("Stub: glGetAttribLocation\n");
}
static bool trigger_glGetProgramiv = false;
void stub_glGetProgramiv() {
    if(trigger_glGetProgramiv) return;
    trigger_glGetProgramiv = true;
    printf("Stub: glGetProgramiv\n");
}
static bool trigger_glGetProgramInfoLog = false;
void stub_glGetProgramInfoLog() {
    if(trigger_glGetProgramInfoLog) return;
    trigger_glGetProgramInfoLog = true;
    printf("Stub: glGetProgramInfoLog\n");
}
static bool trigger_glGetShaderiv = false;
void stub_glGetShaderiv() {
    if(trigger_glGetShaderiv) return;
    trigger_glGetShaderiv = true;
    printf("Stub: glGetShaderiv\n");
}
static bool trigger_glGetShaderInfoLog = false;
void stub_glGetShaderInfoLog() {
    if(trigger_glGetShaderInfoLog) return;
    trigger_glGetShaderInfoLog = true;
    printf("Stub: glGetShaderInfoLog\n");
}
static bool trigger_glGetShaderSource = false;
void stub_glGetShaderSource() {
    if(trigger_glGetShaderSource) return;
    trigger_glGetShaderSource = true;
    printf("Stub: glGetShaderSource\n");
}
static bool trigger_glGetUniformLocation = false;
void stub_glGetUniformLocation() {
    if(trigger_glGetUniformLocation) return;
    trigger_glGetUniformLocation = true;
    printf("Stub: glGetUniformLocation\n");
}
static bool trigger_glGetUniformfv = false;
void stub_glGetUniformfv() {
    if(trigger_glGetUniformfv) return;
    trigger_glGetUniformfv = true;
    printf("Stub: glGetUniformfv\n");
}
static bool trigger_glGetUniformiv = false;
void stub_glGetUniformiv() {
    if(trigger_glGetUniformiv) return;
    trigger_glGetUniformiv = true;
    printf("Stub: glGetUniformiv\n");
}
static bool trigger_glGetVertexAttribdv = false;
void stub_glGetVertexAttribdv() {
    if(trigger_glGetVertexAttribdv) return;
    trigger_glGetVertexAttribdv = true;
    printf("Stub: glGetVertexAttribdv\n");
}
static bool trigger_glGetVertexAttribfv = false;
void stub_glGetVertexAttribfv() {
    if(trigger_glGetVertexAttribfv) return;
    trigger_glGetVertexAttribfv = true;
    printf("Stub: glGetVertexAttribfv\n");
}
static bool trigger_glGetVertexAttribiv = false;
void stub_glGetVertexAttribiv() {
    if(trigger_glGetVertexAttribiv) return;
    trigger_glGetVertexAttribiv = true;
    printf("Stub: glGetVertexAttribiv\n");
}
static bool trigger_glGetVertexAttribPointerv = false;
void stub_glGetVertexAttribPointerv() {
    if(trigger_glGetVertexAttribPointerv) return;
    trigger_glGetVertexAttribPointerv = true;
    printf("Stub: glGetVertexAttribPointerv\n");
}
static bool trigger_glIsProgram = false;
void stub_glIsProgram() {
    if(trigger_glIsProgram) return;
    trigger_glIsProgram = true;
    printf("Stub: glIsProgram\n");
}
static bool trigger_glIsShader = false;
void stub_glIsShader() {
    if(trigger_glIsShader) return;
    trigger_glIsShader = true;
    printf("Stub: glIsShader\n");
}
static bool trigger_glLinkProgram = false;
void stub_glLinkProgram() {
    if(trigger_glLinkProgram) return;
    trigger_glLinkProgram = true;
    printf("Stub: glLinkProgram\n");
}
static bool trigger_glShaderSource = false;
void stub_glShaderSource() {
    if(trigger_glShaderSource) return;
    trigger_glShaderSource = true;
    printf("Stub: glShaderSource\n");
}
static bool trigger_glUseProgram = false;
void stub_glUseProgram() {
    if(trigger_glUseProgram) return;
    trigger_glUseProgram = true;
    printf("Stub: glUseProgram\n");
}
static bool trigger_glUniform1f = false;
void stub_glUniform1f() {
    if(trigger_glUniform1f) return;
    trigger_glUniform1f = true;
    printf("Stub: glUniform1f\n");
}
static bool trigger_glUniform2f = false;
void stub_glUniform2f() {
    if(trigger_glUniform2f) return;
    trigger_glUniform2f = true;
    printf("Stub: glUniform2f\n");
}
static bool trigger_glUniform3f = false;
void stub_glUniform3f() {
    if(trigger_glUniform3f) return;
    trigger_glUniform3f = true;
    printf("Stub: glUniform3f\n");
}
static bool trigger_glUniform4f = false;
void stub_glUniform4f() {
    if(trigger_glUniform4f) return;
    trigger_glUniform4f = true;
    printf("Stub: glUniform4f\n");
}
static bool trigger_glUniform1i = false;
void stub_glUniform1i() {
    if(trigger_glUniform1i) return;
    trigger_glUniform1i = true;
    printf("Stub: glUniform1i\n");
}
static bool trigger_glUniform2i = false;
void stub_glUniform2i() {
    if(trigger_glUniform2i) return;
    trigger_glUniform2i = true;
    printf("Stub: glUniform2i\n");
}
static bool trigger_glUniform3i = false;
void stub_glUniform3i() {
    if(trigger_glUniform3i) return;
    trigger_glUniform3i = true;
    printf("Stub: glUniform3i\n");
}
static bool trigger_glUniform4i = false;
void stub_glUniform4i() {
    if(trigger_glUniform4i) return;
    trigger_glUniform4i = true;
    printf("Stub: glUniform4i\n");
}
static bool trigger_glUniform1fv = false;
void stub_glUniform1fv() {
    if(trigger_glUniform1fv) return;
    trigger_glUniform1fv = true;
    printf("Stub: glUniform1fv\n");
}
static bool trigger_glUniform2fv = false;
void stub_glUniform2fv() {
    if(trigger_glUniform2fv) return;
    trigger_glUniform2fv = true;
    printf("Stub: glUniform2fv\n");
}
static bool trigger_glUniform3fv = false;
void stub_glUniform3fv() {
    if(trigger_glUniform3fv) return;
    trigger_glUniform3fv = true;
    printf("Stub: glUniform3fv\n");
}
static bool trigger_glUniform4fv = false;
void stub_glUniform4fv() {
    if(trigger_glUniform4fv) return;
    trigger_glUniform4fv = true;
    printf("Stub: glUniform4fv\n");
}
static bool trigger_glUniform1iv = false;
void stub_glUniform1iv() {
    if(trigger_glUniform1iv) return;
    trigger_glUniform1iv = true;
    printf("Stub: glUniform1iv\n");
}
static bool trigger_glUniform2iv = false;
void stub_glUniform2iv() {
    if(trigger_glUniform2iv) return;
    trigger_glUniform2iv = true;
    printf("Stub: glUniform2iv\n");
}
static bool trigger_glUniform3iv = false;
void stub_glUniform3iv() {
    if(trigger_glUniform3iv) return;
    trigger_glUniform3iv = true;
    printf("Stub: glUniform3iv\n");
}
static bool trigger_glUniform4iv = false;
void stub_glUniform4iv() {
    if(trigger_glUniform4iv) return;
    trigger_glUniform4iv = true;
    printf("Stub: glUniform4iv\n");
}
static bool trigger_glUniformMatrix2fv = false;
void stub_glUniformMatrix2fv() {
    if(trigger_glUniformMatrix2fv) return;
    trigger_glUniformMatrix2fv = true;
    printf("Stub: glUniformMatrix2fv\n");
}
static bool trigger_glUniformMatrix3fv = false;
void stub_glUniformMatrix3fv() {
    if(trigger_glUniformMatrix3fv) return;
    trigger_glUniformMatrix3fv = true;
    printf("Stub: glUniformMatrix3fv\n");
}
static bool trigger_glUniformMatrix4fv = false;
void stub_glUniformMatrix4fv() {
    if(trigger_glUniformMatrix4fv) return;
    trigger_glUniformMatrix4fv = true;
    printf("Stub: glUniformMatrix4fv\n");
}
static bool trigger_glValidateProgram = false;
void stub_glValidateProgram() {
    if(trigger_glValidateProgram) return;
    trigger_glValidateProgram = true;
    printf("Stub: glValidateProgram\n");
}
static bool trigger_glVertexAttrib1d = false;
void stub_glVertexAttrib1d() {
    if(trigger_glVertexAttrib1d) return;
    trigger_glVertexAttrib1d = true;
    printf("Stub: glVertexAttrib1d\n");
}
static bool trigger_glVertexAttrib1dv = false;
void stub_glVertexAttrib1dv() {
    if(trigger_glVertexAttrib1dv) return;
    trigger_glVertexAttrib1dv = true;
    printf("Stub: glVertexAttrib1dv\n");
}
static bool trigger_glVertexAttrib1f = false;
void stub_glVertexAttrib1f() {
    if(trigger_glVertexAttrib1f) return;
    trigger_glVertexAttrib1f = true;
    printf("Stub: glVertexAttrib1f\n");
}
static bool trigger_glVertexAttrib1fv = false;
void stub_glVertexAttrib1fv() {
    if(trigger_glVertexAttrib1fv) return;
    trigger_glVertexAttrib1fv = true;
    printf("Stub: glVertexAttrib1fv\n");
}
static bool trigger_glVertexAttrib1s = false;
void stub_glVertexAttrib1s() {
    if(trigger_glVertexAttrib1s) return;
    trigger_glVertexAttrib1s = true;
    printf("Stub: glVertexAttrib1s\n");
}
static bool trigger_glVertexAttrib1sv = false;
void stub_glVertexAttrib1sv() {
    if(trigger_glVertexAttrib1sv) return;
    trigger_glVertexAttrib1sv = true;
    printf("Stub: glVertexAttrib1sv\n");
}
static bool trigger_glVertexAttrib2d = false;
void stub_glVertexAttrib2d() {
    if(trigger_glVertexAttrib2d) return;
    trigger_glVertexAttrib2d = true;
    printf("Stub: glVertexAttrib2d\n");
}
static bool trigger_glVertexAttrib2dv = false;
void stub_glVertexAttrib2dv() {
    if(trigger_glVertexAttrib2dv) return;
    trigger_glVertexAttrib2dv = true;
    printf("Stub: glVertexAttrib2dv\n");
}
static bool trigger_glVertexAttrib2f = false;
void stub_glVertexAttrib2f() {
    if(trigger_glVertexAttrib2f) return;
    trigger_glVertexAttrib2f = true;
    printf("Stub: glVertexAttrib2f\n");
}
static bool trigger_glVertexAttrib2fv = false;
void stub_glVertexAttrib2fv() {
    if(trigger_glVertexAttrib2fv) return;
    trigger_glVertexAttrib2fv = true;
    printf("Stub: glVertexAttrib2fv\n");
}
static bool trigger_glVertexAttrib2s = false;
void stub_glVertexAttrib2s() {
    if(trigger_glVertexAttrib2s) return;
    trigger_glVertexAttrib2s = true;
    printf("Stub: glVertexAttrib2s\n");
}
static bool trigger_glVertexAttrib2sv = false;
void stub_glVertexAttrib2sv() {
    if(trigger_glVertexAttrib2sv) return;
    trigger_glVertexAttrib2sv = true;
    printf("Stub: glVertexAttrib2sv\n");
}
static bool trigger_glVertexAttrib3d = false;
void stub_glVertexAttrib3d() {
    if(trigger_glVertexAttrib3d) return;
    trigger_glVertexAttrib3d = true;
    printf("Stub: glVertexAttrib3d\n");
}
static bool trigger_glVertexAttrib3dv = false;
void stub_glVertexAttrib3dv() {
    if(trigger_glVertexAttrib3dv) return;
    trigger_glVertexAttrib3dv = true;
    printf("Stub: glVertexAttrib3dv\n");
}
static bool trigger_glVertexAttrib3f = false;
void stub_glVertexAttrib3f() {
    if(trigger_glVertexAttrib3f) return;
    trigger_glVertexAttrib3f = true;
    printf("Stub: glVertexAttrib3f\n");
}
static bool trigger_glVertexAttrib3fv = false;
void stub_glVertexAttrib3fv() {
    if(trigger_glVertexAttrib3fv) return;
    trigger_glVertexAttrib3fv = true;
    printf("Stub: glVertexAttrib3fv\n");
}
static bool trigger_glVertexAttrib3s = false;
void stub_glVertexAttrib3s() {
    if(trigger_glVertexAttrib3s) return;
    trigger_glVertexAttrib3s = true;
    printf("Stub: glVertexAttrib3s\n");
}
static bool trigger_glVertexAttrib3sv = false;
void stub_glVertexAttrib3sv() {
    if(trigger_glVertexAttrib3sv) return;
    trigger_glVertexAttrib3sv = true;
    printf("Stub: glVertexAttrib3sv\n");
}
static bool trigger_glVertexAttrib4Nbv = false;
void stub_glVertexAttrib4Nbv() {
    if(trigger_glVertexAttrib4Nbv) return;
    trigger_glVertexAttrib4Nbv = true;
    printf("Stub: glVertexAttrib4Nbv\n");
}
static bool trigger_glVertexAttrib4Niv = false;
void stub_glVertexAttrib4Niv() {
    if(trigger_glVertexAttrib4Niv) return;
    trigger_glVertexAttrib4Niv = true;
    printf("Stub: glVertexAttrib4Niv\n");
}
static bool trigger_glVertexAttrib4Nsv = false;
void stub_glVertexAttrib4Nsv() {
    if(trigger_glVertexAttrib4Nsv) return;
    trigger_glVertexAttrib4Nsv = true;
    printf("Stub: glVertexAttrib4Nsv\n");
}
static bool trigger_glVertexAttrib4Nub = false;
void stub_glVertexAttrib4Nub() {
    if(trigger_glVertexAttrib4Nub) return;
    trigger_glVertexAttrib4Nub = true;
    printf("Stub: glVertexAttrib4Nub\n");
}
static bool trigger_glVertexAttrib4Nubv = false;
void stub_glVertexAttrib4Nubv() {
    if(trigger_glVertexAttrib4Nubv) return;
    trigger_glVertexAttrib4Nubv = true;
    printf("Stub: glVertexAttrib4Nubv\n");
}
static bool trigger_glVertexAttrib4Nuiv = false;
void stub_glVertexAttrib4Nuiv() {
    if(trigger_glVertexAttrib4Nuiv) return;
    trigger_glVertexAttrib4Nuiv = true;
    printf("Stub: glVertexAttrib4Nuiv\n");
}
static bool trigger_glVertexAttrib4Nusv = false;
void stub_glVertexAttrib4Nusv() {
    if(trigger_glVertexAttrib4Nusv) return;
    trigger_glVertexAttrib4Nusv = true;
    printf("Stub: glVertexAttrib4Nusv\n");
}
static bool trigger_glVertexAttrib4bv = false;
void stub_glVertexAttrib4bv() {
    if(trigger_glVertexAttrib4bv) return;
    trigger_glVertexAttrib4bv = true;
    printf("Stub: glVertexAttrib4bv\n");
}
static bool trigger_glVertexAttrib4d = false;
void stub_glVertexAttrib4d() {
    if(trigger_glVertexAttrib4d) return;
    trigger_glVertexAttrib4d = true;
    printf("Stub: glVertexAttrib4d\n");
}
static bool trigger_glVertexAttrib4dv = false;
void stub_glVertexAttrib4dv() {
    if(trigger_glVertexAttrib4dv) return;
    trigger_glVertexAttrib4dv = true;
    printf("Stub: glVertexAttrib4dv\n");
}
static bool trigger_glVertexAttrib4f = false;
void stub_glVertexAttrib4f() {
    if(trigger_glVertexAttrib4f) return;
    trigger_glVertexAttrib4f = true;
    printf("Stub: glVertexAttrib4f\n");
}
static bool trigger_glVertexAttrib4fv = false;
void stub_glVertexAttrib4fv() {
    if(trigger_glVertexAttrib4fv) return;
    trigger_glVertexAttrib4fv = true;
    printf("Stub: glVertexAttrib4fv\n");
}
static bool trigger_glVertexAttrib4iv = false;
void stub_glVertexAttrib4iv() {
    if(trigger_glVertexAttrib4iv) return;
    trigger_glVertexAttrib4iv = true;
    printf("Stub: glVertexAttrib4iv\n");
}
static bool trigger_glVertexAttrib4s = false;
void stub_glVertexAttrib4s() {
    if(trigger_glVertexAttrib4s) return;
    trigger_glVertexAttrib4s = true;
    printf("Stub: glVertexAttrib4s\n");
}
static bool trigger_glVertexAttrib4sv = false;
void stub_glVertexAttrib4sv() {
    if(trigger_glVertexAttrib4sv) return;
    trigger_glVertexAttrib4sv = true;
    printf("Stub: glVertexAttrib4sv\n");
}
static bool trigger_glVertexAttrib4ubv = false;
void stub_glVertexAttrib4ubv() {
    if(trigger_glVertexAttrib4ubv) return;
    trigger_glVertexAttrib4ubv = true;
    printf("Stub: glVertexAttrib4ubv\n");
}
static bool trigger_glVertexAttrib4uiv = false;
void stub_glVertexAttrib4uiv() {
    if(trigger_glVertexAttrib4uiv) return;
    trigger_glVertexAttrib4uiv = true;
    printf("Stub: glVertexAttrib4uiv\n");
}
static bool trigger_glVertexAttrib4usv = false;
void stub_glVertexAttrib4usv() {
    if(trigger_glVertexAttrib4usv) return;
    trigger_glVertexAttrib4usv = true;
    printf("Stub: glVertexAttrib4usv\n");
}
static bool trigger_glVertexAttribPointer = false;
void stub_glVertexAttribPointer() {
    if(trigger_glVertexAttribPointer) return;
    trigger_glVertexAttribPointer = true;
    printf("Stub: glVertexAttribPointer\n");
}
static bool trigger_glUniformMatrix2x3fv = false;
void stub_glUniformMatrix2x3fv() {
    if(trigger_glUniformMatrix2x3fv) return;
    trigger_glUniformMatrix2x3fv = true;
    printf("Stub: glUniformMatrix2x3fv\n");
}
static bool trigger_glUniformMatrix3x2fv = false;
void stub_glUniformMatrix3x2fv() {
    if(trigger_glUniformMatrix3x2fv) return;
    trigger_glUniformMatrix3x2fv = true;
    printf("Stub: glUniformMatrix3x2fv\n");
}
static bool trigger_glUniformMatrix2x4fv = false;
void stub_glUniformMatrix2x4fv() {
    if(trigger_glUniformMatrix2x4fv) return;
    trigger_glUniformMatrix2x4fv = true;
    printf("Stub: glUniformMatrix2x4fv\n");
}
static bool trigger_glUniformMatrix4x2fv = false;
void stub_glUniformMatrix4x2fv() {
    if(trigger_glUniformMatrix4x2fv) return;
    trigger_glUniformMatrix4x2fv = true;
    printf("Stub: glUniformMatrix4x2fv\n");
}
static bool trigger_glUniformMatrix3x4fv = false;
void stub_glUniformMatrix3x4fv() {
    if(trigger_glUniformMatrix3x4fv) return;
    trigger_glUniformMatrix3x4fv = true;
    printf("Stub: glUniformMatrix3x4fv\n");
}
static bool trigger_glUniformMatrix4x3fv = false;
void stub_glUniformMatrix4x3fv() {
    if(trigger_glUniformMatrix4x3fv) return;
    trigger_glUniformMatrix4x3fv = true;
    printf("Stub: glUniformMatrix4x3fv\n");
}
static bool trigger_glColorMaski = false;
void stub_glColorMaski() {
    if(trigger_glColorMaski) return;
    trigger_glColorMaski = true;
    printf("Stub: glColorMaski\n");
}
static bool trigger_glGetBooleani_v = false;
void stub_glGetBooleani_v() {
    if(trigger_glGetBooleani_v) return;
    trigger_glGetBooleani_v = true;
    printf("Stub: glGetBooleani_v\n");
}
static bool trigger_glGetIntegeri_v = false;
void stub_glGetIntegeri_v() {
    if(trigger_glGetIntegeri_v) return;
    trigger_glGetIntegeri_v = true;
    printf("Stub: glGetIntegeri_v\n");
}
static bool trigger_glEnablei = false;
void stub_glEnablei() {
    if(trigger_glEnablei) return;
    trigger_glEnablei = true;
    printf("Stub: glEnablei\n");
}
static bool trigger_glDisablei = false;
void stub_glDisablei() {
    if(trigger_glDisablei) return;
    trigger_glDisablei = true;
    printf("Stub: glDisablei\n");
}
static bool trigger_glIsEnabledi = false;
void stub_glIsEnabledi() {
    if(trigger_glIsEnabledi) return;
    trigger_glIsEnabledi = true;
    printf("Stub: glIsEnabledi\n");
}
static bool trigger_glBeginTransformFeedback = false;
void stub_glBeginTransformFeedback() {
    if(trigger_glBeginTransformFeedback) return;
    trigger_glBeginTransformFeedback = true;
    printf("Stub: glBeginTransformFeedback\n");
}
static bool trigger_glEndTransformFeedback = false;
void stub_glEndTransformFeedback() {
    if(trigger_glEndTransformFeedback) return;
    trigger_glEndTransformFeedback = true;
    printf("Stub: glEndTransformFeedback\n");
}
static bool trigger_glBindBufferRange = false;
void stub_glBindBufferRange() {
    if(trigger_glBindBufferRange) return;
    trigger_glBindBufferRange = true;
    printf("Stub: glBindBufferRange\n");
}
static bool trigger_glBindBufferBase = false;
void stub_glBindBufferBase() {
    if(trigger_glBindBufferBase) return;
    trigger_glBindBufferBase = true;
    printf("Stub: glBindBufferBase\n");
}
static bool trigger_glTransformFeedbackVaryings = false;
void stub_glTransformFeedbackVaryings() {
    if(trigger_glTransformFeedbackVaryings) return;
    trigger_glTransformFeedbackVaryings = true;
    printf("Stub: glTransformFeedbackVaryings\n");
}
static bool trigger_glGetTransformFeedbackVarying = false;
void stub_glGetTransformFeedbackVarying() {
    if(trigger_glGetTransformFeedbackVarying) return;
    trigger_glGetTransformFeedbackVarying = true;
    printf("Stub: glGetTransformFeedbackVarying\n");
}
static bool trigger_glClampColor = false;
void stub_glClampColor() {
    if(trigger_glClampColor) return;
    trigger_glClampColor = true;
    printf("Stub: glClampColor\n");
}
static bool trigger_glBeginConditionalRender = false;
void stub_glBeginConditionalRender() {
    if(trigger_glBeginConditionalRender) return;
    trigger_glBeginConditionalRender = true;
    printf("Stub: glBeginConditionalRender\n");
}
static bool trigger_glEndConditionalRender = false;
void stub_glEndConditionalRender() {
    if(trigger_glEndConditionalRender) return;
    trigger_glEndConditionalRender = true;
    printf("Stub: glEndConditionalRender\n");
}
static bool trigger_glVertexAttribIPointer = false;
void stub_glVertexAttribIPointer() {
    if(trigger_glVertexAttribIPointer) return;
    trigger_glVertexAttribIPointer = true;
    printf("Stub: glVertexAttribIPointer\n");
}
static bool trigger_glGetVertexAttribIiv = false;
void stub_glGetVertexAttribIiv() {
    if(trigger_glGetVertexAttribIiv) return;
    trigger_glGetVertexAttribIiv = true;
    printf("Stub: glGetVertexAttribIiv\n");
}
static bool trigger_glGetVertexAttribIuiv = false;
void stub_glGetVertexAttribIuiv() {
    if(trigger_glGetVertexAttribIuiv) return;
    trigger_glGetVertexAttribIuiv = true;
    printf("Stub: glGetVertexAttribIuiv\n");
}
static bool trigger_glVertexAttribI1i = false;
void stub_glVertexAttribI1i() {
    if(trigger_glVertexAttribI1i) return;
    trigger_glVertexAttribI1i = true;
    printf("Stub: glVertexAttribI1i\n");
}
static bool trigger_glVertexAttribI2i = false;
void stub_glVertexAttribI2i() {
    if(trigger_glVertexAttribI2i) return;
    trigger_glVertexAttribI2i = true;
    printf("Stub: glVertexAttribI2i\n");
}
static bool trigger_glVertexAttribI3i = false;
void stub_glVertexAttribI3i() {
    if(trigger_glVertexAttribI3i) return;
    trigger_glVertexAttribI3i = true;
    printf("Stub: glVertexAttribI3i\n");
}
static bool trigger_glVertexAttribI4i = false;
void stub_glVertexAttribI4i() {
    if(trigger_glVertexAttribI4i) return;
    trigger_glVertexAttribI4i = true;
    printf("Stub: glVertexAttribI4i\n");
}
static bool trigger_glVertexAttribI1ui = false;
void stub_glVertexAttribI1ui() {
    if(trigger_glVertexAttribI1ui) return;
    trigger_glVertexAttribI1ui = true;
    printf("Stub: glVertexAttribI1ui\n");
}
static bool trigger_glVertexAttribI2ui = false;
void stub_glVertexAttribI2ui() {
    if(trigger_glVertexAttribI2ui) return;
    trigger_glVertexAttribI2ui = true;
    printf("Stub: glVertexAttribI2ui\n");
}
static bool trigger_glVertexAttribI3ui = false;
void stub_glVertexAttribI3ui() {
    if(trigger_glVertexAttribI3ui) return;
    trigger_glVertexAttribI3ui = true;
    printf("Stub: glVertexAttribI3ui\n");
}
static bool trigger_glVertexAttribI4ui = false;
void stub_glVertexAttribI4ui() {
    if(trigger_glVertexAttribI4ui) return;
    trigger_glVertexAttribI4ui = true;
    printf("Stub: glVertexAttribI4ui\n");
}
static bool trigger_glVertexAttribI1iv = false;
void stub_glVertexAttribI1iv() {
    if(trigger_glVertexAttribI1iv) return;
    trigger_glVertexAttribI1iv = true;
    printf("Stub: glVertexAttribI1iv\n");
}
static bool trigger_glVertexAttribI2iv = false;
void stub_glVertexAttribI2iv() {
    if(trigger_glVertexAttribI2iv) return;
    trigger_glVertexAttribI2iv = true;
    printf("Stub: glVertexAttribI2iv\n");
}
static bool trigger_glVertexAttribI3iv = false;
void stub_glVertexAttribI3iv() {
    if(trigger_glVertexAttribI3iv) return;
    trigger_glVertexAttribI3iv = true;
    printf("Stub: glVertexAttribI3iv\n");
}
static bool trigger_glVertexAttribI4iv = false;
void stub_glVertexAttribI4iv() {
    if(trigger_glVertexAttribI4iv) return;
    trigger_glVertexAttribI4iv = true;
    printf("Stub: glVertexAttribI4iv\n");
}
static bool trigger_glVertexAttribI1uiv = false;
void stub_glVertexAttribI1uiv() {
    if(trigger_glVertexAttribI1uiv) return;
    trigger_glVertexAttribI1uiv = true;
    printf("Stub: glVertexAttribI1uiv\n");
}
static bool trigger_glVertexAttribI2uiv = false;
void stub_glVertexAttribI2uiv() {
    if(trigger_glVertexAttribI2uiv) return;
    trigger_glVertexAttribI2uiv = true;
    printf("Stub: glVertexAttribI2uiv\n");
}
static bool trigger_glVertexAttribI3uiv = false;
void stub_glVertexAttribI3uiv() {
    if(trigger_glVertexAttribI3uiv) return;
    trigger_glVertexAttribI3uiv = true;
    printf("Stub: glVertexAttribI3uiv\n");
}
static bool trigger_glVertexAttribI4uiv = false;
void stub_glVertexAttribI4uiv() {
    if(trigger_glVertexAttribI4uiv) return;
    trigger_glVertexAttribI4uiv = true;
    printf("Stub: glVertexAttribI4uiv\n");
}
static bool trigger_glVertexAttribI4bv = false;
void stub_glVertexAttribI4bv() {
    if(trigger_glVertexAttribI4bv) return;
    trigger_glVertexAttribI4bv = true;
    printf("Stub: glVertexAttribI4bv\n");
}
static bool trigger_glVertexAttribI4sv = false;
void stub_glVertexAttribI4sv() {
    if(trigger_glVertexAttribI4sv) return;
    trigger_glVertexAttribI4sv = true;
    printf("Stub: glVertexAttribI4sv\n");
}
static bool trigger_glVertexAttribI4ubv = false;
void stub_glVertexAttribI4ubv() {
    if(trigger_glVertexAttribI4ubv) return;
    trigger_glVertexAttribI4ubv = true;
    printf("Stub: glVertexAttribI4ubv\n");
}
static bool trigger_glVertexAttribI4usv = false;
void stub_glVertexAttribI4usv() {
    if(trigger_glVertexAttribI4usv) return;
    trigger_glVertexAttribI4usv = true;
    printf("Stub: glVertexAttribI4usv\n");
}
static bool trigger_glGetUniformuiv = false;
void stub_glGetUniformuiv() {
    if(trigger_glGetUniformuiv) return;
    trigger_glGetUniformuiv = true;
    printf("Stub: glGetUniformuiv\n");
}
static bool trigger_glBindFragDataLocation = false;
void stub_glBindFragDataLocation() {
    if(trigger_glBindFragDataLocation) return;
    trigger_glBindFragDataLocation = true;
    printf("Stub: glBindFragDataLocation\n");
}
static bool trigger_glGetFragDataLocation = false;
void stub_glGetFragDataLocation() {
    if(trigger_glGetFragDataLocation) return;
    trigger_glGetFragDataLocation = true;
    printf("Stub: glGetFragDataLocation\n");
}
static bool trigger_glUniform1ui = false;
void stub_glUniform1ui() {
    if(trigger_glUniform1ui) return;
    trigger_glUniform1ui = true;
    printf("Stub: glUniform1ui\n");
}
static bool trigger_glUniform2ui = false;
void stub_glUniform2ui() {
    if(trigger_glUniform2ui) return;
    trigger_glUniform2ui = true;
    printf("Stub: glUniform2ui\n");
}
static bool trigger_glUniform3ui = false;
void stub_glUniform3ui() {
    if(trigger_glUniform3ui) return;
    trigger_glUniform3ui = true;
    printf("Stub: glUniform3ui\n");
}
static bool trigger_glUniform4ui = false;
void stub_glUniform4ui() {
    if(trigger_glUniform4ui) return;
    trigger_glUniform4ui = true;
    printf("Stub: glUniform4ui\n");
}
static bool trigger_glUniform1uiv = false;
void stub_glUniform1uiv() {
    if(trigger_glUniform1uiv) return;
    trigger_glUniform1uiv = true;
    printf("Stub: glUniform1uiv\n");
}
static bool trigger_glUniform2uiv = false;
void stub_glUniform2uiv() {
    if(trigger_glUniform2uiv) return;
    trigger_glUniform2uiv = true;
    printf("Stub: glUniform2uiv\n");
}
static bool trigger_glUniform3uiv = false;
void stub_glUniform3uiv() {
    if(trigger_glUniform3uiv) return;
    trigger_glUniform3uiv = true;
    printf("Stub: glUniform3uiv\n");
}
static bool trigger_glUniform4uiv = false;
void stub_glUniform4uiv() {
    if(trigger_glUniform4uiv) return;
    trigger_glUniform4uiv = true;
    printf("Stub: glUniform4uiv\n");
}
static bool trigger_glTexParameterIiv = false;
void stub_glTexParameterIiv() {
    if(trigger_glTexParameterIiv) return;
    trigger_glTexParameterIiv = true;
    printf("Stub: glTexParameterIiv\n");
}
static bool trigger_glTexParameterIuiv = false;
void stub_glTexParameterIuiv() {
    if(trigger_glTexParameterIuiv) return;
    trigger_glTexParameterIuiv = true;
    printf("Stub: glTexParameterIuiv\n");
}
static bool trigger_glGetTexParameterIiv = false;
void stub_glGetTexParameterIiv() {
    if(trigger_glGetTexParameterIiv) return;
    trigger_glGetTexParameterIiv = true;
    printf("Stub: glGetTexParameterIiv\n");
}
static bool trigger_glGetTexParameterIuiv = false;
void stub_glGetTexParameterIuiv() {
    if(trigger_glGetTexParameterIuiv) return;
    trigger_glGetTexParameterIuiv = true;
    printf("Stub: glGetTexParameterIuiv\n");
}
static bool trigger_glClearBufferiv = false;
void stub_glClearBufferiv() {
    if(trigger_glClearBufferiv) return;
    trigger_glClearBufferiv = true;
    printf("Stub: glClearBufferiv\n");
}
static bool trigger_glClearBufferuiv = false;
void stub_glClearBufferuiv() {
    if(trigger_glClearBufferuiv) return;
    trigger_glClearBufferuiv = true;
    printf("Stub: glClearBufferuiv\n");
}
static bool trigger_glClearBufferfv = false;
void stub_glClearBufferfv() {
    if(trigger_glClearBufferfv) return;
    trigger_glClearBufferfv = true;
    printf("Stub: glClearBufferfv\n");
}
static bool trigger_glClearBufferfi = false;
void stub_glClearBufferfi() {
    if(trigger_glClearBufferfi) return;
    trigger_glClearBufferfi = true;
    printf("Stub: glClearBufferfi\n");
}
static bool trigger_glGetStringi = false;
void stub_glGetStringi() {
    if(trigger_glGetStringi) return;
    trigger_glGetStringi = true;
    printf("Stub: glGetStringi\n");
}
static bool trigger_glIsRenderbuffer = false;
void stub_glIsRenderbuffer() {
    if(trigger_glIsRenderbuffer) return;
    trigger_glIsRenderbuffer = true;
    printf("Stub: glIsRenderbuffer\n");
}
static bool trigger_glBindRenderbuffer = false;
void stub_glBindRenderbuffer() {
    if(trigger_glBindRenderbuffer) return;
    trigger_glBindRenderbuffer = true;
    printf("Stub: glBindRenderbuffer\n");
}
static bool trigger_glDeleteRenderbuffers = false;
void stub_glDeleteRenderbuffers() {
    if(trigger_glDeleteRenderbuffers) return;
    trigger_glDeleteRenderbuffers = true;
    printf("Stub: glDeleteRenderbuffers\n");
}
static bool trigger_glGenRenderbuffers = false;
void stub_glGenRenderbuffers() {
    if(trigger_glGenRenderbuffers) return;
    trigger_glGenRenderbuffers = true;
    printf("Stub: glGenRenderbuffers\n");
}
static bool trigger_glRenderbufferStorage = false;
void stub_glRenderbufferStorage() {
    if(trigger_glRenderbufferStorage) return;
    trigger_glRenderbufferStorage = true;
    printf("Stub: glRenderbufferStorage\n");
}
static bool trigger_glGetRenderbufferParameteriv = false;
void stub_glGetRenderbufferParameteriv() {
    if(trigger_glGetRenderbufferParameteriv) return;
    trigger_glGetRenderbufferParameteriv = true;
    printf("Stub: glGetRenderbufferParameteriv\n");
}
static bool trigger_glIsFramebuffer = false;
void stub_glIsFramebuffer() {
    if(trigger_glIsFramebuffer) return;
    trigger_glIsFramebuffer = true;
    printf("Stub: glIsFramebuffer\n");
}
static bool trigger_glBindFramebuffer = false;
void stub_glBindFramebuffer() {
    if(trigger_glBindFramebuffer) return;
    trigger_glBindFramebuffer = true;
    printf("Stub: glBindFramebuffer\n");
}
static bool trigger_glDeleteFramebuffers = false;
void stub_glDeleteFramebuffers() {
    if(trigger_glDeleteFramebuffers) return;
    trigger_glDeleteFramebuffers = true;
    printf("Stub: glDeleteFramebuffers\n");
}
static bool trigger_glGenFramebuffers = false;
void stub_glGenFramebuffers() {
    if(trigger_glGenFramebuffers) return;
    trigger_glGenFramebuffers = true;
    printf("Stub: glGenFramebuffers\n");
}
static bool trigger_glCheckFramebufferStatus = false;
void stub_glCheckFramebufferStatus() {
    if(trigger_glCheckFramebufferStatus) return;
    trigger_glCheckFramebufferStatus = true;
    printf("Stub: glCheckFramebufferStatus\n");
}
static bool trigger_glFramebufferTexture1D = false;
void stub_glFramebufferTexture1D() {
    if(trigger_glFramebufferTexture1D) return;
    trigger_glFramebufferTexture1D = true;
    printf("Stub: glFramebufferTexture1D\n");
}
static bool trigger_glFramebufferTexture2D = false;
void stub_glFramebufferTexture2D() {
    if(trigger_glFramebufferTexture2D) return;
    trigger_glFramebufferTexture2D = true;
    printf("Stub: glFramebufferTexture2D\n");
}
static bool trigger_glFramebufferTexture3D = false;
void stub_glFramebufferTexture3D() {
    if(trigger_glFramebufferTexture3D) return;
    trigger_glFramebufferTexture3D = true;
    printf("Stub: glFramebufferTexture3D\n");
}
static bool trigger_glFramebufferRenderbuffer = false;
void stub_glFramebufferRenderbuffer() {
    if(trigger_glFramebufferRenderbuffer) return;
    trigger_glFramebufferRenderbuffer = true;
    printf("Stub: glFramebufferRenderbuffer\n");
}
static bool trigger_glGetFramebufferAttachmentParameteriv = false;
void stub_glGetFramebufferAttachmentParameteriv() {
    if(trigger_glGetFramebufferAttachmentParameteriv) return;
    trigger_glGetFramebufferAttachmentParameteriv = true;
    printf("Stub: glGetFramebufferAttachmentParameteriv\n");
}
static bool trigger_glGenerateMipmap = false;
void stub_glGenerateMipmap() {
    if(trigger_glGenerateMipmap) return;
    trigger_glGenerateMipmap = true;
    printf("Stub: glGenerateMipmap\n");
}
static bool trigger_glBlitFramebuffer = false;
void stub_glBlitFramebuffer() {
    if(trigger_glBlitFramebuffer) return;
    trigger_glBlitFramebuffer = true;
    printf("Stub: glBlitFramebuffer\n");
}
static bool trigger_glRenderbufferStorageMultisample = false;
void stub_glRenderbufferStorageMultisample() {
    if(trigger_glRenderbufferStorageMultisample) return;
    trigger_glRenderbufferStorageMultisample = true;
    printf("Stub: glRenderbufferStorageMultisample\n");
}
static bool trigger_glFramebufferTextureLayer = false;
void stub_glFramebufferTextureLayer() {
    if(trigger_glFramebufferTextureLayer) return;
    trigger_glFramebufferTextureLayer = true;
    printf("Stub: glFramebufferTextureLayer\n");
}
static bool trigger_glMapBufferRange = false;
void stub_glMapBufferRange() {
    if(trigger_glMapBufferRange) return;
    trigger_glMapBufferRange = true;
    printf("Stub: glMapBufferRange\n");
}
static bool trigger_glFlushMappedBufferRange = false;
void stub_glFlushMappedBufferRange() {
    if(trigger_glFlushMappedBufferRange) return;
    trigger_glFlushMappedBufferRange = true;
    printf("Stub: glFlushMappedBufferRange\n");
}
static bool trigger_glBindVertexArray = false;
void stub_glBindVertexArray() {
    if(trigger_glBindVertexArray) return;
    trigger_glBindVertexArray = true;
    printf("Stub: glBindVertexArray\n");
}
static bool trigger_glDeleteVertexArrays = false;
void stub_glDeleteVertexArrays() {
    if(trigger_glDeleteVertexArrays) return;
    trigger_glDeleteVertexArrays = true;
    printf("Stub: glDeleteVertexArrays\n");
}
static bool trigger_glGenVertexArrays = false;
void stub_glGenVertexArrays() {
    if(trigger_glGenVertexArrays) return;
    trigger_glGenVertexArrays = true;
    printf("Stub: glGenVertexArrays\n");
}
static bool trigger_glIsVertexArray = false;
void stub_glIsVertexArray() {
    if(trigger_glIsVertexArray) return;
    trigger_glIsVertexArray = true;
    printf("Stub: glIsVertexArray\n");
}
static bool trigger_glDrawArraysInstanced = false;
void stub_glDrawArraysInstanced() {
    if(trigger_glDrawArraysInstanced) return;
    trigger_glDrawArraysInstanced = true;
    printf("Stub: glDrawArraysInstanced\n");
}
static bool trigger_glDrawElementsInstanced = false;
void stub_glDrawElementsInstanced() {
    if(trigger_glDrawElementsInstanced) return;
    trigger_glDrawElementsInstanced = true;
    printf("Stub: glDrawElementsInstanced\n");
}
static bool trigger_glTexBuffer = false;
void stub_glTexBuffer() {
    if(trigger_glTexBuffer) return;
    trigger_glTexBuffer = true;
    printf("Stub: glTexBuffer\n");
}
static bool trigger_glPrimitiveRestartIndex = false;
void stub_glPrimitiveRestartIndex() {
    if(trigger_glPrimitiveRestartIndex) return;
    trigger_glPrimitiveRestartIndex = true;
    printf("Stub: glPrimitiveRestartIndex\n");
}
static bool trigger_glCopyBufferSubData = false;
void stub_glCopyBufferSubData() {
    if(trigger_glCopyBufferSubData) return;
    trigger_glCopyBufferSubData = true;
    printf("Stub: glCopyBufferSubData\n");
}
static bool trigger_glGetUniformIndices = false;
void stub_glGetUniformIndices() {
    if(trigger_glGetUniformIndices) return;
    trigger_glGetUniformIndices = true;
    printf("Stub: glGetUniformIndices\n");
}
static bool trigger_glGetActiveUniformsiv = false;
void stub_glGetActiveUniformsiv() {
    if(trigger_glGetActiveUniformsiv) return;
    trigger_glGetActiveUniformsiv = true;
    printf("Stub: glGetActiveUniformsiv\n");
}
static bool trigger_glGetActiveUniformName = false;
void stub_glGetActiveUniformName() {
    if(trigger_glGetActiveUniformName) return;
    trigger_glGetActiveUniformName = true;
    printf("Stub: glGetActiveUniformName\n");
}
static bool trigger_glGetUniformBlockIndex = false;
void stub_glGetUniformBlockIndex() {
    if(trigger_glGetUniformBlockIndex) return;
    trigger_glGetUniformBlockIndex = true;
    printf("Stub: glGetUniformBlockIndex\n");
}
static bool trigger_glGetActiveUniformBlockiv = false;
void stub_glGetActiveUniformBlockiv() {
    if(trigger_glGetActiveUniformBlockiv) return;
    trigger_glGetActiveUniformBlockiv = true;
    printf("Stub: glGetActiveUniformBlockiv\n");
}
static bool trigger_glGetActiveUniformBlockName = false;
void stub_glGetActiveUniformBlockName() {
    if(trigger_glGetActiveUniformBlockName) return;
    trigger_glGetActiveUniformBlockName = true;
    printf("Stub: glGetActiveUniformBlockName\n");
}
static bool trigger_glUniformBlockBinding = false;
void stub_glUniformBlockBinding() {
    if(trigger_glUniformBlockBinding) return;
    trigger_glUniformBlockBinding = true;
    printf("Stub: glUniformBlockBinding\n");
}
static bool trigger_glDrawElementsBaseVertex = false;
void stub_glDrawElementsBaseVertex() {
    if(trigger_glDrawElementsBaseVertex) return;
    trigger_glDrawElementsBaseVertex = true;
    printf("Stub: glDrawElementsBaseVertex\n");
}
static bool trigger_glDrawRangeElementsBaseVertex = false;
void stub_glDrawRangeElementsBaseVertex() {
    if(trigger_glDrawRangeElementsBaseVertex) return;
    trigger_glDrawRangeElementsBaseVertex = true;
    printf("Stub: glDrawRangeElementsBaseVertex\n");
}
static bool trigger_glDrawElementsInstancedBaseVertex = false;
void stub_glDrawElementsInstancedBaseVertex() {
    if(trigger_glDrawElementsInstancedBaseVertex) return;
    trigger_glDrawElementsInstancedBaseVertex = true;
    printf("Stub: glDrawElementsInstancedBaseVertex\n");
}
static bool trigger_glMultiDrawElementsBaseVertex = false;
void stub_glMultiDrawElementsBaseVertex() {
    if(trigger_glMultiDrawElementsBaseVertex) return;
    trigger_glMultiDrawElementsBaseVertex = true;
    printf("Stub: glMultiDrawElementsBaseVertex\n");
}
static bool trigger_glProvokingVertex = false;
void stub_glProvokingVertex() {
    if(trigger_glProvokingVertex) return;
    trigger_glProvokingVertex = true;
    printf("Stub: glProvokingVertex\n");
}
static bool trigger_glFenceSync = false;
void stub_glFenceSync() {
    if(trigger_glFenceSync) return;
    trigger_glFenceSync = true;
    printf("Stub: glFenceSync\n");
}
static bool trigger_glIsSync = false;
void stub_glIsSync() {
    if(trigger_glIsSync) return;
    trigger_glIsSync = true;
    printf("Stub: glIsSync\n");
}
static bool trigger_glDeleteSync = false;
void stub_glDeleteSync() {
    if(trigger_glDeleteSync) return;
    trigger_glDeleteSync = true;
    printf("Stub: glDeleteSync\n");
}
static bool trigger_glClientWaitSync = false;
void stub_glClientWaitSync() {
    if(trigger_glClientWaitSync) return;
    trigger_glClientWaitSync = true;
    printf("Stub: glClientWaitSync\n");
}
static bool trigger_glWaitSync = false;
void stub_glWaitSync() {
    if(trigger_glWaitSync) return;
    trigger_glWaitSync = true;
    printf("Stub: glWaitSync\n");
}
static bool trigger_glGetInteger64v = false;
void stub_glGetInteger64v() {
    if(trigger_glGetInteger64v) return;
    trigger_glGetInteger64v = true;
    printf("Stub: glGetInteger64v\n");
}
static bool trigger_glGetSynciv = false;
void stub_glGetSynciv() {
    if(trigger_glGetSynciv) return;
    trigger_glGetSynciv = true;
    printf("Stub: glGetSynciv\n");
}
static bool trigger_glGetInteger64i_v = false;
void stub_glGetInteger64i_v() {
    if(trigger_glGetInteger64i_v) return;
    trigger_glGetInteger64i_v = true;
    printf("Stub: glGetInteger64i_v\n");
}
static bool trigger_glGetBufferParameteri64v = false;
void stub_glGetBufferParameteri64v() {
    if(trigger_glGetBufferParameteri64v) return;
    trigger_glGetBufferParameteri64v = true;
    printf("Stub: glGetBufferParameteri64v\n");
}
static bool trigger_glFramebufferTexture = false;
void stub_glFramebufferTexture() {
    if(trigger_glFramebufferTexture) return;
    trigger_glFramebufferTexture = true;
    printf("Stub: glFramebufferTexture\n");
}
static bool trigger_glTexImage2DMultisample = false;
void stub_glTexImage2DMultisample() {
    if(trigger_glTexImage2DMultisample) return;
    trigger_glTexImage2DMultisample = true;
    printf("Stub: glTexImage2DMultisample\n");
}
static bool trigger_glTexImage3DMultisample = false;
void stub_glTexImage3DMultisample() {
    if(trigger_glTexImage3DMultisample) return;
    trigger_glTexImage3DMultisample = true;
    printf("Stub: glTexImage3DMultisample\n");
}
static bool trigger_glGetMultisamplefv = false;
void stub_glGetMultisamplefv() {
    if(trigger_glGetMultisamplefv) return;
    trigger_glGetMultisamplefv = true;
    printf("Stub: glGetMultisamplefv\n");
}
static bool trigger_glSampleMaski = false;
void stub_glSampleMaski() {
    if(trigger_glSampleMaski) return;
    trigger_glSampleMaski = true;
    printf("Stub: glSampleMaski\n");
}
static bool trigger_glBindFragDataLocationIndexed = false;
void stub_glBindFragDataLocationIndexed() {
    if(trigger_glBindFragDataLocationIndexed) return;
    trigger_glBindFragDataLocationIndexed = true;
    printf("Stub: glBindFragDataLocationIndexed\n");
}
static bool trigger_glGetFragDataIndex = false;
void stub_glGetFragDataIndex() {
    if(trigger_glGetFragDataIndex) return;
    trigger_glGetFragDataIndex = true;
    printf("Stub: glGetFragDataIndex\n");
}
static bool trigger_glGenSamplers = false;
void stub_glGenSamplers() {
    if(trigger_glGenSamplers) return;
    trigger_glGenSamplers = true;
    printf("Stub: glGenSamplers\n");
}
static bool trigger_glDeleteSamplers = false;
void stub_glDeleteSamplers() {
    if(trigger_glDeleteSamplers) return;
    trigger_glDeleteSamplers = true;
    printf("Stub: glDeleteSamplers\n");
}
static bool trigger_glIsSampler = false;
void stub_glIsSampler() {
    if(trigger_glIsSampler) return;
    trigger_glIsSampler = true;
    printf("Stub: glIsSampler\n");
}
static bool trigger_glBindSampler = false;
void stub_glBindSampler() {
    if(trigger_glBindSampler) return;
    trigger_glBindSampler = true;
    printf("Stub: glBindSampler\n");
}
static bool trigger_glSamplerParameteri = false;
void stub_glSamplerParameteri() {
    if(trigger_glSamplerParameteri) return;
    trigger_glSamplerParameteri = true;
    printf("Stub: glSamplerParameteri\n");
}
static bool trigger_glSamplerParameteriv = false;
void stub_glSamplerParameteriv() {
    if(trigger_glSamplerParameteriv) return;
    trigger_glSamplerParameteriv = true;
    printf("Stub: glSamplerParameteriv\n");
}
static bool trigger_glSamplerParameterf = false;
void stub_glSamplerParameterf() {
    if(trigger_glSamplerParameterf) return;
    trigger_glSamplerParameterf = true;
    printf("Stub: glSamplerParameterf\n");
}
static bool trigger_glSamplerParameterfv = false;
void stub_glSamplerParameterfv() {
    if(trigger_glSamplerParameterfv) return;
    trigger_glSamplerParameterfv = true;
    printf("Stub: glSamplerParameterfv\n");
}
static bool trigger_glSamplerParameterIiv = false;
void stub_glSamplerParameterIiv() {
    if(trigger_glSamplerParameterIiv) return;
    trigger_glSamplerParameterIiv = true;
    printf("Stub: glSamplerParameterIiv\n");
}
static bool trigger_glSamplerParameterIuiv = false;
void stub_glSamplerParameterIuiv() {
    if(trigger_glSamplerParameterIuiv) return;
    trigger_glSamplerParameterIuiv = true;
    printf("Stub: glSamplerParameterIuiv\n");
}
static bool trigger_glGetSamplerParameteriv = false;
void stub_glGetSamplerParameteriv() {
    if(trigger_glGetSamplerParameteriv) return;
    trigger_glGetSamplerParameteriv = true;
    printf("Stub: glGetSamplerParameteriv\n");
}
static bool trigger_glGetSamplerParameterIiv = false;
void stub_glGetSamplerParameterIiv() {
    if(trigger_glGetSamplerParameterIiv) return;
    trigger_glGetSamplerParameterIiv = true;
    printf("Stub: glGetSamplerParameterIiv\n");
}
static bool trigger_glGetSamplerParameterfv = false;
void stub_glGetSamplerParameterfv() {
    if(trigger_glGetSamplerParameterfv) return;
    trigger_glGetSamplerParameterfv = true;
    printf("Stub: glGetSamplerParameterfv\n");
}
static bool trigger_glGetSamplerParameterIuiv = false;
void stub_glGetSamplerParameterIuiv() {
    if(trigger_glGetSamplerParameterIuiv) return;
    trigger_glGetSamplerParameterIuiv = true;
    printf("Stub: glGetSamplerParameterIuiv\n");
}
static bool trigger_glQueryCounter = false;
void stub_glQueryCounter() {
    if(trigger_glQueryCounter) return;
    trigger_glQueryCounter = true;
    printf("Stub: glQueryCounter\n");
}
static bool trigger_glGetQueryObjecti64v = false;
void stub_glGetQueryObjecti64v() {
    if(trigger_glGetQueryObjecti64v) return;
    trigger_glGetQueryObjecti64v = true;
    printf("Stub: glGetQueryObjecti64v\n");
}
static bool trigger_glGetQueryObjectui64v = false;
void stub_glGetQueryObjectui64v() {
    if(trigger_glGetQueryObjectui64v) return;
    trigger_glGetQueryObjectui64v = true;
    printf("Stub: glGetQueryObjectui64v\n");
}
static bool trigger_glVertexAttribDivisor = false;
void stub_glVertexAttribDivisor() {
    if(trigger_glVertexAttribDivisor) return;
    trigger_glVertexAttribDivisor = true;
    printf("Stub: glVertexAttribDivisor\n");
}
static bool trigger_glVertexAttribP1ui = false;
void stub_glVertexAttribP1ui() {
    if(trigger_glVertexAttribP1ui) return;
    trigger_glVertexAttribP1ui = true;
    printf("Stub: glVertexAttribP1ui\n");
}
static bool trigger_glVertexAttribP1uiv = false;
void stub_glVertexAttribP1uiv() {
    if(trigger_glVertexAttribP1uiv) return;
    trigger_glVertexAttribP1uiv = true;
    printf("Stub: glVertexAttribP1uiv\n");
}
static bool trigger_glVertexAttribP2ui = false;
void stub_glVertexAttribP2ui() {
    if(trigger_glVertexAttribP2ui) return;
    trigger_glVertexAttribP2ui = true;
    printf("Stub: glVertexAttribP2ui\n");
}
static bool trigger_glVertexAttribP2uiv = false;
void stub_glVertexAttribP2uiv() {
    if(trigger_glVertexAttribP2uiv) return;
    trigger_glVertexAttribP2uiv = true;
    printf("Stub: glVertexAttribP2uiv\n");
}
static bool trigger_glVertexAttribP3ui = false;
void stub_glVertexAttribP3ui() {
    if(trigger_glVertexAttribP3ui) return;
    trigger_glVertexAttribP3ui = true;
    printf("Stub: glVertexAttribP3ui\n");
}
static bool trigger_glVertexAttribP3uiv = false;
void stub_glVertexAttribP3uiv() {
    if(trigger_glVertexAttribP3uiv) return;
    trigger_glVertexAttribP3uiv = true;
    printf("Stub: glVertexAttribP3uiv\n");
}
static bool trigger_glVertexAttribP4ui = false;
void stub_glVertexAttribP4ui() {
    if(trigger_glVertexAttribP4ui) return;
    trigger_glVertexAttribP4ui = true;
    printf("Stub: glVertexAttribP4ui\n");
}
static bool trigger_glVertexAttribP4uiv = false;
void stub_glVertexAttribP4uiv() {
    if(trigger_glVertexAttribP4uiv) return;
    trigger_glVertexAttribP4uiv = true;
    printf("Stub: glVertexAttribP4uiv\n");
}
static bool trigger_glMinSampleShading = false;
void stub_glMinSampleShading() {
    if(trigger_glMinSampleShading) return;
    trigger_glMinSampleShading = true;
    printf("Stub: glMinSampleShading\n");
}
static bool trigger_glBlendEquationi = false;
void stub_glBlendEquationi() {
    if(trigger_glBlendEquationi) return;
    trigger_glBlendEquationi = true;
    printf("Stub: glBlendEquationi\n");
}
static bool trigger_glBlendEquationSeparatei = false;
void stub_glBlendEquationSeparatei() {
    if(trigger_glBlendEquationSeparatei) return;
    trigger_glBlendEquationSeparatei = true;
    printf("Stub: glBlendEquationSeparatei\n");
}
static bool trigger_glBlendFunci = false;
void stub_glBlendFunci() {
    if(trigger_glBlendFunci) return;
    trigger_glBlendFunci = true;
    printf("Stub: glBlendFunci\n");
}
static bool trigger_glBlendFuncSeparatei = false;
void stub_glBlendFuncSeparatei() {
    if(trigger_glBlendFuncSeparatei) return;
    trigger_glBlendFuncSeparatei = true;
    printf("Stub: glBlendFuncSeparatei\n");
}
static bool trigger_glDrawArraysIndirect = false;
void stub_glDrawArraysIndirect() {
    if(trigger_glDrawArraysIndirect) return;
    trigger_glDrawArraysIndirect = true;
    printf("Stub: glDrawArraysIndirect\n");
}
static bool trigger_glDrawElementsIndirect = false;
void stub_glDrawElementsIndirect() {
    if(trigger_glDrawElementsIndirect) return;
    trigger_glDrawElementsIndirect = true;
    printf("Stub: glDrawElementsIndirect\n");
}
static bool trigger_glUniform1d = false;
void stub_glUniform1d() {
    if(trigger_glUniform1d) return;
    trigger_glUniform1d = true;
    printf("Stub: glUniform1d\n");
}
static bool trigger_glUniform2d = false;
void stub_glUniform2d() {
    if(trigger_glUniform2d) return;
    trigger_glUniform2d = true;
    printf("Stub: glUniform2d\n");
}
static bool trigger_glUniform3d = false;
void stub_glUniform3d() {
    if(trigger_glUniform3d) return;
    trigger_glUniform3d = true;
    printf("Stub: glUniform3d\n");
}
static bool trigger_glUniform4d = false;
void stub_glUniform4d() {
    if(trigger_glUniform4d) return;
    trigger_glUniform4d = true;
    printf("Stub: glUniform4d\n");
}
static bool trigger_glUniform1dv = false;
void stub_glUniform1dv() {
    if(trigger_glUniform1dv) return;
    trigger_glUniform1dv = true;
    printf("Stub: glUniform1dv\n");
}
static bool trigger_glUniform2dv = false;
void stub_glUniform2dv() {
    if(trigger_glUniform2dv) return;
    trigger_glUniform2dv = true;
    printf("Stub: glUniform2dv\n");
}
static bool trigger_glUniform3dv = false;
void stub_glUniform3dv() {
    if(trigger_glUniform3dv) return;
    trigger_glUniform3dv = true;
    printf("Stub: glUniform3dv\n");
}
static bool trigger_glUniform4dv = false;
void stub_glUniform4dv() {
    if(trigger_glUniform4dv) return;
    trigger_glUniform4dv = true;
    printf("Stub: glUniform4dv\n");
}
static bool trigger_glUniformMatrix2dv = false;
void stub_glUniformMatrix2dv() {
    if(trigger_glUniformMatrix2dv) return;
    trigger_glUniformMatrix2dv = true;
    printf("Stub: glUniformMatrix2dv\n");
}
static bool trigger_glUniformMatrix3dv = false;
void stub_glUniformMatrix3dv() {
    if(trigger_glUniformMatrix3dv) return;
    trigger_glUniformMatrix3dv = true;
    printf("Stub: glUniformMatrix3dv\n");
}
static bool trigger_glUniformMatrix4dv = false;
void stub_glUniformMatrix4dv() {
    if(trigger_glUniformMatrix4dv) return;
    trigger_glUniformMatrix4dv = true;
    printf("Stub: glUniformMatrix4dv\n");
}
static bool trigger_glUniformMatrix2x3dv = false;
void stub_glUniformMatrix2x3dv() {
    if(trigger_glUniformMatrix2x3dv) return;
    trigger_glUniformMatrix2x3dv = true;
    printf("Stub: glUniformMatrix2x3dv\n");
}
static bool trigger_glUniformMatrix2x4dv = false;
void stub_glUniformMatrix2x4dv() {
    if(trigger_glUniformMatrix2x4dv) return;
    trigger_glUniformMatrix2x4dv = true;
    printf("Stub: glUniformMatrix2x4dv\n");
}
static bool trigger_glUniformMatrix3x2dv = false;
void stub_glUniformMatrix3x2dv() {
    if(trigger_glUniformMatrix3x2dv) return;
    trigger_glUniformMatrix3x2dv = true;
    printf("Stub: glUniformMatrix3x2dv\n");
}
static bool trigger_glUniformMatrix3x4dv = false;
void stub_glUniformMatrix3x4dv() {
    if(trigger_glUniformMatrix3x4dv) return;
    trigger_glUniformMatrix3x4dv = true;
    printf("Stub: glUniformMatrix3x4dv\n");
}
static bool trigger_glUniformMatrix4x2dv = false;
void stub_glUniformMatrix4x2dv() {
    if(trigger_glUniformMatrix4x2dv) return;
    trigger_glUniformMatrix4x2dv = true;
    printf("Stub: glUniformMatrix4x2dv\n");
}
static bool trigger_glUniformMatrix4x3dv = false;
void stub_glUniformMatrix4x3dv() {
    if(trigger_glUniformMatrix4x3dv) return;
    trigger_glUniformMatrix4x3dv = true;
    printf("Stub: glUniformMatrix4x3dv\n");
}
static bool trigger_glGetUniformdv = false;
void stub_glGetUniformdv() {
    if(trigger_glGetUniformdv) return;
    trigger_glGetUniformdv = true;
    printf("Stub: glGetUniformdv\n");
}
static bool trigger_glGetSubroutineUniformLocation = false;
void stub_glGetSubroutineUniformLocation() {
    if(trigger_glGetSubroutineUniformLocation) return;
    trigger_glGetSubroutineUniformLocation = true;
    printf("Stub: glGetSubroutineUniformLocation\n");
}
static bool trigger_glGetSubroutineIndex = false;
void stub_glGetSubroutineIndex() {
    if(trigger_glGetSubroutineIndex) return;
    trigger_glGetSubroutineIndex = true;
    printf("Stub: glGetSubroutineIndex\n");
}
static bool trigger_glGetActiveSubroutineUniformiv = false;
void stub_glGetActiveSubroutineUniformiv() {
    if(trigger_glGetActiveSubroutineUniformiv) return;
    trigger_glGetActiveSubroutineUniformiv = true;
    printf("Stub: glGetActiveSubroutineUniformiv\n");
}
static bool trigger_glGetActiveSubroutineUniformName = false;
void stub_glGetActiveSubroutineUniformName() {
    if(trigger_glGetActiveSubroutineUniformName) return;
    trigger_glGetActiveSubroutineUniformName = true;
    printf("Stub: glGetActiveSubroutineUniformName\n");
}
static bool trigger_glGetActiveSubroutineName = false;
void stub_glGetActiveSubroutineName() {
    if(trigger_glGetActiveSubroutineName) return;
    trigger_glGetActiveSubroutineName = true;
    printf("Stub: glGetActiveSubroutineName\n");
}
static bool trigger_glUniformSubroutinesuiv = false;
void stub_glUniformSubroutinesuiv() {
    if(trigger_glUniformSubroutinesuiv) return;
    trigger_glUniformSubroutinesuiv = true;
    printf("Stub: glUniformSubroutinesuiv\n");
}
static bool trigger_glGetUniformSubroutineuiv = false;
void stub_glGetUniformSubroutineuiv() {
    if(trigger_glGetUniformSubroutineuiv) return;
    trigger_glGetUniformSubroutineuiv = true;
    printf("Stub: glGetUniformSubroutineuiv\n");
}
static bool trigger_glGetProgramStageiv = false;
void stub_glGetProgramStageiv() {
    if(trigger_glGetProgramStageiv) return;
    trigger_glGetProgramStageiv = true;
    printf("Stub: glGetProgramStageiv\n");
}
static bool trigger_glPatchParameteri = false;
void stub_glPatchParameteri() {
    if(trigger_glPatchParameteri) return;
    trigger_glPatchParameteri = true;
    printf("Stub: glPatchParameteri\n");
}
static bool trigger_glPatchParameterfv = false;
void stub_glPatchParameterfv() {
    if(trigger_glPatchParameterfv) return;
    trigger_glPatchParameterfv = true;
    printf("Stub: glPatchParameterfv\n");
}
static bool trigger_glBindTransformFeedback = false;
void stub_glBindTransformFeedback() {
    if(trigger_glBindTransformFeedback) return;
    trigger_glBindTransformFeedback = true;
    printf("Stub: glBindTransformFeedback\n");
}
static bool trigger_glDeleteTransformFeedbacks = false;
void stub_glDeleteTransformFeedbacks() {
    if(trigger_glDeleteTransformFeedbacks) return;
    trigger_glDeleteTransformFeedbacks = true;
    printf("Stub: glDeleteTransformFeedbacks\n");
}
static bool trigger_glGenTransformFeedbacks = false;
void stub_glGenTransformFeedbacks() {
    if(trigger_glGenTransformFeedbacks) return;
    trigger_glGenTransformFeedbacks = true;
    printf("Stub: glGenTransformFeedbacks\n");
}
static bool trigger_glIsTransformFeedback = false;
void stub_glIsTransformFeedback() {
    if(trigger_glIsTransformFeedback) return;
    trigger_glIsTransformFeedback = true;
    printf("Stub: glIsTransformFeedback\n");
}
static bool trigger_glPauseTransformFeedback = false;
void stub_glPauseTransformFeedback() {
    if(trigger_glPauseTransformFeedback) return;
    trigger_glPauseTransformFeedback = true;
    printf("Stub: glPauseTransformFeedback\n");
}
static bool trigger_glResumeTransformFeedback = false;
void stub_glResumeTransformFeedback() {
    if(trigger_glResumeTransformFeedback) return;
    trigger_glResumeTransformFeedback = true;
    printf("Stub: glResumeTransformFeedback\n");
}
static bool trigger_glDrawTransformFeedback = false;
void stub_glDrawTransformFeedback() {
    if(trigger_glDrawTransformFeedback) return;
    trigger_glDrawTransformFeedback = true;
    printf("Stub: glDrawTransformFeedback\n");
}
static bool trigger_glDrawTransformFeedbackStream = false;
void stub_glDrawTransformFeedbackStream() {
    if(trigger_glDrawTransformFeedbackStream) return;
    trigger_glDrawTransformFeedbackStream = true;
    printf("Stub: glDrawTransformFeedbackStream\n");
}
static bool trigger_glBeginQueryIndexed = false;
void stub_glBeginQueryIndexed() {
    if(trigger_glBeginQueryIndexed) return;
    trigger_glBeginQueryIndexed = true;
    printf("Stub: glBeginQueryIndexed\n");
}
static bool trigger_glEndQueryIndexed = false;
void stub_glEndQueryIndexed() {
    if(trigger_glEndQueryIndexed) return;
    trigger_glEndQueryIndexed = true;
    printf("Stub: glEndQueryIndexed\n");
}
static bool trigger_glGetQueryIndexediv = false;
void stub_glGetQueryIndexediv() {
    if(trigger_glGetQueryIndexediv) return;
    trigger_glGetQueryIndexediv = true;
    printf("Stub: glGetQueryIndexediv\n");
}
static bool trigger_glReleaseShaderCompiler = false;
void stub_glReleaseShaderCompiler() {
    if(trigger_glReleaseShaderCompiler) return;
    trigger_glReleaseShaderCompiler = true;
    printf("Stub: glReleaseShaderCompiler\n");
}
static bool trigger_glShaderBinary = false;
void stub_glShaderBinary() {
    if(trigger_glShaderBinary) return;
    trigger_glShaderBinary = true;
    printf("Stub: glShaderBinary\n");
}
static bool trigger_glGetShaderPrecisionFormat = false;
void stub_glGetShaderPrecisionFormat() {
    if(trigger_glGetShaderPrecisionFormat) return;
    trigger_glGetShaderPrecisionFormat = true;
    printf("Stub: glGetShaderPrecisionFormat\n");
}
static bool trigger_glDepthRangef = false;
void stub_glDepthRangef() {
    if(trigger_glDepthRangef) return;
    trigger_glDepthRangef = true;
    printf("Stub: glDepthRangef\n");
}
static bool trigger_glClearDepthf = false;
void stub_glClearDepthf() {
    if(trigger_glClearDepthf) return;
    trigger_glClearDepthf = true;
    printf("Stub: glClearDepthf\n");
}
static bool trigger_glGetProgramBinary = false;
void stub_glGetProgramBinary() {
    if(trigger_glGetProgramBinary) return;
    trigger_glGetProgramBinary = true;
    printf("Stub: glGetProgramBinary\n");
}
static bool trigger_glProgramBinary = false;
void stub_glProgramBinary() {
    if(trigger_glProgramBinary) return;
    trigger_glProgramBinary = true;
    printf("Stub: glProgramBinary\n");
}
static bool trigger_glProgramParameteri = false;
void stub_glProgramParameteri() {
    if(trigger_glProgramParameteri) return;
    trigger_glProgramParameteri = true;
    printf("Stub: glProgramParameteri\n");
}
static bool trigger_glUseProgramStages = false;
void stub_glUseProgramStages() {
    if(trigger_glUseProgramStages) return;
    trigger_glUseProgramStages = true;
    printf("Stub: glUseProgramStages\n");
}
static bool trigger_glActiveShaderProgram = false;
void stub_glActiveShaderProgram() {
    if(trigger_glActiveShaderProgram) return;
    trigger_glActiveShaderProgram = true;
    printf("Stub: glActiveShaderProgram\n");
}
static bool trigger_glCreateShaderProgramv = false;
void stub_glCreateShaderProgramv() {
    if(trigger_glCreateShaderProgramv) return;
    trigger_glCreateShaderProgramv = true;
    printf("Stub: glCreateShaderProgramv\n");
}
static bool trigger_glBindProgramPipeline = false;
void stub_glBindProgramPipeline() {
    if(trigger_glBindProgramPipeline) return;
    trigger_glBindProgramPipeline = true;
    printf("Stub: glBindProgramPipeline\n");
}
static bool trigger_glDeleteProgramPipelines = false;
void stub_glDeleteProgramPipelines() {
    if(trigger_glDeleteProgramPipelines) return;
    trigger_glDeleteProgramPipelines = true;
    printf("Stub: glDeleteProgramPipelines\n");
}
static bool trigger_glGenProgramPipelines = false;
void stub_glGenProgramPipelines() {
    if(trigger_glGenProgramPipelines) return;
    trigger_glGenProgramPipelines = true;
    printf("Stub: glGenProgramPipelines\n");
}
static bool trigger_glIsProgramPipeline = false;
void stub_glIsProgramPipeline() {
    if(trigger_glIsProgramPipeline) return;
    trigger_glIsProgramPipeline = true;
    printf("Stub: glIsProgramPipeline\n");
}
static bool trigger_glGetProgramPipelineiv = false;
void stub_glGetProgramPipelineiv() {
    if(trigger_glGetProgramPipelineiv) return;
    trigger_glGetProgramPipelineiv = true;
    printf("Stub: glGetProgramPipelineiv\n");
}
static bool trigger_glProgramUniform1i = false;
void stub_glProgramUniform1i() {
    if(trigger_glProgramUniform1i) return;
    trigger_glProgramUniform1i = true;
    printf("Stub: glProgramUniform1i\n");
}
static bool trigger_glProgramUniform1iv = false;
void stub_glProgramUniform1iv() {
    if(trigger_glProgramUniform1iv) return;
    trigger_glProgramUniform1iv = true;
    printf("Stub: glProgramUniform1iv\n");
}
static bool trigger_glProgramUniform1f = false;
void stub_glProgramUniform1f() {
    if(trigger_glProgramUniform1f) return;
    trigger_glProgramUniform1f = true;
    printf("Stub: glProgramUniform1f\n");
}
static bool trigger_glProgramUniform1fv = false;
void stub_glProgramUniform1fv() {
    if(trigger_glProgramUniform1fv) return;
    trigger_glProgramUniform1fv = true;
    printf("Stub: glProgramUniform1fv\n");
}
static bool trigger_glProgramUniform1d = false;
void stub_glProgramUniform1d() {
    if(trigger_glProgramUniform1d) return;
    trigger_glProgramUniform1d = true;
    printf("Stub: glProgramUniform1d\n");
}
static bool trigger_glProgramUniform1dv = false;
void stub_glProgramUniform1dv() {
    if(trigger_glProgramUniform1dv) return;
    trigger_glProgramUniform1dv = true;
    printf("Stub: glProgramUniform1dv\n");
}
static bool trigger_glProgramUniform1ui = false;
void stub_glProgramUniform1ui() {
    if(trigger_glProgramUniform1ui) return;
    trigger_glProgramUniform1ui = true;
    printf("Stub: glProgramUniform1ui\n");
}
static bool trigger_glProgramUniform1uiv = false;
void stub_glProgramUniform1uiv() {
    if(trigger_glProgramUniform1uiv) return;
    trigger_glProgramUniform1uiv = true;
    printf("Stub: glProgramUniform1uiv\n");
}
static bool trigger_glProgramUniform2i = false;
void stub_glProgramUniform2i() {
    if(trigger_glProgramUniform2i) return;
    trigger_glProgramUniform2i = true;
    printf("Stub: glProgramUniform2i\n");
}
static bool trigger_glProgramUniform2iv = false;
void stub_glProgramUniform2iv() {
    if(trigger_glProgramUniform2iv) return;
    trigger_glProgramUniform2iv = true;
    printf("Stub: glProgramUniform2iv\n");
}
static bool trigger_glProgramUniform2f = false;
void stub_glProgramUniform2f() {
    if(trigger_glProgramUniform2f) return;
    trigger_glProgramUniform2f = true;
    printf("Stub: glProgramUniform2f\n");
}
static bool trigger_glProgramUniform2fv = false;
void stub_glProgramUniform2fv() {
    if(trigger_glProgramUniform2fv) return;
    trigger_glProgramUniform2fv = true;
    printf("Stub: glProgramUniform2fv\n");
}
static bool trigger_glProgramUniform2d = false;
void stub_glProgramUniform2d() {
    if(trigger_glProgramUniform2d) return;
    trigger_glProgramUniform2d = true;
    printf("Stub: glProgramUniform2d\n");
}
static bool trigger_glProgramUniform2dv = false;
void stub_glProgramUniform2dv() {
    if(trigger_glProgramUniform2dv) return;
    trigger_glProgramUniform2dv = true;
    printf("Stub: glProgramUniform2dv\n");
}
static bool trigger_glProgramUniform2ui = false;
void stub_glProgramUniform2ui() {
    if(trigger_glProgramUniform2ui) return;
    trigger_glProgramUniform2ui = true;
    printf("Stub: glProgramUniform2ui\n");
}
static bool trigger_glProgramUniform2uiv = false;
void stub_glProgramUniform2uiv() {
    if(trigger_glProgramUniform2uiv) return;
    trigger_glProgramUniform2uiv = true;
    printf("Stub: glProgramUniform2uiv\n");
}
static bool trigger_glProgramUniform3i = false;
void stub_glProgramUniform3i() {
    if(trigger_glProgramUniform3i) return;
    trigger_glProgramUniform3i = true;
    printf("Stub: glProgramUniform3i\n");
}
static bool trigger_glProgramUniform3iv = false;
void stub_glProgramUniform3iv() {
    if(trigger_glProgramUniform3iv) return;
    trigger_glProgramUniform3iv = true;
    printf("Stub: glProgramUniform3iv\n");
}
static bool trigger_glProgramUniform3f = false;
void stub_glProgramUniform3f() {
    if(trigger_glProgramUniform3f) return;
    trigger_glProgramUniform3f = true;
    printf("Stub: glProgramUniform3f\n");
}
static bool trigger_glProgramUniform3fv = false;
void stub_glProgramUniform3fv() {
    if(trigger_glProgramUniform3fv) return;
    trigger_glProgramUniform3fv = true;
    printf("Stub: glProgramUniform3fv\n");
}
static bool trigger_glProgramUniform3d = false;
void stub_glProgramUniform3d() {
    if(trigger_glProgramUniform3d) return;
    trigger_glProgramUniform3d = true;
    printf("Stub: glProgramUniform3d\n");
}
static bool trigger_glProgramUniform3dv = false;
void stub_glProgramUniform3dv() {
    if(trigger_glProgramUniform3dv) return;
    trigger_glProgramUniform3dv = true;
    printf("Stub: glProgramUniform3dv\n");
}
static bool trigger_glProgramUniform3ui = false;
void stub_glProgramUniform3ui() {
    if(trigger_glProgramUniform3ui) return;
    trigger_glProgramUniform3ui = true;
    printf("Stub: glProgramUniform3ui\n");
}
static bool trigger_glProgramUniform3uiv = false;
void stub_glProgramUniform3uiv() {
    if(trigger_glProgramUniform3uiv) return;
    trigger_glProgramUniform3uiv = true;
    printf("Stub: glProgramUniform3uiv\n");
}
static bool trigger_glProgramUniform4i = false;
void stub_glProgramUniform4i() {
    if(trigger_glProgramUniform4i) return;
    trigger_glProgramUniform4i = true;
    printf("Stub: glProgramUniform4i\n");
}
static bool trigger_glProgramUniform4iv = false;
void stub_glProgramUniform4iv() {
    if(trigger_glProgramUniform4iv) return;
    trigger_glProgramUniform4iv = true;
    printf("Stub: glProgramUniform4iv\n");
}
static bool trigger_glProgramUniform4f = false;
void stub_glProgramUniform4f() {
    if(trigger_glProgramUniform4f) return;
    trigger_glProgramUniform4f = true;
    printf("Stub: glProgramUniform4f\n");
}
static bool trigger_glProgramUniform4fv = false;
void stub_glProgramUniform4fv() {
    if(trigger_glProgramUniform4fv) return;
    trigger_glProgramUniform4fv = true;
    printf("Stub: glProgramUniform4fv\n");
}
static bool trigger_glProgramUniform4d = false;
void stub_glProgramUniform4d() {
    if(trigger_glProgramUniform4d) return;
    trigger_glProgramUniform4d = true;
    printf("Stub: glProgramUniform4d\n");
}
static bool trigger_glProgramUniform4dv = false;
void stub_glProgramUniform4dv() {
    if(trigger_glProgramUniform4dv) return;
    trigger_glProgramUniform4dv = true;
    printf("Stub: glProgramUniform4dv\n");
}
static bool trigger_glProgramUniform4ui = false;
void stub_glProgramUniform4ui() {
    if(trigger_glProgramUniform4ui) return;
    trigger_glProgramUniform4ui = true;
    printf("Stub: glProgramUniform4ui\n");
}
static bool trigger_glProgramUniform4uiv = false;
void stub_glProgramUniform4uiv() {
    if(trigger_glProgramUniform4uiv) return;
    trigger_glProgramUniform4uiv = true;
    printf("Stub: glProgramUniform4uiv\n");
}
static bool trigger_glProgramUniformMatrix2fv = false;
void stub_glProgramUniformMatrix2fv() {
    if(trigger_glProgramUniformMatrix2fv) return;
    trigger_glProgramUniformMatrix2fv = true;
    printf("Stub: glProgramUniformMatrix2fv\n");
}
static bool trigger_glProgramUniformMatrix3fv = false;
void stub_glProgramUniformMatrix3fv() {
    if(trigger_glProgramUniformMatrix3fv) return;
    trigger_glProgramUniformMatrix3fv = true;
    printf("Stub: glProgramUniformMatrix3fv\n");
}
static bool trigger_glProgramUniformMatrix4fv = false;
void stub_glProgramUniformMatrix4fv() {
    if(trigger_glProgramUniformMatrix4fv) return;
    trigger_glProgramUniformMatrix4fv = true;
    printf("Stub: glProgramUniformMatrix4fv\n");
}
static bool trigger_glProgramUniformMatrix2dv = false;
void stub_glProgramUniformMatrix2dv() {
    if(trigger_glProgramUniformMatrix2dv) return;
    trigger_glProgramUniformMatrix2dv = true;
    printf("Stub: glProgramUniformMatrix2dv\n");
}
static bool trigger_glProgramUniformMatrix3dv = false;
void stub_glProgramUniformMatrix3dv() {
    if(trigger_glProgramUniformMatrix3dv) return;
    trigger_glProgramUniformMatrix3dv = true;
    printf("Stub: glProgramUniformMatrix3dv\n");
}
static bool trigger_glProgramUniformMatrix4dv = false;
void stub_glProgramUniformMatrix4dv() {
    if(trigger_glProgramUniformMatrix4dv) return;
    trigger_glProgramUniformMatrix4dv = true;
    printf("Stub: glProgramUniformMatrix4dv\n");
}
static bool trigger_glProgramUniformMatrix2x3fv = false;
void stub_glProgramUniformMatrix2x3fv() {
    if(trigger_glProgramUniformMatrix2x3fv) return;
    trigger_glProgramUniformMatrix2x3fv = true;
    printf("Stub: glProgramUniformMatrix2x3fv\n");
}
static bool trigger_glProgramUniformMatrix3x2fv = false;
void stub_glProgramUniformMatrix3x2fv() {
    if(trigger_glProgramUniformMatrix3x2fv) return;
    trigger_glProgramUniformMatrix3x2fv = true;
    printf("Stub: glProgramUniformMatrix3x2fv\n");
}
static bool trigger_glProgramUniformMatrix2x4fv = false;
void stub_glProgramUniformMatrix2x4fv() {
    if(trigger_glProgramUniformMatrix2x4fv) return;
    trigger_glProgramUniformMatrix2x4fv = true;
    printf("Stub: glProgramUniformMatrix2x4fv\n");
}
static bool trigger_glProgramUniformMatrix4x2fv = false;
void stub_glProgramUniformMatrix4x2fv() {
    if(trigger_glProgramUniformMatrix4x2fv) return;
    trigger_glProgramUniformMatrix4x2fv = true;
    printf("Stub: glProgramUniformMatrix4x2fv\n");
}
static bool trigger_glProgramUniformMatrix3x4fv = false;
void stub_glProgramUniformMatrix3x4fv() {
    if(trigger_glProgramUniformMatrix3x4fv) return;
    trigger_glProgramUniformMatrix3x4fv = true;
    printf("Stub: glProgramUniformMatrix3x4fv\n");
}
static bool trigger_glProgramUniformMatrix4x3fv = false;
void stub_glProgramUniformMatrix4x3fv() {
    if(trigger_glProgramUniformMatrix4x3fv) return;
    trigger_glProgramUniformMatrix4x3fv = true;
    printf("Stub: glProgramUniformMatrix4x3fv\n");
}
static bool trigger_glProgramUniformMatrix2x3dv = false;
void stub_glProgramUniformMatrix2x3dv() {
    if(trigger_glProgramUniformMatrix2x3dv) return;
    trigger_glProgramUniformMatrix2x3dv = true;
    printf("Stub: glProgramUniformMatrix2x3dv\n");
}
static bool trigger_glProgramUniformMatrix3x2dv = false;
void stub_glProgramUniformMatrix3x2dv() {
    if(trigger_glProgramUniformMatrix3x2dv) return;
    trigger_glProgramUniformMatrix3x2dv = true;
    printf("Stub: glProgramUniformMatrix3x2dv\n");
}
static bool trigger_glProgramUniformMatrix2x4dv = false;
void stub_glProgramUniformMatrix2x4dv() {
    if(trigger_glProgramUniformMatrix2x4dv) return;
    trigger_glProgramUniformMatrix2x4dv = true;
    printf("Stub: glProgramUniformMatrix2x4dv\n");
}
static bool trigger_glProgramUniformMatrix4x2dv = false;
void stub_glProgramUniformMatrix4x2dv() {
    if(trigger_glProgramUniformMatrix4x2dv) return;
    trigger_glProgramUniformMatrix4x2dv = true;
    printf("Stub: glProgramUniformMatrix4x2dv\n");
}
static bool trigger_glProgramUniformMatrix3x4dv = false;
void stub_glProgramUniformMatrix3x4dv() {
    if(trigger_glProgramUniformMatrix3x4dv) return;
    trigger_glProgramUniformMatrix3x4dv = true;
    printf("Stub: glProgramUniformMatrix3x4dv\n");
}
static bool trigger_glProgramUniformMatrix4x3dv = false;
void stub_glProgramUniformMatrix4x3dv() {
    if(trigger_glProgramUniformMatrix4x3dv) return;
    trigger_glProgramUniformMatrix4x3dv = true;
    printf("Stub: glProgramUniformMatrix4x3dv\n");
}
static bool trigger_glValidateProgramPipeline = false;
void stub_glValidateProgramPipeline() {
    if(trigger_glValidateProgramPipeline) return;
    trigger_glValidateProgramPipeline = true;
    printf("Stub: glValidateProgramPipeline\n");
}
static bool trigger_glGetProgramPipelineInfoLog = false;
void stub_glGetProgramPipelineInfoLog() {
    if(trigger_glGetProgramPipelineInfoLog) return;
    trigger_glGetProgramPipelineInfoLog = true;
    printf("Stub: glGetProgramPipelineInfoLog\n");
}
static bool trigger_glVertexAttribL1d = false;
void stub_glVertexAttribL1d() {
    if(trigger_glVertexAttribL1d) return;
    trigger_glVertexAttribL1d = true;
    printf("Stub: glVertexAttribL1d\n");
}
static bool trigger_glVertexAttribL2d = false;
void stub_glVertexAttribL2d() {
    if(trigger_glVertexAttribL2d) return;
    trigger_glVertexAttribL2d = true;
    printf("Stub: glVertexAttribL2d\n");
}
static bool trigger_glVertexAttribL3d = false;
void stub_glVertexAttribL3d() {
    if(trigger_glVertexAttribL3d) return;
    trigger_glVertexAttribL3d = true;
    printf("Stub: glVertexAttribL3d\n");
}
static bool trigger_glVertexAttribL4d = false;
void stub_glVertexAttribL4d() {
    if(trigger_glVertexAttribL4d) return;
    trigger_glVertexAttribL4d = true;
    printf("Stub: glVertexAttribL4d\n");
}
static bool trigger_glVertexAttribL1dv = false;
void stub_glVertexAttribL1dv() {
    if(trigger_glVertexAttribL1dv) return;
    trigger_glVertexAttribL1dv = true;
    printf("Stub: glVertexAttribL1dv\n");
}
static bool trigger_glVertexAttribL2dv = false;
void stub_glVertexAttribL2dv() {
    if(trigger_glVertexAttribL2dv) return;
    trigger_glVertexAttribL2dv = true;
    printf("Stub: glVertexAttribL2dv\n");
}
static bool trigger_glVertexAttribL3dv = false;
void stub_glVertexAttribL3dv() {
    if(trigger_glVertexAttribL3dv) return;
    trigger_glVertexAttribL3dv = true;
    printf("Stub: glVertexAttribL3dv\n");
}
static bool trigger_glVertexAttribL4dv = false;
void stub_glVertexAttribL4dv() {
    if(trigger_glVertexAttribL4dv) return;
    trigger_glVertexAttribL4dv = true;
    printf("Stub: glVertexAttribL4dv\n");
}
static bool trigger_glVertexAttribLPointer = false;
void stub_glVertexAttribLPointer() {
    if(trigger_glVertexAttribLPointer) return;
    trigger_glVertexAttribLPointer = true;
    printf("Stub: glVertexAttribLPointer\n");
}
static bool trigger_glGetVertexAttribLdv = false;
void stub_glGetVertexAttribLdv() {
    if(trigger_glGetVertexAttribLdv) return;
    trigger_glGetVertexAttribLdv = true;
    printf("Stub: glGetVertexAttribLdv\n");
}
static bool trigger_glViewportArrayv = false;
void stub_glViewportArrayv() {
    if(trigger_glViewportArrayv) return;
    trigger_glViewportArrayv = true;
    printf("Stub: glViewportArrayv\n");
}
static bool trigger_glViewportIndexedf = false;
void stub_glViewportIndexedf() {
    if(trigger_glViewportIndexedf) return;
    trigger_glViewportIndexedf = true;
    printf("Stub: glViewportIndexedf\n");
}
static bool trigger_glViewportIndexedfv = false;
void stub_glViewportIndexedfv() {
    if(trigger_glViewportIndexedfv) return;
    trigger_glViewportIndexedfv = true;
    printf("Stub: glViewportIndexedfv\n");
}
static bool trigger_glScissorArrayv = false;
void stub_glScissorArrayv() {
    if(trigger_glScissorArrayv) return;
    trigger_glScissorArrayv = true;
    printf("Stub: glScissorArrayv\n");
}
static bool trigger_glScissorIndexed = false;
void stub_glScissorIndexed() {
    if(trigger_glScissorIndexed) return;
    trigger_glScissorIndexed = true;
    printf("Stub: glScissorIndexed\n");
}
static bool trigger_glScissorIndexedv = false;
void stub_glScissorIndexedv() {
    if(trigger_glScissorIndexedv) return;
    trigger_glScissorIndexedv = true;
    printf("Stub: glScissorIndexedv\n");
}
static bool trigger_glDepthRangeArrayv = false;
void stub_glDepthRangeArrayv() {
    if(trigger_glDepthRangeArrayv) return;
    trigger_glDepthRangeArrayv = true;
    printf("Stub: glDepthRangeArrayv\n");
}
static bool trigger_glDepthRangeIndexed = false;
void stub_glDepthRangeIndexed() {
    if(trigger_glDepthRangeIndexed) return;
    trigger_glDepthRangeIndexed = true;
    printf("Stub: glDepthRangeIndexed\n");
}
static bool trigger_glGetFloati_v = false;
void stub_glGetFloati_v() {
    if(trigger_glGetFloati_v) return;
    trigger_glGetFloati_v = true;
    printf("Stub: glGetFloati_v\n");
}
static bool trigger_glGetDoublei_v = false;
void stub_glGetDoublei_v() {
    if(trigger_glGetDoublei_v) return;
    trigger_glGetDoublei_v = true;
    printf("Stub: glGetDoublei_v\n");
}
static bool trigger_glDrawArraysInstancedBaseInstance = false;
void stub_glDrawArraysInstancedBaseInstance() {
    if(trigger_glDrawArraysInstancedBaseInstance) return;
    trigger_glDrawArraysInstancedBaseInstance = true;
    printf("Stub: glDrawArraysInstancedBaseInstance\n");
}
static bool trigger_glDrawElementsInstancedBaseInstance = false;
void stub_glDrawElementsInstancedBaseInstance() {
    if(trigger_glDrawElementsInstancedBaseInstance) return;
    trigger_glDrawElementsInstancedBaseInstance = true;
    printf("Stub: glDrawElementsInstancedBaseInstance\n");
}
static bool trigger_glDrawElementsInstancedBaseVertexBaseInstance = false;
void stub_glDrawElementsInstancedBaseVertexBaseInstance() {
    if(trigger_glDrawElementsInstancedBaseVertexBaseInstance) return;
    trigger_glDrawElementsInstancedBaseVertexBaseInstance = true;
    printf("Stub: glDrawElementsInstancedBaseVertexBaseInstance\n");
}
static bool trigger_glGetInternalformativ = false;
void stub_glGetInternalformativ() {
    if(trigger_glGetInternalformativ) return;
    trigger_glGetInternalformativ = true;
    printf("Stub: glGetInternalformativ\n");
}
static bool trigger_glGetActiveAtomicCounterBufferiv = false;
void stub_glGetActiveAtomicCounterBufferiv() {
    if(trigger_glGetActiveAtomicCounterBufferiv) return;
    trigger_glGetActiveAtomicCounterBufferiv = true;
    printf("Stub: glGetActiveAtomicCounterBufferiv\n");
}
static bool trigger_glBindImageTexture = false;
void stub_glBindImageTexture() {
    if(trigger_glBindImageTexture) return;
    trigger_glBindImageTexture = true;
    printf("Stub: glBindImageTexture\n");
}
static bool trigger_glMemoryBarrier = false;
void stub_glMemoryBarrier() {
    if(trigger_glMemoryBarrier) return;
    trigger_glMemoryBarrier = true;
    printf("Stub: glMemoryBarrier\n");
}
static bool trigger_glTexStorage1D = false;
void stub_glTexStorage1D() {
    if(trigger_glTexStorage1D) return;
    trigger_glTexStorage1D = true;
    printf("Stub: glTexStorage1D\n");
}
static bool trigger_glTexStorage2D = false;
void stub_glTexStorage2D() {
    if(trigger_glTexStorage2D) return;
    trigger_glTexStorage2D = true;
    printf("Stub: glTexStorage2D\n");
}
static bool trigger_glTexStorage3D = false;
void stub_glTexStorage3D() {
    if(trigger_glTexStorage3D) return;
    trigger_glTexStorage3D = true;
    printf("Stub: glTexStorage3D\n");
}
static bool trigger_glDrawTransformFeedbackInstanced = false;
void stub_glDrawTransformFeedbackInstanced() {
    if(trigger_glDrawTransformFeedbackInstanced) return;
    trigger_glDrawTransformFeedbackInstanced = true;
    printf("Stub: glDrawTransformFeedbackInstanced\n");
}
static bool trigger_glDrawTransformFeedbackStreamInstanced = false;
void stub_glDrawTransformFeedbackStreamInstanced() {
    if(trigger_glDrawTransformFeedbackStreamInstanced) return;
    trigger_glDrawTransformFeedbackStreamInstanced = true;
    printf("Stub: glDrawTransformFeedbackStreamInstanced\n");
}
static bool trigger_glClearBufferData = false;
void stub_glClearBufferData() {
    if(trigger_glClearBufferData) return;
    trigger_glClearBufferData = true;
    printf("Stub: glClearBufferData\n");
}
static bool trigger_glClearBufferSubData = false;
void stub_glClearBufferSubData() {
    if(trigger_glClearBufferSubData) return;
    trigger_glClearBufferSubData = true;
    printf("Stub: glClearBufferSubData\n");
}
static bool trigger_glDispatchCompute = false;
void stub_glDispatchCompute() {
    if(trigger_glDispatchCompute) return;
    trigger_glDispatchCompute = true;
    printf("Stub: glDispatchCompute\n");
}
static bool trigger_glDispatchComputeIndirect = false;
void stub_glDispatchComputeIndirect() {
    if(trigger_glDispatchComputeIndirect) return;
    trigger_glDispatchComputeIndirect = true;
    printf("Stub: glDispatchComputeIndirect\n");
}
static bool trigger_glCopyImageSubData = false;
void stub_glCopyImageSubData() {
    if(trigger_glCopyImageSubData) return;
    trigger_glCopyImageSubData = true;
    printf("Stub: glCopyImageSubData\n");
}
static bool trigger_glFramebufferParameteri = false;
void stub_glFramebufferParameteri() {
    if(trigger_glFramebufferParameteri) return;
    trigger_glFramebufferParameteri = true;
    printf("Stub: glFramebufferParameteri\n");
}
static bool trigger_glGetFramebufferParameteriv = false;
void stub_glGetFramebufferParameteriv() {
    if(trigger_glGetFramebufferParameteriv) return;
    trigger_glGetFramebufferParameteriv = true;
    printf("Stub: glGetFramebufferParameteriv\n");
}
static bool trigger_glGetInternalformati64v = false;
void stub_glGetInternalformati64v() {
    if(trigger_glGetInternalformati64v) return;
    trigger_glGetInternalformati64v = true;
    printf("Stub: glGetInternalformati64v\n");
}
static bool trigger_glInvalidateTexSubImage = false;
void stub_glInvalidateTexSubImage() {
    if(trigger_glInvalidateTexSubImage) return;
    trigger_glInvalidateTexSubImage = true;
    printf("Stub: glInvalidateTexSubImage\n");
}
static bool trigger_glInvalidateTexImage = false;
void stub_glInvalidateTexImage() {
    if(trigger_glInvalidateTexImage) return;
    trigger_glInvalidateTexImage = true;
    printf("Stub: glInvalidateTexImage\n");
}
static bool trigger_glInvalidateBufferSubData = false;
void stub_glInvalidateBufferSubData() {
    if(trigger_glInvalidateBufferSubData) return;
    trigger_glInvalidateBufferSubData = true;
    printf("Stub: glInvalidateBufferSubData\n");
}
static bool trigger_glInvalidateBufferData = false;
void stub_glInvalidateBufferData() {
    if(trigger_glInvalidateBufferData) return;
    trigger_glInvalidateBufferData = true;
    printf("Stub: glInvalidateBufferData\n");
}
static bool trigger_glInvalidateFramebuffer = false;
void stub_glInvalidateFramebuffer() {
    if(trigger_glInvalidateFramebuffer) return;
    trigger_glInvalidateFramebuffer = true;
    printf("Stub: glInvalidateFramebuffer\n");
}
static bool trigger_glInvalidateSubFramebuffer = false;
void stub_glInvalidateSubFramebuffer() {
    if(trigger_glInvalidateSubFramebuffer) return;
    trigger_glInvalidateSubFramebuffer = true;
    printf("Stub: glInvalidateSubFramebuffer\n");
}
static bool trigger_glMultiDrawArraysIndirect = false;
void stub_glMultiDrawArraysIndirect() {
    if(trigger_glMultiDrawArraysIndirect) return;
    trigger_glMultiDrawArraysIndirect = true;
    printf("Stub: glMultiDrawArraysIndirect\n");
}
static bool trigger_glMultiDrawElementsIndirect = false;
void stub_glMultiDrawElementsIndirect() {
    if(trigger_glMultiDrawElementsIndirect) return;
    trigger_glMultiDrawElementsIndirect = true;
    printf("Stub: glMultiDrawElementsIndirect\n");
}
static bool trigger_glGetProgramInterfaceiv = false;
void stub_glGetProgramInterfaceiv() {
    if(trigger_glGetProgramInterfaceiv) return;
    trigger_glGetProgramInterfaceiv = true;
    printf("Stub: glGetProgramInterfaceiv\n");
}
static bool trigger_glGetProgramResourceIndex = false;
void stub_glGetProgramResourceIndex() {
    if(trigger_glGetProgramResourceIndex) return;
    trigger_glGetProgramResourceIndex = true;
    printf("Stub: glGetProgramResourceIndex\n");
}
static bool trigger_glGetProgramResourceName = false;
void stub_glGetProgramResourceName() {
    if(trigger_glGetProgramResourceName) return;
    trigger_glGetProgramResourceName = true;
    printf("Stub: glGetProgramResourceName\n");
}
static bool trigger_glGetProgramResourceiv = false;
void stub_glGetProgramResourceiv() {
    if(trigger_glGetProgramResourceiv) return;
    trigger_glGetProgramResourceiv = true;
    printf("Stub: glGetProgramResourceiv\n");
}
static bool trigger_glGetProgramResourceLocation = false;
void stub_glGetProgramResourceLocation() {
    if(trigger_glGetProgramResourceLocation) return;
    trigger_glGetProgramResourceLocation = true;
    printf("Stub: glGetProgramResourceLocation\n");
}
static bool trigger_glGetProgramResourceLocationIndex = false;
void stub_glGetProgramResourceLocationIndex() {
    if(trigger_glGetProgramResourceLocationIndex) return;
    trigger_glGetProgramResourceLocationIndex = true;
    printf("Stub: glGetProgramResourceLocationIndex\n");
}
static bool trigger_glShaderStorageBlockBinding = false;
void stub_glShaderStorageBlockBinding() {
    if(trigger_glShaderStorageBlockBinding) return;
    trigger_glShaderStorageBlockBinding = true;
    printf("Stub: glShaderStorageBlockBinding\n");
}
static bool trigger_glTexBufferRange = false;
void stub_glTexBufferRange() {
    if(trigger_glTexBufferRange) return;
    trigger_glTexBufferRange = true;
    printf("Stub: glTexBufferRange\n");
}
static bool trigger_glTexStorage2DMultisample = false;
void stub_glTexStorage2DMultisample() {
    if(trigger_glTexStorage2DMultisample) return;
    trigger_glTexStorage2DMultisample = true;
    printf("Stub: glTexStorage2DMultisample\n");
}
static bool trigger_glTexStorage3DMultisample = false;
void stub_glTexStorage3DMultisample() {
    if(trigger_glTexStorage3DMultisample) return;
    trigger_glTexStorage3DMultisample = true;
    printf("Stub: glTexStorage3DMultisample\n");
}
static bool trigger_glTextureView = false;
void stub_glTextureView() {
    if(trigger_glTextureView) return;
    trigger_glTextureView = true;
    printf("Stub: glTextureView\n");
}
static bool trigger_glBindVertexBuffer = false;
void stub_glBindVertexBuffer() {
    if(trigger_glBindVertexBuffer) return;
    trigger_glBindVertexBuffer = true;
    printf("Stub: glBindVertexBuffer\n");
}
static bool trigger_glVertexAttribFormat = false;
void stub_glVertexAttribFormat() {
    if(trigger_glVertexAttribFormat) return;
    trigger_glVertexAttribFormat = true;
    printf("Stub: glVertexAttribFormat\n");
}
static bool trigger_glVertexAttribIFormat = false;
void stub_glVertexAttribIFormat() {
    if(trigger_glVertexAttribIFormat) return;
    trigger_glVertexAttribIFormat = true;
    printf("Stub: glVertexAttribIFormat\n");
}
static bool trigger_glVertexAttribLFormat = false;
void stub_glVertexAttribLFormat() {
    if(trigger_glVertexAttribLFormat) return;
    trigger_glVertexAttribLFormat = true;
    printf("Stub: glVertexAttribLFormat\n");
}
static bool trigger_glVertexAttribBinding = false;
void stub_glVertexAttribBinding() {
    if(trigger_glVertexAttribBinding) return;
    trigger_glVertexAttribBinding = true;
    printf("Stub: glVertexAttribBinding\n");
}
static bool trigger_glVertexBindingDivisor = false;
void stub_glVertexBindingDivisor() {
    if(trigger_glVertexBindingDivisor) return;
    trigger_glVertexBindingDivisor = true;
    printf("Stub: glVertexBindingDivisor\n");
}
static bool trigger_glDebugMessageControl = false;
void stub_glDebugMessageControl() {
    if(trigger_glDebugMessageControl) return;
    trigger_glDebugMessageControl = true;
    printf("Stub: glDebugMessageControl\n");
}
static bool trigger_glDebugMessageInsert = false;
void stub_glDebugMessageInsert() {
    if(trigger_glDebugMessageInsert) return;
    trigger_glDebugMessageInsert = true;
    printf("Stub: glDebugMessageInsert\n");
}
static bool trigger_glDebugMessageCallback = false;
void stub_glDebugMessageCallback() {
    if(trigger_glDebugMessageCallback) return;
    trigger_glDebugMessageCallback = true;
    printf("Stub: glDebugMessageCallback\n");
}
static bool trigger_glGetDebugMessageLog = false;
void stub_glGetDebugMessageLog() {
    if(trigger_glGetDebugMessageLog) return;
    trigger_glGetDebugMessageLog = true;
    printf("Stub: glGetDebugMessageLog\n");
}
static bool trigger_glPushDebugGroup = false;
void stub_glPushDebugGroup() {
    if(trigger_glPushDebugGroup) return;
    trigger_glPushDebugGroup = true;
    printf("Stub: glPushDebugGroup\n");
}
static bool trigger_glPopDebugGroup = false;
void stub_glPopDebugGroup() {
    if(trigger_glPopDebugGroup) return;
    trigger_glPopDebugGroup = true;
    printf("Stub: glPopDebugGroup\n");
}
static bool trigger_glObjectLabel = false;
void stub_glObjectLabel() {
    if(trigger_glObjectLabel) return;
    trigger_glObjectLabel = true;
    printf("Stub: glObjectLabel\n");
}
static bool trigger_glGetObjectLabel = false;
void stub_glGetObjectLabel() {
    if(trigger_glGetObjectLabel) return;
    trigger_glGetObjectLabel = true;
    printf("Stub: glGetObjectLabel\n");
}
static bool trigger_glObjectPtrLabel = false;
void stub_glObjectPtrLabel() {
    if(trigger_glObjectPtrLabel) return;
    trigger_glObjectPtrLabel = true;
    printf("Stub: glObjectPtrLabel\n");
}
static bool trigger_glGetObjectPtrLabel = false;
void stub_glGetObjectPtrLabel() {
    if(trigger_glGetObjectPtrLabel) return;
    trigger_glGetObjectPtrLabel = true;
    printf("Stub: glGetObjectPtrLabel\n");
}
static bool trigger_glBufferStorage = false;
void stub_glBufferStorage() {
    if(trigger_glBufferStorage) return;
    trigger_glBufferStorage = true;
    printf("Stub: glBufferStorage\n");
}
static bool trigger_glClearTexImage = false;
void stub_glClearTexImage() {
    if(trigger_glClearTexImage) return;
    trigger_glClearTexImage = true;
    printf("Stub: glClearTexImage\n");
}
static bool trigger_glClearTexSubImage = false;
void stub_glClearTexSubImage() {
    if(trigger_glClearTexSubImage) return;
    trigger_glClearTexSubImage = true;
    printf("Stub: glClearTexSubImage\n");
}
static bool trigger_glBindBuffersBase = false;
void stub_glBindBuffersBase() {
    if(trigger_glBindBuffersBase) return;
    trigger_glBindBuffersBase = true;
    printf("Stub: glBindBuffersBase\n");
}
static bool trigger_glBindBuffersRange = false;
void stub_glBindBuffersRange() {
    if(trigger_glBindBuffersRange) return;
    trigger_glBindBuffersRange = true;
    printf("Stub: glBindBuffersRange\n");
}
static bool trigger_glBindTextures = false;
void stub_glBindTextures() {
    if(trigger_glBindTextures) return;
    trigger_glBindTextures = true;
    printf("Stub: glBindTextures\n");
}
static bool trigger_glBindSamplers = false;
void stub_glBindSamplers() {
    if(trigger_glBindSamplers) return;
    trigger_glBindSamplers = true;
    printf("Stub: glBindSamplers\n");
}
static bool trigger_glBindImageTextures = false;
void stub_glBindImageTextures() {
    if(trigger_glBindImageTextures) return;
    trigger_glBindImageTextures = true;
    printf("Stub: glBindImageTextures\n");
}
static bool trigger_glBindVertexBuffers = false;
void stub_glBindVertexBuffers() {
    if(trigger_glBindVertexBuffers) return;
    trigger_glBindVertexBuffers = true;
    printf("Stub: glBindVertexBuffers\n");
}
static bool trigger_glClipControl = false;
void stub_glClipControl() {
    if(trigger_glClipControl) return;
    trigger_glClipControl = true;
    printf("Stub: glClipControl\n");
}
static bool trigger_glCreateTransformFeedbacks = false;
void stub_glCreateTransformFeedbacks() {
    if(trigger_glCreateTransformFeedbacks) return;
    trigger_glCreateTransformFeedbacks = true;
    printf("Stub: glCreateTransformFeedbacks\n");
}
static bool trigger_glTransformFeedbackBufferBase = false;
void stub_glTransformFeedbackBufferBase() {
    if(trigger_glTransformFeedbackBufferBase) return;
    trigger_glTransformFeedbackBufferBase = true;
    printf("Stub: glTransformFeedbackBufferBase\n");
}
static bool trigger_glTransformFeedbackBufferRange = false;
void stub_glTransformFeedbackBufferRange() {
    if(trigger_glTransformFeedbackBufferRange) return;
    trigger_glTransformFeedbackBufferRange = true;
    printf("Stub: glTransformFeedbackBufferRange\n");
}
static bool trigger_glGetTransformFeedbackiv = false;
void stub_glGetTransformFeedbackiv() {
    if(trigger_glGetTransformFeedbackiv) return;
    trigger_glGetTransformFeedbackiv = true;
    printf("Stub: glGetTransformFeedbackiv\n");
}
static bool trigger_glGetTransformFeedbacki_v = false;
void stub_glGetTransformFeedbacki_v() {
    if(trigger_glGetTransformFeedbacki_v) return;
    trigger_glGetTransformFeedbacki_v = true;
    printf("Stub: glGetTransformFeedbacki_v\n");
}
static bool trigger_glGetTransformFeedbacki64_v = false;
void stub_glGetTransformFeedbacki64_v() {
    if(trigger_glGetTransformFeedbacki64_v) return;
    trigger_glGetTransformFeedbacki64_v = true;
    printf("Stub: glGetTransformFeedbacki64_v\n");
}
static bool trigger_glCreateBuffers = false;
void stub_glCreateBuffers() {
    if(trigger_glCreateBuffers) return;
    trigger_glCreateBuffers = true;
    printf("Stub: glCreateBuffers\n");
}
static bool trigger_glNamedBufferStorage = false;
void stub_glNamedBufferStorage() {
    if(trigger_glNamedBufferStorage) return;
    trigger_glNamedBufferStorage = true;
    printf("Stub: glNamedBufferStorage\n");
}
static bool trigger_glNamedBufferData = false;
void stub_glNamedBufferData() {
    if(trigger_glNamedBufferData) return;
    trigger_glNamedBufferData = true;
    printf("Stub: glNamedBufferData\n");
}
static bool trigger_glNamedBufferSubData = false;
void stub_glNamedBufferSubData() {
    if(trigger_glNamedBufferSubData) return;
    trigger_glNamedBufferSubData = true;
    printf("Stub: glNamedBufferSubData\n");
}
static bool trigger_glCopyNamedBufferSubData = false;
void stub_glCopyNamedBufferSubData() {
    if(trigger_glCopyNamedBufferSubData) return;
    trigger_glCopyNamedBufferSubData = true;
    printf("Stub: glCopyNamedBufferSubData\n");
}
static bool trigger_glClearNamedBufferData = false;
void stub_glClearNamedBufferData() {
    if(trigger_glClearNamedBufferData) return;
    trigger_glClearNamedBufferData = true;
    printf("Stub: glClearNamedBufferData\n");
}
static bool trigger_glClearNamedBufferSubData = false;
void stub_glClearNamedBufferSubData() {
    if(trigger_glClearNamedBufferSubData) return;
    trigger_glClearNamedBufferSubData = true;
    printf("Stub: glClearNamedBufferSubData\n");
}
static bool trigger_glMapNamedBuffer = false;
void stub_glMapNamedBuffer() {
    if(trigger_glMapNamedBuffer) return;
    trigger_glMapNamedBuffer = true;
    printf("Stub: glMapNamedBuffer\n");
}
static bool trigger_glMapNamedBufferRange = false;
void stub_glMapNamedBufferRange() {
    if(trigger_glMapNamedBufferRange) return;
    trigger_glMapNamedBufferRange = true;
    printf("Stub: glMapNamedBufferRange\n");
}
static bool trigger_glUnmapNamedBuffer = false;
void stub_glUnmapNamedBuffer() {
    if(trigger_glUnmapNamedBuffer) return;
    trigger_glUnmapNamedBuffer = true;
    printf("Stub: glUnmapNamedBuffer\n");
}
static bool trigger_glFlushMappedNamedBufferRange = false;
void stub_glFlushMappedNamedBufferRange() {
    if(trigger_glFlushMappedNamedBufferRange) return;
    trigger_glFlushMappedNamedBufferRange = true;
    printf("Stub: glFlushMappedNamedBufferRange\n");
}
static bool trigger_glGetNamedBufferParameteriv = false;
void stub_glGetNamedBufferParameteriv() {
    if(trigger_glGetNamedBufferParameteriv) return;
    trigger_glGetNamedBufferParameteriv = true;
    printf("Stub: glGetNamedBufferParameteriv\n");
}
static bool trigger_glGetNamedBufferParameteri64v = false;
void stub_glGetNamedBufferParameteri64v() {
    if(trigger_glGetNamedBufferParameteri64v) return;
    trigger_glGetNamedBufferParameteri64v = true;
    printf("Stub: glGetNamedBufferParameteri64v\n");
}
static bool trigger_glGetNamedBufferPointerv = false;
void stub_glGetNamedBufferPointerv() {
    if(trigger_glGetNamedBufferPointerv) return;
    trigger_glGetNamedBufferPointerv = true;
    printf("Stub: glGetNamedBufferPointerv\n");
}
static bool trigger_glGetNamedBufferSubData = false;
void stub_glGetNamedBufferSubData() {
    if(trigger_glGetNamedBufferSubData) return;
    trigger_glGetNamedBufferSubData = true;
    printf("Stub: glGetNamedBufferSubData\n");
}
static bool trigger_glCreateFramebuffers = false;
void stub_glCreateFramebuffers() {
    if(trigger_glCreateFramebuffers) return;
    trigger_glCreateFramebuffers = true;
    printf("Stub: glCreateFramebuffers\n");
}
static bool trigger_glNamedFramebufferRenderbuffer = false;
void stub_glNamedFramebufferRenderbuffer() {
    if(trigger_glNamedFramebufferRenderbuffer) return;
    trigger_glNamedFramebufferRenderbuffer = true;
    printf("Stub: glNamedFramebufferRenderbuffer\n");
}
static bool trigger_glNamedFramebufferParameteri = false;
void stub_glNamedFramebufferParameteri() {
    if(trigger_glNamedFramebufferParameteri) return;
    trigger_glNamedFramebufferParameteri = true;
    printf("Stub: glNamedFramebufferParameteri\n");
}
static bool trigger_glNamedFramebufferTexture = false;
void stub_glNamedFramebufferTexture() {
    if(trigger_glNamedFramebufferTexture) return;
    trigger_glNamedFramebufferTexture = true;
    printf("Stub: glNamedFramebufferTexture\n");
}
static bool trigger_glNamedFramebufferTextureLayer = false;
void stub_glNamedFramebufferTextureLayer() {
    if(trigger_glNamedFramebufferTextureLayer) return;
    trigger_glNamedFramebufferTextureLayer = true;
    printf("Stub: glNamedFramebufferTextureLayer\n");
}
static bool trigger_glNamedFramebufferDrawBuffer = false;
void stub_glNamedFramebufferDrawBuffer() {
    if(trigger_glNamedFramebufferDrawBuffer) return;
    trigger_glNamedFramebufferDrawBuffer = true;
    printf("Stub: glNamedFramebufferDrawBuffer\n");
}
static bool trigger_glNamedFramebufferDrawBuffers = false;
void stub_glNamedFramebufferDrawBuffers() {
    if(trigger_glNamedFramebufferDrawBuffers) return;
    trigger_glNamedFramebufferDrawBuffers = true;
    printf("Stub: glNamedFramebufferDrawBuffers\n");
}
static bool trigger_glNamedFramebufferReadBuffer = false;
void stub_glNamedFramebufferReadBuffer() {
    if(trigger_glNamedFramebufferReadBuffer) return;
    trigger_glNamedFramebufferReadBuffer = true;
    printf("Stub: glNamedFramebufferReadBuffer\n");
}
static bool trigger_glInvalidateNamedFramebufferData = false;
void stub_glInvalidateNamedFramebufferData() {
    if(trigger_glInvalidateNamedFramebufferData) return;
    trigger_glInvalidateNamedFramebufferData = true;
    printf("Stub: glInvalidateNamedFramebufferData\n");
}
static bool trigger_glInvalidateNamedFramebufferSubData = false;
void stub_glInvalidateNamedFramebufferSubData() {
    if(trigger_glInvalidateNamedFramebufferSubData) return;
    trigger_glInvalidateNamedFramebufferSubData = true;
    printf("Stub: glInvalidateNamedFramebufferSubData\n");
}
static bool trigger_glClearNamedFramebufferiv = false;
void stub_glClearNamedFramebufferiv() {
    if(trigger_glClearNamedFramebufferiv) return;
    trigger_glClearNamedFramebufferiv = true;
    printf("Stub: glClearNamedFramebufferiv\n");
}
static bool trigger_glClearNamedFramebufferuiv = false;
void stub_glClearNamedFramebufferuiv() {
    if(trigger_glClearNamedFramebufferuiv) return;
    trigger_glClearNamedFramebufferuiv = true;
    printf("Stub: glClearNamedFramebufferuiv\n");
}
static bool trigger_glClearNamedFramebufferfv = false;
void stub_glClearNamedFramebufferfv() {
    if(trigger_glClearNamedFramebufferfv) return;
    trigger_glClearNamedFramebufferfv = true;
    printf("Stub: glClearNamedFramebufferfv\n");
}
static bool trigger_glClearNamedFramebufferfi = false;
void stub_glClearNamedFramebufferfi() {
    if(trigger_glClearNamedFramebufferfi) return;
    trigger_glClearNamedFramebufferfi = true;
    printf("Stub: glClearNamedFramebufferfi\n");
}
static bool trigger_glBlitNamedFramebuffer = false;
void stub_glBlitNamedFramebuffer() {
    if(trigger_glBlitNamedFramebuffer) return;
    trigger_glBlitNamedFramebuffer = true;
    printf("Stub: glBlitNamedFramebuffer\n");
}
static bool trigger_glCheckNamedFramebufferStatus = false;
void stub_glCheckNamedFramebufferStatus() {
    if(trigger_glCheckNamedFramebufferStatus) return;
    trigger_glCheckNamedFramebufferStatus = true;
    printf("Stub: glCheckNamedFramebufferStatus\n");
}
static bool trigger_glGetNamedFramebufferParameteriv = false;
void stub_glGetNamedFramebufferParameteriv() {
    if(trigger_glGetNamedFramebufferParameteriv) return;
    trigger_glGetNamedFramebufferParameteriv = true;
    printf("Stub: glGetNamedFramebufferParameteriv\n");
}
static bool trigger_glGetNamedFramebufferAttachmentParameteriv = false;
void stub_glGetNamedFramebufferAttachmentParameteriv() {
    if(trigger_glGetNamedFramebufferAttachmentParameteriv) return;
    trigger_glGetNamedFramebufferAttachmentParameteriv = true;
    printf("Stub: glGetNamedFramebufferAttachmentParameteriv\n");
}
static bool trigger_glCreateRenderbuffers = false;
void stub_glCreateRenderbuffers() {
    if(trigger_glCreateRenderbuffers) return;
    trigger_glCreateRenderbuffers = true;
    printf("Stub: glCreateRenderbuffers\n");
}
static bool trigger_glNamedRenderbufferStorage = false;
void stub_glNamedRenderbufferStorage() {
    if(trigger_glNamedRenderbufferStorage) return;
    trigger_glNamedRenderbufferStorage = true;
    printf("Stub: glNamedRenderbufferStorage\n");
}
static bool trigger_glNamedRenderbufferStorageMultisample = false;
void stub_glNamedRenderbufferStorageMultisample() {
    if(trigger_glNamedRenderbufferStorageMultisample) return;
    trigger_glNamedRenderbufferStorageMultisample = true;
    printf("Stub: glNamedRenderbufferStorageMultisample\n");
}
static bool trigger_glGetNamedRenderbufferParameteriv = false;
void stub_glGetNamedRenderbufferParameteriv() {
    if(trigger_glGetNamedRenderbufferParameteriv) return;
    trigger_glGetNamedRenderbufferParameteriv = true;
    printf("Stub: glGetNamedRenderbufferParameteriv\n");
}
static bool trigger_glCreateTextures = false;
void stub_glCreateTextures() {
    if(trigger_glCreateTextures) return;
    trigger_glCreateTextures = true;
    printf("Stub: glCreateTextures\n");
}
static bool trigger_glTextureBuffer = false;
void stub_glTextureBuffer() {
    if(trigger_glTextureBuffer) return;
    trigger_glTextureBuffer = true;
    printf("Stub: glTextureBuffer\n");
}
static bool trigger_glTextureBufferRange = false;
void stub_glTextureBufferRange() {
    if(trigger_glTextureBufferRange) return;
    trigger_glTextureBufferRange = true;
    printf("Stub: glTextureBufferRange\n");
}
static bool trigger_glTextureStorage1D = false;
void stub_glTextureStorage1D() {
    if(trigger_glTextureStorage1D) return;
    trigger_glTextureStorage1D = true;
    printf("Stub: glTextureStorage1D\n");
}
static bool trigger_glTextureStorage2D = false;
void stub_glTextureStorage2D() {
    if(trigger_glTextureStorage2D) return;
    trigger_glTextureStorage2D = true;
    printf("Stub: glTextureStorage2D\n");
}
static bool trigger_glTextureStorage3D = false;
void stub_glTextureStorage3D() {
    if(trigger_glTextureStorage3D) return;
    trigger_glTextureStorage3D = true;
    printf("Stub: glTextureStorage3D\n");
}
static bool trigger_glTextureStorage2DMultisample = false;
void stub_glTextureStorage2DMultisample() {
    if(trigger_glTextureStorage2DMultisample) return;
    trigger_glTextureStorage2DMultisample = true;
    printf("Stub: glTextureStorage2DMultisample\n");
}
static bool trigger_glTextureStorage3DMultisample = false;
void stub_glTextureStorage3DMultisample() {
    if(trigger_glTextureStorage3DMultisample) return;
    trigger_glTextureStorage3DMultisample = true;
    printf("Stub: glTextureStorage3DMultisample\n");
}
static bool trigger_glTextureSubImage1D = false;
void stub_glTextureSubImage1D() {
    if(trigger_glTextureSubImage1D) return;
    trigger_glTextureSubImage1D = true;
    printf("Stub: glTextureSubImage1D\n");
}
static bool trigger_glTextureSubImage2D = false;
void stub_glTextureSubImage2D() {
    if(trigger_glTextureSubImage2D) return;
    trigger_glTextureSubImage2D = true;
    printf("Stub: glTextureSubImage2D\n");
}
static bool trigger_glTextureSubImage3D = false;
void stub_glTextureSubImage3D() {
    if(trigger_glTextureSubImage3D) return;
    trigger_glTextureSubImage3D = true;
    printf("Stub: glTextureSubImage3D\n");
}
static bool trigger_glCompressedTextureSubImage1D = false;
void stub_glCompressedTextureSubImage1D() {
    if(trigger_glCompressedTextureSubImage1D) return;
    trigger_glCompressedTextureSubImage1D = true;
    printf("Stub: glCompressedTextureSubImage1D\n");
}
static bool trigger_glCompressedTextureSubImage2D = false;
void stub_glCompressedTextureSubImage2D() {
    if(trigger_glCompressedTextureSubImage2D) return;
    trigger_glCompressedTextureSubImage2D = true;
    printf("Stub: glCompressedTextureSubImage2D\n");
}
static bool trigger_glCompressedTextureSubImage3D = false;
void stub_glCompressedTextureSubImage3D() {
    if(trigger_glCompressedTextureSubImage3D) return;
    trigger_glCompressedTextureSubImage3D = true;
    printf("Stub: glCompressedTextureSubImage3D\n");
}
static bool trigger_glCopyTextureSubImage1D = false;
void stub_glCopyTextureSubImage1D() {
    if(trigger_glCopyTextureSubImage1D) return;
    trigger_glCopyTextureSubImage1D = true;
    printf("Stub: glCopyTextureSubImage1D\n");
}
static bool trigger_glCopyTextureSubImage2D = false;
void stub_glCopyTextureSubImage2D() {
    if(trigger_glCopyTextureSubImage2D) return;
    trigger_glCopyTextureSubImage2D = true;
    printf("Stub: glCopyTextureSubImage2D\n");
}
static bool trigger_glCopyTextureSubImage3D = false;
void stub_glCopyTextureSubImage3D() {
    if(trigger_glCopyTextureSubImage3D) return;
    trigger_glCopyTextureSubImage3D = true;
    printf("Stub: glCopyTextureSubImage3D\n");
}
static bool trigger_glTextureParameterf = false;
void stub_glTextureParameterf() {
    if(trigger_glTextureParameterf) return;
    trigger_glTextureParameterf = true;
    printf("Stub: glTextureParameterf\n");
}
static bool trigger_glTextureParameterfv = false;
void stub_glTextureParameterfv() {
    if(trigger_glTextureParameterfv) return;
    trigger_glTextureParameterfv = true;
    printf("Stub: glTextureParameterfv\n");
}
static bool trigger_glTextureParameteri = false;
void stub_glTextureParameteri() {
    if(trigger_glTextureParameteri) return;
    trigger_glTextureParameteri = true;
    printf("Stub: glTextureParameteri\n");
}
static bool trigger_glTextureParameterIiv = false;
void stub_glTextureParameterIiv() {
    if(trigger_glTextureParameterIiv) return;
    trigger_glTextureParameterIiv = true;
    printf("Stub: glTextureParameterIiv\n");
}
static bool trigger_glTextureParameterIuiv = false;
void stub_glTextureParameterIuiv() {
    if(trigger_glTextureParameterIuiv) return;
    trigger_glTextureParameterIuiv = true;
    printf("Stub: glTextureParameterIuiv\n");
}
static bool trigger_glTextureParameteriv = false;
void stub_glTextureParameteriv() {
    if(trigger_glTextureParameteriv) return;
    trigger_glTextureParameteriv = true;
    printf("Stub: glTextureParameteriv\n");
}
static bool trigger_glGenerateTextureMipmap = false;
void stub_glGenerateTextureMipmap() {
    if(trigger_glGenerateTextureMipmap) return;
    trigger_glGenerateTextureMipmap = true;
    printf("Stub: glGenerateTextureMipmap\n");
}
static bool trigger_glBindTextureUnit = false;
void stub_glBindTextureUnit() {
    if(trigger_glBindTextureUnit) return;
    trigger_glBindTextureUnit = true;
    printf("Stub: glBindTextureUnit\n");
}
static bool trigger_glGetTextureImage = false;
void stub_glGetTextureImage() {
    if(trigger_glGetTextureImage) return;
    trigger_glGetTextureImage = true;
    printf("Stub: glGetTextureImage\n");
}
static bool trigger_glGetCompressedTextureImage = false;
void stub_glGetCompressedTextureImage() {
    if(trigger_glGetCompressedTextureImage) return;
    trigger_glGetCompressedTextureImage = true;
    printf("Stub: glGetCompressedTextureImage\n");
}
static bool trigger_glGetTextureLevelParameterfv = false;
void stub_glGetTextureLevelParameterfv() {
    if(trigger_glGetTextureLevelParameterfv) return;
    trigger_glGetTextureLevelParameterfv = true;
    printf("Stub: glGetTextureLevelParameterfv\n");
}
static bool trigger_glGetTextureLevelParameteriv = false;
void stub_glGetTextureLevelParameteriv() {
    if(trigger_glGetTextureLevelParameteriv) return;
    trigger_glGetTextureLevelParameteriv = true;
    printf("Stub: glGetTextureLevelParameteriv\n");
}
static bool trigger_glGetTextureParameterfv = false;
void stub_glGetTextureParameterfv() {
    if(trigger_glGetTextureParameterfv) return;
    trigger_glGetTextureParameterfv = true;
    printf("Stub: glGetTextureParameterfv\n");
}
static bool trigger_glGetTextureParameterIiv = false;
void stub_glGetTextureParameterIiv() {
    if(trigger_glGetTextureParameterIiv) return;
    trigger_glGetTextureParameterIiv = true;
    printf("Stub: glGetTextureParameterIiv\n");
}
static bool trigger_glGetTextureParameterIuiv = false;
void stub_glGetTextureParameterIuiv() {
    if(trigger_glGetTextureParameterIuiv) return;
    trigger_glGetTextureParameterIuiv = true;
    printf("Stub: glGetTextureParameterIuiv\n");
}
static bool trigger_glGetTextureParameteriv = false;
void stub_glGetTextureParameteriv() {
    if(trigger_glGetTextureParameteriv) return;
    trigger_glGetTextureParameteriv = true;
    printf("Stub: glGetTextureParameteriv\n");
}
static bool trigger_glCreateVertexArrays = false;
void stub_glCreateVertexArrays() {
    if(trigger_glCreateVertexArrays) return;
    trigger_glCreateVertexArrays = true;
    printf("Stub: glCreateVertexArrays\n");
}
static bool trigger_glDisableVertexArrayAttrib = false;
void stub_glDisableVertexArrayAttrib() {
    if(trigger_glDisableVertexArrayAttrib) return;
    trigger_glDisableVertexArrayAttrib = true;
    printf("Stub: glDisableVertexArrayAttrib\n");
}
static bool trigger_glEnableVertexArrayAttrib = false;
void stub_glEnableVertexArrayAttrib() {
    if(trigger_glEnableVertexArrayAttrib) return;
    trigger_glEnableVertexArrayAttrib = true;
    printf("Stub: glEnableVertexArrayAttrib\n");
}
static bool trigger_glVertexArrayElementBuffer = false;
void stub_glVertexArrayElementBuffer() {
    if(trigger_glVertexArrayElementBuffer) return;
    trigger_glVertexArrayElementBuffer = true;
    printf("Stub: glVertexArrayElementBuffer\n");
}
static bool trigger_glVertexArrayVertexBuffer = false;
void stub_glVertexArrayVertexBuffer() {
    if(trigger_glVertexArrayVertexBuffer) return;
    trigger_glVertexArrayVertexBuffer = true;
    printf("Stub: glVertexArrayVertexBuffer\n");
}
static bool trigger_glVertexArrayVertexBuffers = false;
void stub_glVertexArrayVertexBuffers() {
    if(trigger_glVertexArrayVertexBuffers) return;
    trigger_glVertexArrayVertexBuffers = true;
    printf("Stub: glVertexArrayVertexBuffers\n");
}
static bool trigger_glVertexArrayAttribBinding = false;
void stub_glVertexArrayAttribBinding() {
    if(trigger_glVertexArrayAttribBinding) return;
    trigger_glVertexArrayAttribBinding = true;
    printf("Stub: glVertexArrayAttribBinding\n");
}
static bool trigger_glVertexArrayAttribFormat = false;
void stub_glVertexArrayAttribFormat() {
    if(trigger_glVertexArrayAttribFormat) return;
    trigger_glVertexArrayAttribFormat = true;
    printf("Stub: glVertexArrayAttribFormat\n");
}
static bool trigger_glVertexArrayAttribIFormat = false;
void stub_glVertexArrayAttribIFormat() {
    if(trigger_glVertexArrayAttribIFormat) return;
    trigger_glVertexArrayAttribIFormat = true;
    printf("Stub: glVertexArrayAttribIFormat\n");
}
static bool trigger_glVertexArrayAttribLFormat = false;
void stub_glVertexArrayAttribLFormat() {
    if(trigger_glVertexArrayAttribLFormat) return;
    trigger_glVertexArrayAttribLFormat = true;
    printf("Stub: glVertexArrayAttribLFormat\n");
}
static bool trigger_glVertexArrayBindingDivisor = false;
void stub_glVertexArrayBindingDivisor() {
    if(trigger_glVertexArrayBindingDivisor) return;
    trigger_glVertexArrayBindingDivisor = true;
    printf("Stub: glVertexArrayBindingDivisor\n");
}
static bool trigger_glGetVertexArrayiv = false;
void stub_glGetVertexArrayiv() {
    if(trigger_glGetVertexArrayiv) return;
    trigger_glGetVertexArrayiv = true;
    printf("Stub: glGetVertexArrayiv\n");
}
static bool trigger_glGetVertexArrayIndexediv = false;
void stub_glGetVertexArrayIndexediv() {
    if(trigger_glGetVertexArrayIndexediv) return;
    trigger_glGetVertexArrayIndexediv = true;
    printf("Stub: glGetVertexArrayIndexediv\n");
}
static bool trigger_glGetVertexArrayIndexed64iv = false;
void stub_glGetVertexArrayIndexed64iv() {
    if(trigger_glGetVertexArrayIndexed64iv) return;
    trigger_glGetVertexArrayIndexed64iv = true;
    printf("Stub: glGetVertexArrayIndexed64iv\n");
}
static bool trigger_glCreateSamplers = false;
void stub_glCreateSamplers() {
    if(trigger_glCreateSamplers) return;
    trigger_glCreateSamplers = true;
    printf("Stub: glCreateSamplers\n");
}
static bool trigger_glCreateProgramPipelines = false;
void stub_glCreateProgramPipelines() {
    if(trigger_glCreateProgramPipelines) return;
    trigger_glCreateProgramPipelines = true;
    printf("Stub: glCreateProgramPipelines\n");
}
static bool trigger_glCreateQueries = false;
void stub_glCreateQueries() {
    if(trigger_glCreateQueries) return;
    trigger_glCreateQueries = true;
    printf("Stub: glCreateQueries\n");
}
static bool trigger_glGetQueryBufferObjecti64v = false;
void stub_glGetQueryBufferObjecti64v() {
    if(trigger_glGetQueryBufferObjecti64v) return;
    trigger_glGetQueryBufferObjecti64v = true;
    printf("Stub: glGetQueryBufferObjecti64v\n");
}
static bool trigger_glGetQueryBufferObjectiv = false;
void stub_glGetQueryBufferObjectiv() {
    if(trigger_glGetQueryBufferObjectiv) return;
    trigger_glGetQueryBufferObjectiv = true;
    printf("Stub: glGetQueryBufferObjectiv\n");
}
static bool trigger_glGetQueryBufferObjectui64v = false;
void stub_glGetQueryBufferObjectui64v() {
    if(trigger_glGetQueryBufferObjectui64v) return;
    trigger_glGetQueryBufferObjectui64v = true;
    printf("Stub: glGetQueryBufferObjectui64v\n");
}
static bool trigger_glGetQueryBufferObjectuiv = false;
void stub_glGetQueryBufferObjectuiv() {
    if(trigger_glGetQueryBufferObjectuiv) return;
    trigger_glGetQueryBufferObjectuiv = true;
    printf("Stub: glGetQueryBufferObjectuiv\n");
}
static bool trigger_glMemoryBarrierByRegion = false;
void stub_glMemoryBarrierByRegion() {
    if(trigger_glMemoryBarrierByRegion) return;
    trigger_glMemoryBarrierByRegion = true;
    printf("Stub: glMemoryBarrierByRegion\n");
}
static bool trigger_glGetTextureSubImage = false;
void stub_glGetTextureSubImage() {
    if(trigger_glGetTextureSubImage) return;
    trigger_glGetTextureSubImage = true;
    printf("Stub: glGetTextureSubImage\n");
}
static bool trigger_glGetCompressedTextureSubImage = false;
void stub_glGetCompressedTextureSubImage() {
    if(trigger_glGetCompressedTextureSubImage) return;
    trigger_glGetCompressedTextureSubImage = true;
    printf("Stub: glGetCompressedTextureSubImage\n");
}
static bool trigger_glGetGraphicsResetStatus = false;
void stub_glGetGraphicsResetStatus() {
    if(trigger_glGetGraphicsResetStatus) return;
    trigger_glGetGraphicsResetStatus = true;
    printf("Stub: glGetGraphicsResetStatus\n");
}
static bool trigger_glGetnCompressedTexImage = false;
void stub_glGetnCompressedTexImage() {
    if(trigger_glGetnCompressedTexImage) return;
    trigger_glGetnCompressedTexImage = true;
    printf("Stub: glGetnCompressedTexImage\n");
}
static bool trigger_glGetnTexImage = false;
void stub_glGetnTexImage() {
    if(trigger_glGetnTexImage) return;
    trigger_glGetnTexImage = true;
    printf("Stub: glGetnTexImage\n");
}
static bool trigger_glGetnUniformdv = false;
void stub_glGetnUniformdv() {
    if(trigger_glGetnUniformdv) return;
    trigger_glGetnUniformdv = true;
    printf("Stub: glGetnUniformdv\n");
}
static bool trigger_glGetnUniformfv = false;
void stub_glGetnUniformfv() {
    if(trigger_glGetnUniformfv) return;
    trigger_glGetnUniformfv = true;
    printf("Stub: glGetnUniformfv\n");
}
static bool trigger_glGetnUniformiv = false;
void stub_glGetnUniformiv() {
    if(trigger_glGetnUniformiv) return;
    trigger_glGetnUniformiv = true;
    printf("Stub: glGetnUniformiv\n");
}
static bool trigger_glGetnUniformuiv = false;
void stub_glGetnUniformuiv() {
    if(trigger_glGetnUniformuiv) return;
    trigger_glGetnUniformuiv = true;
    printf("Stub: glGetnUniformuiv\n");
}
static bool trigger_glReadnPixels = false;
void stub_glReadnPixels() {
    if(trigger_glReadnPixels) return;
    trigger_glReadnPixels = true;
    printf("Stub: glReadnPixels\n");
}
static bool trigger_glTextureBarrier = false;
void stub_glTextureBarrier() {
    if(trigger_glTextureBarrier) return;
    trigger_glTextureBarrier = true;
    printf("Stub: glTextureBarrier\n");
}
static bool trigger_glSpecializeShader = false;
void stub_glSpecializeShader() {
    if(trigger_glSpecializeShader) return;
    trigger_glSpecializeShader = true;
    printf("Stub: glSpecializeShader\n");
}
static bool trigger_glMultiDrawArraysIndirectCount = false;
void stub_glMultiDrawArraysIndirectCount() {
    if(trigger_glMultiDrawArraysIndirectCount) return;
    trigger_glMultiDrawArraysIndirectCount = true;
    printf("Stub: glMultiDrawArraysIndirectCount\n");
}
static bool trigger_glMultiDrawElementsIndirectCount = false;
void stub_glMultiDrawElementsIndirectCount() {
    if(trigger_glMultiDrawElementsIndirectCount) return;
    trigger_glMultiDrawElementsIndirectCount = true;
    printf("Stub: glMultiDrawElementsIndirectCount\n");
}
static bool trigger_glPolygonOffsetClamp = false;
void stub_glPolygonOffsetClamp() {
    if(trigger_glPolygonOffsetClamp) return;
    trigger_glPolygonOffsetClamp = true;
    printf("Stub: glPolygonOffsetClamp\n");
}
static bool trigger_glPrimitiveBoundingBoxARB = false;
void stub_glPrimitiveBoundingBoxARB() {
    if(trigger_glPrimitiveBoundingBoxARB) return;
    trigger_glPrimitiveBoundingBoxARB = true;
    printf("Stub: glPrimitiveBoundingBoxARB\n");
}
static bool trigger_glGetTextureHandleARB = false;
void stub_glGetTextureHandleARB() {
    if(trigger_glGetTextureHandleARB) return;
    trigger_glGetTextureHandleARB = true;
    printf("Stub: glGetTextureHandleARB\n");
}
static bool trigger_glGetTextureSamplerHandleARB = false;
void stub_glGetTextureSamplerHandleARB() {
    if(trigger_glGetTextureSamplerHandleARB) return;
    trigger_glGetTextureSamplerHandleARB = true;
    printf("Stub: glGetTextureSamplerHandleARB\n");
}
static bool trigger_glMakeTextureHandleResidentARB = false;
void stub_glMakeTextureHandleResidentARB() {
    if(trigger_glMakeTextureHandleResidentARB) return;
    trigger_glMakeTextureHandleResidentARB = true;
    printf("Stub: glMakeTextureHandleResidentARB\n");
}
static bool trigger_glMakeTextureHandleNonResidentARB = false;
void stub_glMakeTextureHandleNonResidentARB() {
    if(trigger_glMakeTextureHandleNonResidentARB) return;
    trigger_glMakeTextureHandleNonResidentARB = true;
    printf("Stub: glMakeTextureHandleNonResidentARB\n");
}
static bool trigger_glGetImageHandleARB = false;
void stub_glGetImageHandleARB() {
    if(trigger_glGetImageHandleARB) return;
    trigger_glGetImageHandleARB = true;
    printf("Stub: glGetImageHandleARB\n");
}
static bool trigger_glMakeImageHandleResidentARB = false;
void stub_glMakeImageHandleResidentARB() {
    if(trigger_glMakeImageHandleResidentARB) return;
    trigger_glMakeImageHandleResidentARB = true;
    printf("Stub: glMakeImageHandleResidentARB\n");
}
static bool trigger_glMakeImageHandleNonResidentARB = false;
void stub_glMakeImageHandleNonResidentARB() {
    if(trigger_glMakeImageHandleNonResidentARB) return;
    trigger_glMakeImageHandleNonResidentARB = true;
    printf("Stub: glMakeImageHandleNonResidentARB\n");
}
static bool trigger_glUniformHandleui64ARB = false;
void stub_glUniformHandleui64ARB() {
    if(trigger_glUniformHandleui64ARB) return;
    trigger_glUniformHandleui64ARB = true;
    printf("Stub: glUniformHandleui64ARB\n");
}
static bool trigger_glUniformHandleui64vARB = false;
void stub_glUniformHandleui64vARB() {
    if(trigger_glUniformHandleui64vARB) return;
    trigger_glUniformHandleui64vARB = true;
    printf("Stub: glUniformHandleui64vARB\n");
}
static bool trigger_glProgramUniformHandleui64ARB = false;
void stub_glProgramUniformHandleui64ARB() {
    if(trigger_glProgramUniformHandleui64ARB) return;
    trigger_glProgramUniformHandleui64ARB = true;
    printf("Stub: glProgramUniformHandleui64ARB\n");
}
static bool trigger_glProgramUniformHandleui64vARB = false;
void stub_glProgramUniformHandleui64vARB() {
    if(trigger_glProgramUniformHandleui64vARB) return;
    trigger_glProgramUniformHandleui64vARB = true;
    printf("Stub: glProgramUniformHandleui64vARB\n");
}
static bool trigger_glIsTextureHandleResidentARB = false;
void stub_glIsTextureHandleResidentARB() {
    if(trigger_glIsTextureHandleResidentARB) return;
    trigger_glIsTextureHandleResidentARB = true;
    printf("Stub: glIsTextureHandleResidentARB\n");
}
static bool trigger_glIsImageHandleResidentARB = false;
void stub_glIsImageHandleResidentARB() {
    if(trigger_glIsImageHandleResidentARB) return;
    trigger_glIsImageHandleResidentARB = true;
    printf("Stub: glIsImageHandleResidentARB\n");
}
static bool trigger_glVertexAttribL1ui64ARB = false;
void stub_glVertexAttribL1ui64ARB() {
    if(trigger_glVertexAttribL1ui64ARB) return;
    trigger_glVertexAttribL1ui64ARB = true;
    printf("Stub: glVertexAttribL1ui64ARB\n");
}
static bool trigger_glVertexAttribL1ui64vARB = false;
void stub_glVertexAttribL1ui64vARB() {
    if(trigger_glVertexAttribL1ui64vARB) return;
    trigger_glVertexAttribL1ui64vARB = true;
    printf("Stub: glVertexAttribL1ui64vARB\n");
}
static bool trigger_glGetVertexAttribLui64vARB = false;
void stub_glGetVertexAttribLui64vARB() {
    if(trigger_glGetVertexAttribLui64vARB) return;
    trigger_glGetVertexAttribLui64vARB = true;
    printf("Stub: glGetVertexAttribLui64vARB\n");
}
static bool trigger_glCreateSyncFromCLeventARB = false;
void stub_glCreateSyncFromCLeventARB() {
    if(trigger_glCreateSyncFromCLeventARB) return;
    trigger_glCreateSyncFromCLeventARB = true;
    printf("Stub: glCreateSyncFromCLeventARB\n");
}
static bool trigger_glDispatchComputeGroupSizeARB = false;
void stub_glDispatchComputeGroupSizeARB() {
    if(trigger_glDispatchComputeGroupSizeARB) return;
    trigger_glDispatchComputeGroupSizeARB = true;
    printf("Stub: glDispatchComputeGroupSizeARB\n");
}
static bool trigger_glDebugMessageControlARB = false;
void stub_glDebugMessageControlARB() {
    if(trigger_glDebugMessageControlARB) return;
    trigger_glDebugMessageControlARB = true;
    printf("Stub: glDebugMessageControlARB\n");
}
static bool trigger_glDebugMessageInsertARB = false;
void stub_glDebugMessageInsertARB() {
    if(trigger_glDebugMessageInsertARB) return;
    trigger_glDebugMessageInsertARB = true;
    printf("Stub: glDebugMessageInsertARB\n");
}
static bool trigger_glDebugMessageCallbackARB = false;
void stub_glDebugMessageCallbackARB() {
    if(trigger_glDebugMessageCallbackARB) return;
    trigger_glDebugMessageCallbackARB = true;
    printf("Stub: glDebugMessageCallbackARB\n");
}
static bool trigger_glGetDebugMessageLogARB = false;
void stub_glGetDebugMessageLogARB() {
    if(trigger_glGetDebugMessageLogARB) return;
    trigger_glGetDebugMessageLogARB = true;
    printf("Stub: glGetDebugMessageLogARB\n");
}
static bool trigger_glBlendEquationiARB = false;
void stub_glBlendEquationiARB() {
    if(trigger_glBlendEquationiARB) return;
    trigger_glBlendEquationiARB = true;
    printf("Stub: glBlendEquationiARB\n");
}
static bool trigger_glBlendEquationSeparateiARB = false;
void stub_glBlendEquationSeparateiARB() {
    if(trigger_glBlendEquationSeparateiARB) return;
    trigger_glBlendEquationSeparateiARB = true;
    printf("Stub: glBlendEquationSeparateiARB\n");
}
static bool trigger_glBlendFunciARB = false;
void stub_glBlendFunciARB() {
    if(trigger_glBlendFunciARB) return;
    trigger_glBlendFunciARB = true;
    printf("Stub: glBlendFunciARB\n");
}
static bool trigger_glBlendFuncSeparateiARB = false;
void stub_glBlendFuncSeparateiARB() {
    if(trigger_glBlendFuncSeparateiARB) return;
    trigger_glBlendFuncSeparateiARB = true;
    printf("Stub: glBlendFuncSeparateiARB\n");
}
static bool trigger_glDrawArraysInstancedARB = false;
void stub_glDrawArraysInstancedARB() {
    if(trigger_glDrawArraysInstancedARB) return;
    trigger_glDrawArraysInstancedARB = true;
    printf("Stub: glDrawArraysInstancedARB\n");
}
static bool trigger_glDrawElementsInstancedARB = false;
void stub_glDrawElementsInstancedARB() {
    if(trigger_glDrawElementsInstancedARB) return;
    trigger_glDrawElementsInstancedARB = true;
    printf("Stub: glDrawElementsInstancedARB\n");
}
static bool trigger_glProgramParameteriARB = false;
void stub_glProgramParameteriARB() {
    if(trigger_glProgramParameteriARB) return;
    trigger_glProgramParameteriARB = true;
    printf("Stub: glProgramParameteriARB\n");
}
static bool trigger_glFramebufferTextureARB = false;
void stub_glFramebufferTextureARB() {
    if(trigger_glFramebufferTextureARB) return;
    trigger_glFramebufferTextureARB = true;
    printf("Stub: glFramebufferTextureARB\n");
}
static bool trigger_glFramebufferTextureLayerARB = false;
void stub_glFramebufferTextureLayerARB() {
    if(trigger_glFramebufferTextureLayerARB) return;
    trigger_glFramebufferTextureLayerARB = true;
    printf("Stub: glFramebufferTextureLayerARB\n");
}
static bool trigger_glFramebufferTextureFaceARB = false;
void stub_glFramebufferTextureFaceARB() {
    if(trigger_glFramebufferTextureFaceARB) return;
    trigger_glFramebufferTextureFaceARB = true;
    printf("Stub: glFramebufferTextureFaceARB\n");
}
static bool trigger_glSpecializeShaderARB = false;
void stub_glSpecializeShaderARB() {
    if(trigger_glSpecializeShaderARB) return;
    trigger_glSpecializeShaderARB = true;
    printf("Stub: glSpecializeShaderARB\n");
}
static bool trigger_glUniform1i64ARB = false;
void stub_glUniform1i64ARB() {
    if(trigger_glUniform1i64ARB) return;
    trigger_glUniform1i64ARB = true;
    printf("Stub: glUniform1i64ARB\n");
}
static bool trigger_glUniform2i64ARB = false;
void stub_glUniform2i64ARB() {
    if(trigger_glUniform2i64ARB) return;
    trigger_glUniform2i64ARB = true;
    printf("Stub: glUniform2i64ARB\n");
}
static bool trigger_glUniform3i64ARB = false;
void stub_glUniform3i64ARB() {
    if(trigger_glUniform3i64ARB) return;
    trigger_glUniform3i64ARB = true;
    printf("Stub: glUniform3i64ARB\n");
}
static bool trigger_glUniform4i64ARB = false;
void stub_glUniform4i64ARB() {
    if(trigger_glUniform4i64ARB) return;
    trigger_glUniform4i64ARB = true;
    printf("Stub: glUniform4i64ARB\n");
}
static bool trigger_glUniform1i64vARB = false;
void stub_glUniform1i64vARB() {
    if(trigger_glUniform1i64vARB) return;
    trigger_glUniform1i64vARB = true;
    printf("Stub: glUniform1i64vARB\n");
}
static bool trigger_glUniform2i64vARB = false;
void stub_glUniform2i64vARB() {
    if(trigger_glUniform2i64vARB) return;
    trigger_glUniform2i64vARB = true;
    printf("Stub: glUniform2i64vARB\n");
}
static bool trigger_glUniform3i64vARB = false;
void stub_glUniform3i64vARB() {
    if(trigger_glUniform3i64vARB) return;
    trigger_glUniform3i64vARB = true;
    printf("Stub: glUniform3i64vARB\n");
}
static bool trigger_glUniform4i64vARB = false;
void stub_glUniform4i64vARB() {
    if(trigger_glUniform4i64vARB) return;
    trigger_glUniform4i64vARB = true;
    printf("Stub: glUniform4i64vARB\n");
}
static bool trigger_glUniform1ui64ARB = false;
void stub_glUniform1ui64ARB() {
    if(trigger_glUniform1ui64ARB) return;
    trigger_glUniform1ui64ARB = true;
    printf("Stub: glUniform1ui64ARB\n");
}
static bool trigger_glUniform2ui64ARB = false;
void stub_glUniform2ui64ARB() {
    if(trigger_glUniform2ui64ARB) return;
    trigger_glUniform2ui64ARB = true;
    printf("Stub: glUniform2ui64ARB\n");
}
static bool trigger_glUniform3ui64ARB = false;
void stub_glUniform3ui64ARB() {
    if(trigger_glUniform3ui64ARB) return;
    trigger_glUniform3ui64ARB = true;
    printf("Stub: glUniform3ui64ARB\n");
}
static bool trigger_glUniform4ui64ARB = false;
void stub_glUniform4ui64ARB() {
    if(trigger_glUniform4ui64ARB) return;
    trigger_glUniform4ui64ARB = true;
    printf("Stub: glUniform4ui64ARB\n");
}
static bool trigger_glUniform1ui64vARB = false;
void stub_glUniform1ui64vARB() {
    if(trigger_glUniform1ui64vARB) return;
    trigger_glUniform1ui64vARB = true;
    printf("Stub: glUniform1ui64vARB\n");
}
static bool trigger_glUniform2ui64vARB = false;
void stub_glUniform2ui64vARB() {
    if(trigger_glUniform2ui64vARB) return;
    trigger_glUniform2ui64vARB = true;
    printf("Stub: glUniform2ui64vARB\n");
}
static bool trigger_glUniform3ui64vARB = false;
void stub_glUniform3ui64vARB() {
    if(trigger_glUniform3ui64vARB) return;
    trigger_glUniform3ui64vARB = true;
    printf("Stub: glUniform3ui64vARB\n");
}
static bool trigger_glUniform4ui64vARB = false;
void stub_glUniform4ui64vARB() {
    if(trigger_glUniform4ui64vARB) return;
    trigger_glUniform4ui64vARB = true;
    printf("Stub: glUniform4ui64vARB\n");
}
static bool trigger_glGetUniformi64vARB = false;
void stub_glGetUniformi64vARB() {
    if(trigger_glGetUniformi64vARB) return;
    trigger_glGetUniformi64vARB = true;
    printf("Stub: glGetUniformi64vARB\n");
}
static bool trigger_glGetUniformui64vARB = false;
void stub_glGetUniformui64vARB() {
    if(trigger_glGetUniformui64vARB) return;
    trigger_glGetUniformui64vARB = true;
    printf("Stub: glGetUniformui64vARB\n");
}
static bool trigger_glGetnUniformi64vARB = false;
void stub_glGetnUniformi64vARB() {
    if(trigger_glGetnUniformi64vARB) return;
    trigger_glGetnUniformi64vARB = true;
    printf("Stub: glGetnUniformi64vARB\n");
}
static bool trigger_glGetnUniformui64vARB = false;
void stub_glGetnUniformui64vARB() {
    if(trigger_glGetnUniformui64vARB) return;
    trigger_glGetnUniformui64vARB = true;
    printf("Stub: glGetnUniformui64vARB\n");
}
static bool trigger_glProgramUniform1i64ARB = false;
void stub_glProgramUniform1i64ARB() {
    if(trigger_glProgramUniform1i64ARB) return;
    trigger_glProgramUniform1i64ARB = true;
    printf("Stub: glProgramUniform1i64ARB\n");
}
static bool trigger_glProgramUniform2i64ARB = false;
void stub_glProgramUniform2i64ARB() {
    if(trigger_glProgramUniform2i64ARB) return;
    trigger_glProgramUniform2i64ARB = true;
    printf("Stub: glProgramUniform2i64ARB\n");
}
static bool trigger_glProgramUniform3i64ARB = false;
void stub_glProgramUniform3i64ARB() {
    if(trigger_glProgramUniform3i64ARB) return;
    trigger_glProgramUniform3i64ARB = true;
    printf("Stub: glProgramUniform3i64ARB\n");
}
static bool trigger_glProgramUniform4i64ARB = false;
void stub_glProgramUniform4i64ARB() {
    if(trigger_glProgramUniform4i64ARB) return;
    trigger_glProgramUniform4i64ARB = true;
    printf("Stub: glProgramUniform4i64ARB\n");
}
static bool trigger_glProgramUniform1i64vARB = false;
void stub_glProgramUniform1i64vARB() {
    if(trigger_glProgramUniform1i64vARB) return;
    trigger_glProgramUniform1i64vARB = true;
    printf("Stub: glProgramUniform1i64vARB\n");
}
static bool trigger_glProgramUniform2i64vARB = false;
void stub_glProgramUniform2i64vARB() {
    if(trigger_glProgramUniform2i64vARB) return;
    trigger_glProgramUniform2i64vARB = true;
    printf("Stub: glProgramUniform2i64vARB\n");
}
static bool trigger_glProgramUniform3i64vARB = false;
void stub_glProgramUniform3i64vARB() {
    if(trigger_glProgramUniform3i64vARB) return;
    trigger_glProgramUniform3i64vARB = true;
    printf("Stub: glProgramUniform3i64vARB\n");
}
static bool trigger_glProgramUniform4i64vARB = false;
void stub_glProgramUniform4i64vARB() {
    if(trigger_glProgramUniform4i64vARB) return;
    trigger_glProgramUniform4i64vARB = true;
    printf("Stub: glProgramUniform4i64vARB\n");
}
static bool trigger_glProgramUniform1ui64ARB = false;
void stub_glProgramUniform1ui64ARB() {
    if(trigger_glProgramUniform1ui64ARB) return;
    trigger_glProgramUniform1ui64ARB = true;
    printf("Stub: glProgramUniform1ui64ARB\n");
}
static bool trigger_glProgramUniform2ui64ARB = false;
void stub_glProgramUniform2ui64ARB() {
    if(trigger_glProgramUniform2ui64ARB) return;
    trigger_glProgramUniform2ui64ARB = true;
    printf("Stub: glProgramUniform2ui64ARB\n");
}
static bool trigger_glProgramUniform3ui64ARB = false;
void stub_glProgramUniform3ui64ARB() {
    if(trigger_glProgramUniform3ui64ARB) return;
    trigger_glProgramUniform3ui64ARB = true;
    printf("Stub: glProgramUniform3ui64ARB\n");
}
static bool trigger_glProgramUniform4ui64ARB = false;
void stub_glProgramUniform4ui64ARB() {
    if(trigger_glProgramUniform4ui64ARB) return;
    trigger_glProgramUniform4ui64ARB = true;
    printf("Stub: glProgramUniform4ui64ARB\n");
}
static bool trigger_glProgramUniform1ui64vARB = false;
void stub_glProgramUniform1ui64vARB() {
    if(trigger_glProgramUniform1ui64vARB) return;
    trigger_glProgramUniform1ui64vARB = true;
    printf("Stub: glProgramUniform1ui64vARB\n");
}
static bool trigger_glProgramUniform2ui64vARB = false;
void stub_glProgramUniform2ui64vARB() {
    if(trigger_glProgramUniform2ui64vARB) return;
    trigger_glProgramUniform2ui64vARB = true;
    printf("Stub: glProgramUniform2ui64vARB\n");
}
static bool trigger_glProgramUniform3ui64vARB = false;
void stub_glProgramUniform3ui64vARB() {
    if(trigger_glProgramUniform3ui64vARB) return;
    trigger_glProgramUniform3ui64vARB = true;
    printf("Stub: glProgramUniform3ui64vARB\n");
}
static bool trigger_glProgramUniform4ui64vARB = false;
void stub_glProgramUniform4ui64vARB() {
    if(trigger_glProgramUniform4ui64vARB) return;
    trigger_glProgramUniform4ui64vARB = true;
    printf("Stub: glProgramUniform4ui64vARB\n");
}
static bool trigger_glMultiDrawArraysIndirectCountARB = false;
void stub_glMultiDrawArraysIndirectCountARB() {
    if(trigger_glMultiDrawArraysIndirectCountARB) return;
    trigger_glMultiDrawArraysIndirectCountARB = true;
    printf("Stub: glMultiDrawArraysIndirectCountARB\n");
}
static bool trigger_glMultiDrawElementsIndirectCountARB = false;
void stub_glMultiDrawElementsIndirectCountARB() {
    if(trigger_glMultiDrawElementsIndirectCountARB) return;
    trigger_glMultiDrawElementsIndirectCountARB = true;
    printf("Stub: glMultiDrawElementsIndirectCountARB\n");
}
static bool trigger_glVertexAttribDivisorARB = false;
void stub_glVertexAttribDivisorARB() {
    if(trigger_glVertexAttribDivisorARB) return;
    trigger_glVertexAttribDivisorARB = true;
    printf("Stub: glVertexAttribDivisorARB\n");
}
static bool trigger_glMaxShaderCompilerThreadsARB = false;
void stub_glMaxShaderCompilerThreadsARB() {
    if(trigger_glMaxShaderCompilerThreadsARB) return;
    trigger_glMaxShaderCompilerThreadsARB = true;
    printf("Stub: glMaxShaderCompilerThreadsARB\n");
}
static bool trigger_glGetGraphicsResetStatusARB = false;
void stub_glGetGraphicsResetStatusARB() {
    if(trigger_glGetGraphicsResetStatusARB) return;
    trigger_glGetGraphicsResetStatusARB = true;
    printf("Stub: glGetGraphicsResetStatusARB\n");
}
static bool trigger_glGetnTexImageARB = false;
void stub_glGetnTexImageARB() {
    if(trigger_glGetnTexImageARB) return;
    trigger_glGetnTexImageARB = true;
    printf("Stub: glGetnTexImageARB\n");
}
static bool trigger_glReadnPixelsARB = false;
void stub_glReadnPixelsARB() {
    if(trigger_glReadnPixelsARB) return;
    trigger_glReadnPixelsARB = true;
    printf("Stub: glReadnPixelsARB\n");
}
static bool trigger_glGetnCompressedTexImageARB = false;
void stub_glGetnCompressedTexImageARB() {
    if(trigger_glGetnCompressedTexImageARB) return;
    trigger_glGetnCompressedTexImageARB = true;
    printf("Stub: glGetnCompressedTexImageARB\n");
}
static bool trigger_glGetnUniformfvARB = false;
void stub_glGetnUniformfvARB() {
    if(trigger_glGetnUniformfvARB) return;
    trigger_glGetnUniformfvARB = true;
    printf("Stub: glGetnUniformfvARB\n");
}
static bool trigger_glGetnUniformivARB = false;
void stub_glGetnUniformivARB() {
    if(trigger_glGetnUniformivARB) return;
    trigger_glGetnUniformivARB = true;
    printf("Stub: glGetnUniformivARB\n");
}
static bool trigger_glGetnUniformuivARB = false;
void stub_glGetnUniformuivARB() {
    if(trigger_glGetnUniformuivARB) return;
    trigger_glGetnUniformuivARB = true;
    printf("Stub: glGetnUniformuivARB\n");
}
static bool trigger_glGetnUniformdvARB = false;
void stub_glGetnUniformdvARB() {
    if(trigger_glGetnUniformdvARB) return;
    trigger_glGetnUniformdvARB = true;
    printf("Stub: glGetnUniformdvARB\n");
}
static bool trigger_glFramebufferSampleLocationsfvARB = false;
void stub_glFramebufferSampleLocationsfvARB() {
    if(trigger_glFramebufferSampleLocationsfvARB) return;
    trigger_glFramebufferSampleLocationsfvARB = true;
    printf("Stub: glFramebufferSampleLocationsfvARB\n");
}
static bool trigger_glNamedFramebufferSampleLocationsfvARB = false;
void stub_glNamedFramebufferSampleLocationsfvARB() {
    if(trigger_glNamedFramebufferSampleLocationsfvARB) return;
    trigger_glNamedFramebufferSampleLocationsfvARB = true;
    printf("Stub: glNamedFramebufferSampleLocationsfvARB\n");
}
static bool trigger_glEvaluateDepthValuesARB = false;
void stub_glEvaluateDepthValuesARB() {
    if(trigger_glEvaluateDepthValuesARB) return;
    trigger_glEvaluateDepthValuesARB = true;
    printf("Stub: glEvaluateDepthValuesARB\n");
}
static bool trigger_glMinSampleShadingARB = false;
void stub_glMinSampleShadingARB() {
    if(trigger_glMinSampleShadingARB) return;
    trigger_glMinSampleShadingARB = true;
    printf("Stub: glMinSampleShadingARB\n");
}
static bool trigger_glNamedStringARB = false;
void stub_glNamedStringARB() {
    if(trigger_glNamedStringARB) return;
    trigger_glNamedStringARB = true;
    printf("Stub: glNamedStringARB\n");
}
static bool trigger_glDeleteNamedStringARB = false;
void stub_glDeleteNamedStringARB() {
    if(trigger_glDeleteNamedStringARB) return;
    trigger_glDeleteNamedStringARB = true;
    printf("Stub: glDeleteNamedStringARB\n");
}
static bool trigger_glCompileShaderIncludeARB = false;
void stub_glCompileShaderIncludeARB() {
    if(trigger_glCompileShaderIncludeARB) return;
    trigger_glCompileShaderIncludeARB = true;
    printf("Stub: glCompileShaderIncludeARB\n");
}
static bool trigger_glIsNamedStringARB = false;
void stub_glIsNamedStringARB() {
    if(trigger_glIsNamedStringARB) return;
    trigger_glIsNamedStringARB = true;
    printf("Stub: glIsNamedStringARB\n");
}
static bool trigger_glGetNamedStringARB = false;
void stub_glGetNamedStringARB() {
    if(trigger_glGetNamedStringARB) return;
    trigger_glGetNamedStringARB = true;
    printf("Stub: glGetNamedStringARB\n");
}
static bool trigger_glGetNamedStringivARB = false;
void stub_glGetNamedStringivARB() {
    if(trigger_glGetNamedStringivARB) return;
    trigger_glGetNamedStringivARB = true;
    printf("Stub: glGetNamedStringivARB\n");
}
static bool trigger_glBufferPageCommitmentARB = false;
void stub_glBufferPageCommitmentARB() {
    if(trigger_glBufferPageCommitmentARB) return;
    trigger_glBufferPageCommitmentARB = true;
    printf("Stub: glBufferPageCommitmentARB\n");
}
static bool trigger_glNamedBufferPageCommitmentEXT = false;
void stub_glNamedBufferPageCommitmentEXT() {
    if(trigger_glNamedBufferPageCommitmentEXT) return;
    trigger_glNamedBufferPageCommitmentEXT = true;
    printf("Stub: glNamedBufferPageCommitmentEXT\n");
}
static bool trigger_glNamedBufferPageCommitmentARB = false;
void stub_glNamedBufferPageCommitmentARB() {
    if(trigger_glNamedBufferPageCommitmentARB) return;
    trigger_glNamedBufferPageCommitmentARB = true;
    printf("Stub: glNamedBufferPageCommitmentARB\n");
}
static bool trigger_glTexPageCommitmentARB = false;
void stub_glTexPageCommitmentARB() {
    if(trigger_glTexPageCommitmentARB) return;
    trigger_glTexPageCommitmentARB = true;
    printf("Stub: glTexPageCommitmentARB\n");
}
static bool trigger_glTexBufferARB = false;
void stub_glTexBufferARB() {
    if(trigger_glTexBufferARB) return;
    trigger_glTexBufferARB = true;
    printf("Stub: glTexBufferARB\n");
}
static bool trigger_glDepthRangeArraydvNV = false;
void stub_glDepthRangeArraydvNV() {
    if(trigger_glDepthRangeArraydvNV) return;
    trigger_glDepthRangeArraydvNV = true;
    printf("Stub: glDepthRangeArraydvNV\n");
}
static bool trigger_glDepthRangeIndexeddNV = false;
void stub_glDepthRangeIndexeddNV() {
    if(trigger_glDepthRangeIndexeddNV) return;
    trigger_glDepthRangeIndexeddNV = true;
    printf("Stub: glDepthRangeIndexeddNV\n");
}
static bool trigger_glBlendBarrierKHR = false;
void stub_glBlendBarrierKHR() {
    if(trigger_glBlendBarrierKHR) return;
    trigger_glBlendBarrierKHR = true;
    printf("Stub: glBlendBarrierKHR\n");
}
static bool trigger_glMaxShaderCompilerThreadsKHR = false;
void stub_glMaxShaderCompilerThreadsKHR() {
    if(trigger_glMaxShaderCompilerThreadsKHR) return;
    trigger_glMaxShaderCompilerThreadsKHR = true;
    printf("Stub: glMaxShaderCompilerThreadsKHR\n");
}
static bool trigger_glRenderbufferStorageMultisampleAdvancedAMD = false;
void stub_glRenderbufferStorageMultisampleAdvancedAMD() {
    if(trigger_glRenderbufferStorageMultisampleAdvancedAMD) return;
    trigger_glRenderbufferStorageMultisampleAdvancedAMD = true;
    printf("Stub: glRenderbufferStorageMultisampleAdvancedAMD\n");
}
static bool trigger_glNamedRenderbufferStorageMultisampleAdvancedAMD = false;
void stub_glNamedRenderbufferStorageMultisampleAdvancedAMD() {
    if(trigger_glNamedRenderbufferStorageMultisampleAdvancedAMD) return;
    trigger_glNamedRenderbufferStorageMultisampleAdvancedAMD = true;
    printf("Stub: glNamedRenderbufferStorageMultisampleAdvancedAMD\n");
}
static bool trigger_glGetPerfMonitorGroupsAMD = false;
void stub_glGetPerfMonitorGroupsAMD() {
    if(trigger_glGetPerfMonitorGroupsAMD) return;
    trigger_glGetPerfMonitorGroupsAMD = true;
    printf("Stub: glGetPerfMonitorGroupsAMD\n");
}
static bool trigger_glGetPerfMonitorCountersAMD = false;
void stub_glGetPerfMonitorCountersAMD() {
    if(trigger_glGetPerfMonitorCountersAMD) return;
    trigger_glGetPerfMonitorCountersAMD = true;
    printf("Stub: glGetPerfMonitorCountersAMD\n");
}
static bool trigger_glGetPerfMonitorGroupStringAMD = false;
void stub_glGetPerfMonitorGroupStringAMD() {
    if(trigger_glGetPerfMonitorGroupStringAMD) return;
    trigger_glGetPerfMonitorGroupStringAMD = true;
    printf("Stub: glGetPerfMonitorGroupStringAMD\n");
}
static bool trigger_glGetPerfMonitorCounterStringAMD = false;
void stub_glGetPerfMonitorCounterStringAMD() {
    if(trigger_glGetPerfMonitorCounterStringAMD) return;
    trigger_glGetPerfMonitorCounterStringAMD = true;
    printf("Stub: glGetPerfMonitorCounterStringAMD\n");
}
static bool trigger_glGetPerfMonitorCounterInfoAMD = false;
void stub_glGetPerfMonitorCounterInfoAMD() {
    if(trigger_glGetPerfMonitorCounterInfoAMD) return;
    trigger_glGetPerfMonitorCounterInfoAMD = true;
    printf("Stub: glGetPerfMonitorCounterInfoAMD\n");
}
static bool trigger_glGenPerfMonitorsAMD = false;
void stub_glGenPerfMonitorsAMD() {
    if(trigger_glGenPerfMonitorsAMD) return;
    trigger_glGenPerfMonitorsAMD = true;
    printf("Stub: glGenPerfMonitorsAMD\n");
}
static bool trigger_glDeletePerfMonitorsAMD = false;
void stub_glDeletePerfMonitorsAMD() {
    if(trigger_glDeletePerfMonitorsAMD) return;
    trigger_glDeletePerfMonitorsAMD = true;
    printf("Stub: glDeletePerfMonitorsAMD\n");
}
static bool trigger_glSelectPerfMonitorCountersAMD = false;
void stub_glSelectPerfMonitorCountersAMD() {
    if(trigger_glSelectPerfMonitorCountersAMD) return;
    trigger_glSelectPerfMonitorCountersAMD = true;
    printf("Stub: glSelectPerfMonitorCountersAMD\n");
}
static bool trigger_glBeginPerfMonitorAMD = false;
void stub_glBeginPerfMonitorAMD() {
    if(trigger_glBeginPerfMonitorAMD) return;
    trigger_glBeginPerfMonitorAMD = true;
    printf("Stub: glBeginPerfMonitorAMD\n");
}
static bool trigger_glEndPerfMonitorAMD = false;
void stub_glEndPerfMonitorAMD() {
    if(trigger_glEndPerfMonitorAMD) return;
    trigger_glEndPerfMonitorAMD = true;
    printf("Stub: glEndPerfMonitorAMD\n");
}
static bool trigger_glGetPerfMonitorCounterDataAMD = false;
void stub_glGetPerfMonitorCounterDataAMD() {
    if(trigger_glGetPerfMonitorCounterDataAMD) return;
    trigger_glGetPerfMonitorCounterDataAMD = true;
    printf("Stub: glGetPerfMonitorCounterDataAMD\n");
}
static bool trigger_glEGLImageTargetTexStorageEXT = false;
void stub_glEGLImageTargetTexStorageEXT() {
    if(trigger_glEGLImageTargetTexStorageEXT) return;
    trigger_glEGLImageTargetTexStorageEXT = true;
    printf("Stub: glEGLImageTargetTexStorageEXT\n");
}
static bool trigger_glEGLImageTargetTextureStorageEXT = false;
void stub_glEGLImageTargetTextureStorageEXT() {
    if(trigger_glEGLImageTargetTextureStorageEXT) return;
    trigger_glEGLImageTargetTextureStorageEXT = true;
    printf("Stub: glEGLImageTargetTextureStorageEXT\n");
}
static bool trigger_glLabelObjectEXT = false;
void stub_glLabelObjectEXT() {
    if(trigger_glLabelObjectEXT) return;
    trigger_glLabelObjectEXT = true;
    printf("Stub: glLabelObjectEXT\n");
}
static bool trigger_glGetObjectLabelEXT = false;
void stub_glGetObjectLabelEXT() {
    if(trigger_glGetObjectLabelEXT) return;
    trigger_glGetObjectLabelEXT = true;
    printf("Stub: glGetObjectLabelEXT\n");
}
static bool trigger_glInsertEventMarkerEXT = false;
void stub_glInsertEventMarkerEXT() {
    if(trigger_glInsertEventMarkerEXT) return;
    trigger_glInsertEventMarkerEXT = true;
    printf("Stub: glInsertEventMarkerEXT\n");
}
static bool trigger_glPushGroupMarkerEXT = false;
void stub_glPushGroupMarkerEXT() {
    if(trigger_glPushGroupMarkerEXT) return;
    trigger_glPushGroupMarkerEXT = true;
    printf("Stub: glPushGroupMarkerEXT\n");
}
static bool trigger_glPopGroupMarkerEXT = false;
void stub_glPopGroupMarkerEXT() {
    if(trigger_glPopGroupMarkerEXT) return;
    trigger_glPopGroupMarkerEXT = true;
    printf("Stub: glPopGroupMarkerEXT\n");
}
static bool trigger_glMatrixLoadfEXT = false;
void stub_glMatrixLoadfEXT() {
    if(trigger_glMatrixLoadfEXT) return;
    trigger_glMatrixLoadfEXT = true;
    printf("Stub: glMatrixLoadfEXT\n");
}
static bool trigger_glMatrixLoaddEXT = false;
void stub_glMatrixLoaddEXT() {
    if(trigger_glMatrixLoaddEXT) return;
    trigger_glMatrixLoaddEXT = true;
    printf("Stub: glMatrixLoaddEXT\n");
}
static bool trigger_glMatrixMultfEXT = false;
void stub_glMatrixMultfEXT() {
    if(trigger_glMatrixMultfEXT) return;
    trigger_glMatrixMultfEXT = true;
    printf("Stub: glMatrixMultfEXT\n");
}
static bool trigger_glMatrixMultdEXT = false;
void stub_glMatrixMultdEXT() {
    if(trigger_glMatrixMultdEXT) return;
    trigger_glMatrixMultdEXT = true;
    printf("Stub: glMatrixMultdEXT\n");
}
static bool trigger_glMatrixLoadIdentityEXT = false;
void stub_glMatrixLoadIdentityEXT() {
    if(trigger_glMatrixLoadIdentityEXT) return;
    trigger_glMatrixLoadIdentityEXT = true;
    printf("Stub: glMatrixLoadIdentityEXT\n");
}
static bool trigger_glMatrixRotatefEXT = false;
void stub_glMatrixRotatefEXT() {
    if(trigger_glMatrixRotatefEXT) return;
    trigger_glMatrixRotatefEXT = true;
    printf("Stub: glMatrixRotatefEXT\n");
}
static bool trigger_glMatrixRotatedEXT = false;
void stub_glMatrixRotatedEXT() {
    if(trigger_glMatrixRotatedEXT) return;
    trigger_glMatrixRotatedEXT = true;
    printf("Stub: glMatrixRotatedEXT\n");
}
static bool trigger_glMatrixScalefEXT = false;
void stub_glMatrixScalefEXT() {
    if(trigger_glMatrixScalefEXT) return;
    trigger_glMatrixScalefEXT = true;
    printf("Stub: glMatrixScalefEXT\n");
}
static bool trigger_glMatrixScaledEXT = false;
void stub_glMatrixScaledEXT() {
    if(trigger_glMatrixScaledEXT) return;
    trigger_glMatrixScaledEXT = true;
    printf("Stub: glMatrixScaledEXT\n");
}
static bool trigger_glMatrixTranslatefEXT = false;
void stub_glMatrixTranslatefEXT() {
    if(trigger_glMatrixTranslatefEXT) return;
    trigger_glMatrixTranslatefEXT = true;
    printf("Stub: glMatrixTranslatefEXT\n");
}
static bool trigger_glMatrixTranslatedEXT = false;
void stub_glMatrixTranslatedEXT() {
    if(trigger_glMatrixTranslatedEXT) return;
    trigger_glMatrixTranslatedEXT = true;
    printf("Stub: glMatrixTranslatedEXT\n");
}
static bool trigger_glMatrixFrustumEXT = false;
void stub_glMatrixFrustumEXT() {
    if(trigger_glMatrixFrustumEXT) return;
    trigger_glMatrixFrustumEXT = true;
    printf("Stub: glMatrixFrustumEXT\n");
}
static bool trigger_glMatrixOrthoEXT = false;
void stub_glMatrixOrthoEXT() {
    if(trigger_glMatrixOrthoEXT) return;
    trigger_glMatrixOrthoEXT = true;
    printf("Stub: glMatrixOrthoEXT\n");
}
static bool trigger_glMatrixPopEXT = false;
void stub_glMatrixPopEXT() {
    if(trigger_glMatrixPopEXT) return;
    trigger_glMatrixPopEXT = true;
    printf("Stub: glMatrixPopEXT\n");
}
static bool trigger_glMatrixPushEXT = false;
void stub_glMatrixPushEXT() {
    if(trigger_glMatrixPushEXT) return;
    trigger_glMatrixPushEXT = true;
    printf("Stub: glMatrixPushEXT\n");
}
static bool trigger_glClientAttribDefaultEXT = false;
void stub_glClientAttribDefaultEXT() {
    if(trigger_glClientAttribDefaultEXT) return;
    trigger_glClientAttribDefaultEXT = true;
    printf("Stub: glClientAttribDefaultEXT\n");
}
static bool trigger_glPushClientAttribDefaultEXT = false;
void stub_glPushClientAttribDefaultEXT() {
    if(trigger_glPushClientAttribDefaultEXT) return;
    trigger_glPushClientAttribDefaultEXT = true;
    printf("Stub: glPushClientAttribDefaultEXT\n");
}
static bool trigger_glTextureParameterfEXT = false;
void stub_glTextureParameterfEXT() {
    if(trigger_glTextureParameterfEXT) return;
    trigger_glTextureParameterfEXT = true;
    printf("Stub: glTextureParameterfEXT\n");
}
static bool trigger_glTextureParameterfvEXT = false;
void stub_glTextureParameterfvEXT() {
    if(trigger_glTextureParameterfvEXT) return;
    trigger_glTextureParameterfvEXT = true;
    printf("Stub: glTextureParameterfvEXT\n");
}
static bool trigger_glTextureParameteriEXT = false;
void stub_glTextureParameteriEXT() {
    if(trigger_glTextureParameteriEXT) return;
    trigger_glTextureParameteriEXT = true;
    printf("Stub: glTextureParameteriEXT\n");
}
static bool trigger_glTextureParameterivEXT = false;
void stub_glTextureParameterivEXT() {
    if(trigger_glTextureParameterivEXT) return;
    trigger_glTextureParameterivEXT = true;
    printf("Stub: glTextureParameterivEXT\n");
}
static bool trigger_glTextureImage1DEXT = false;
void stub_glTextureImage1DEXT() {
    if(trigger_glTextureImage1DEXT) return;
    trigger_glTextureImage1DEXT = true;
    printf("Stub: glTextureImage1DEXT\n");
}
static bool trigger_glTextureImage2DEXT = false;
void stub_glTextureImage2DEXT() {
    if(trigger_glTextureImage2DEXT) return;
    trigger_glTextureImage2DEXT = true;
    printf("Stub: glTextureImage2DEXT\n");
}
static bool trigger_glTextureSubImage1DEXT = false;
void stub_glTextureSubImage1DEXT() {
    if(trigger_glTextureSubImage1DEXT) return;
    trigger_glTextureSubImage1DEXT = true;
    printf("Stub: glTextureSubImage1DEXT\n");
}
static bool trigger_glTextureSubImage2DEXT = false;
void stub_glTextureSubImage2DEXT() {
    if(trigger_glTextureSubImage2DEXT) return;
    trigger_glTextureSubImage2DEXT = true;
    printf("Stub: glTextureSubImage2DEXT\n");
}
static bool trigger_glCopyTextureImage1DEXT = false;
void stub_glCopyTextureImage1DEXT() {
    if(trigger_glCopyTextureImage1DEXT) return;
    trigger_glCopyTextureImage1DEXT = true;
    printf("Stub: glCopyTextureImage1DEXT\n");
}
static bool trigger_glCopyTextureImage2DEXT = false;
void stub_glCopyTextureImage2DEXT() {
    if(trigger_glCopyTextureImage2DEXT) return;
    trigger_glCopyTextureImage2DEXT = true;
    printf("Stub: glCopyTextureImage2DEXT\n");
}
static bool trigger_glCopyTextureSubImage1DEXT = false;
void stub_glCopyTextureSubImage1DEXT() {
    if(trigger_glCopyTextureSubImage1DEXT) return;
    trigger_glCopyTextureSubImage1DEXT = true;
    printf("Stub: glCopyTextureSubImage1DEXT\n");
}
static bool trigger_glCopyTextureSubImage2DEXT = false;
void stub_glCopyTextureSubImage2DEXT() {
    if(trigger_glCopyTextureSubImage2DEXT) return;
    trigger_glCopyTextureSubImage2DEXT = true;
    printf("Stub: glCopyTextureSubImage2DEXT\n");
}
static bool trigger_glGetTextureImageEXT = false;
void stub_glGetTextureImageEXT() {
    if(trigger_glGetTextureImageEXT) return;
    trigger_glGetTextureImageEXT = true;
    printf("Stub: glGetTextureImageEXT\n");
}
static bool trigger_glGetTextureParameterfvEXT = false;
void stub_glGetTextureParameterfvEXT() {
    if(trigger_glGetTextureParameterfvEXT) return;
    trigger_glGetTextureParameterfvEXT = true;
    printf("Stub: glGetTextureParameterfvEXT\n");
}
static bool trigger_glGetTextureParameterivEXT = false;
void stub_glGetTextureParameterivEXT() {
    if(trigger_glGetTextureParameterivEXT) return;
    trigger_glGetTextureParameterivEXT = true;
    printf("Stub: glGetTextureParameterivEXT\n");
}
static bool trigger_glGetTextureLevelParameterfvEXT = false;
void stub_glGetTextureLevelParameterfvEXT() {
    if(trigger_glGetTextureLevelParameterfvEXT) return;
    trigger_glGetTextureLevelParameterfvEXT = true;
    printf("Stub: glGetTextureLevelParameterfvEXT\n");
}
static bool trigger_glGetTextureLevelParameterivEXT = false;
void stub_glGetTextureLevelParameterivEXT() {
    if(trigger_glGetTextureLevelParameterivEXT) return;
    trigger_glGetTextureLevelParameterivEXT = true;
    printf("Stub: glGetTextureLevelParameterivEXT\n");
}
static bool trigger_glTextureImage3DEXT = false;
void stub_glTextureImage3DEXT() {
    if(trigger_glTextureImage3DEXT) return;
    trigger_glTextureImage3DEXT = true;
    printf("Stub: glTextureImage3DEXT\n");
}
static bool trigger_glTextureSubImage3DEXT = false;
void stub_glTextureSubImage3DEXT() {
    if(trigger_glTextureSubImage3DEXT) return;
    trigger_glTextureSubImage3DEXT = true;
    printf("Stub: glTextureSubImage3DEXT\n");
}
static bool trigger_glCopyTextureSubImage3DEXT = false;
void stub_glCopyTextureSubImage3DEXT() {
    if(trigger_glCopyTextureSubImage3DEXT) return;
    trigger_glCopyTextureSubImage3DEXT = true;
    printf("Stub: glCopyTextureSubImage3DEXT\n");
}
static bool trigger_glBindMultiTextureEXT = false;
void stub_glBindMultiTextureEXT() {
    if(trigger_glBindMultiTextureEXT) return;
    trigger_glBindMultiTextureEXT = true;
    printf("Stub: glBindMultiTextureEXT\n");
}
static bool trigger_glMultiTexCoordPointerEXT = false;
void stub_glMultiTexCoordPointerEXT() {
    if(trigger_glMultiTexCoordPointerEXT) return;
    trigger_glMultiTexCoordPointerEXT = true;
    printf("Stub: glMultiTexCoordPointerEXT\n");
}
static bool trigger_glMultiTexEnvfEXT = false;
void stub_glMultiTexEnvfEXT() {
    if(trigger_glMultiTexEnvfEXT) return;
    trigger_glMultiTexEnvfEXT = true;
    printf("Stub: glMultiTexEnvfEXT\n");
}
static bool trigger_glMultiTexEnvfvEXT = false;
void stub_glMultiTexEnvfvEXT() {
    if(trigger_glMultiTexEnvfvEXT) return;
    trigger_glMultiTexEnvfvEXT = true;
    printf("Stub: glMultiTexEnvfvEXT\n");
}
static bool trigger_glMultiTexEnviEXT = false;
void stub_glMultiTexEnviEXT() {
    if(trigger_glMultiTexEnviEXT) return;
    trigger_glMultiTexEnviEXT = true;
    printf("Stub: glMultiTexEnviEXT\n");
}
static bool trigger_glMultiTexEnvivEXT = false;
void stub_glMultiTexEnvivEXT() {
    if(trigger_glMultiTexEnvivEXT) return;
    trigger_glMultiTexEnvivEXT = true;
    printf("Stub: glMultiTexEnvivEXT\n");
}
static bool trigger_glMultiTexGendEXT = false;
void stub_glMultiTexGendEXT() {
    if(trigger_glMultiTexGendEXT) return;
    trigger_glMultiTexGendEXT = true;
    printf("Stub: glMultiTexGendEXT\n");
}
static bool trigger_glMultiTexGendvEXT = false;
void stub_glMultiTexGendvEXT() {
    if(trigger_glMultiTexGendvEXT) return;
    trigger_glMultiTexGendvEXT = true;
    printf("Stub: glMultiTexGendvEXT\n");
}
static bool trigger_glMultiTexGenfEXT = false;
void stub_glMultiTexGenfEXT() {
    if(trigger_glMultiTexGenfEXT) return;
    trigger_glMultiTexGenfEXT = true;
    printf("Stub: glMultiTexGenfEXT\n");
}
static bool trigger_glMultiTexGenfvEXT = false;
void stub_glMultiTexGenfvEXT() {
    if(trigger_glMultiTexGenfvEXT) return;
    trigger_glMultiTexGenfvEXT = true;
    printf("Stub: glMultiTexGenfvEXT\n");
}
static bool trigger_glMultiTexGeniEXT = false;
void stub_glMultiTexGeniEXT() {
    if(trigger_glMultiTexGeniEXT) return;
    trigger_glMultiTexGeniEXT = true;
    printf("Stub: glMultiTexGeniEXT\n");
}
static bool trigger_glMultiTexGenivEXT = false;
void stub_glMultiTexGenivEXT() {
    if(trigger_glMultiTexGenivEXT) return;
    trigger_glMultiTexGenivEXT = true;
    printf("Stub: glMultiTexGenivEXT\n");
}
static bool trigger_glGetMultiTexEnvfvEXT = false;
void stub_glGetMultiTexEnvfvEXT() {
    if(trigger_glGetMultiTexEnvfvEXT) return;
    trigger_glGetMultiTexEnvfvEXT = true;
    printf("Stub: glGetMultiTexEnvfvEXT\n");
}
static bool trigger_glGetMultiTexEnvivEXT = false;
void stub_glGetMultiTexEnvivEXT() {
    if(trigger_glGetMultiTexEnvivEXT) return;
    trigger_glGetMultiTexEnvivEXT = true;
    printf("Stub: glGetMultiTexEnvivEXT\n");
}
static bool trigger_glGetMultiTexGendvEXT = false;
void stub_glGetMultiTexGendvEXT() {
    if(trigger_glGetMultiTexGendvEXT) return;
    trigger_glGetMultiTexGendvEXT = true;
    printf("Stub: glGetMultiTexGendvEXT\n");
}
static bool trigger_glGetMultiTexGenfvEXT = false;
void stub_glGetMultiTexGenfvEXT() {
    if(trigger_glGetMultiTexGenfvEXT) return;
    trigger_glGetMultiTexGenfvEXT = true;
    printf("Stub: glGetMultiTexGenfvEXT\n");
}
static bool trigger_glGetMultiTexGenivEXT = false;
void stub_glGetMultiTexGenivEXT() {
    if(trigger_glGetMultiTexGenivEXT) return;
    trigger_glGetMultiTexGenivEXT = true;
    printf("Stub: glGetMultiTexGenivEXT\n");
}
static bool trigger_glMultiTexParameteriEXT = false;
void stub_glMultiTexParameteriEXT() {
    if(trigger_glMultiTexParameteriEXT) return;
    trigger_glMultiTexParameteriEXT = true;
    printf("Stub: glMultiTexParameteriEXT\n");
}
static bool trigger_glMultiTexParameterivEXT = false;
void stub_glMultiTexParameterivEXT() {
    if(trigger_glMultiTexParameterivEXT) return;
    trigger_glMultiTexParameterivEXT = true;
    printf("Stub: glMultiTexParameterivEXT\n");
}
static bool trigger_glMultiTexParameterfEXT = false;
void stub_glMultiTexParameterfEXT() {
    if(trigger_glMultiTexParameterfEXT) return;
    trigger_glMultiTexParameterfEXT = true;
    printf("Stub: glMultiTexParameterfEXT\n");
}
static bool trigger_glMultiTexParameterfvEXT = false;
void stub_glMultiTexParameterfvEXT() {
    if(trigger_glMultiTexParameterfvEXT) return;
    trigger_glMultiTexParameterfvEXT = true;
    printf("Stub: glMultiTexParameterfvEXT\n");
}
static bool trigger_glMultiTexImage1DEXT = false;
void stub_glMultiTexImage1DEXT() {
    if(trigger_glMultiTexImage1DEXT) return;
    trigger_glMultiTexImage1DEXT = true;
    printf("Stub: glMultiTexImage1DEXT\n");
}
static bool trigger_glMultiTexImage2DEXT = false;
void stub_glMultiTexImage2DEXT() {
    if(trigger_glMultiTexImage2DEXT) return;
    trigger_glMultiTexImage2DEXT = true;
    printf("Stub: glMultiTexImage2DEXT\n");
}
static bool trigger_glMultiTexSubImage1DEXT = false;
void stub_glMultiTexSubImage1DEXT() {
    if(trigger_glMultiTexSubImage1DEXT) return;
    trigger_glMultiTexSubImage1DEXT = true;
    printf("Stub: glMultiTexSubImage1DEXT\n");
}
static bool trigger_glMultiTexSubImage2DEXT = false;
void stub_glMultiTexSubImage2DEXT() {
    if(trigger_glMultiTexSubImage2DEXT) return;
    trigger_glMultiTexSubImage2DEXT = true;
    printf("Stub: glMultiTexSubImage2DEXT\n");
}
static bool trigger_glCopyMultiTexImage1DEXT = false;
void stub_glCopyMultiTexImage1DEXT() {
    if(trigger_glCopyMultiTexImage1DEXT) return;
    trigger_glCopyMultiTexImage1DEXT = true;
    printf("Stub: glCopyMultiTexImage1DEXT\n");
}
static bool trigger_glCopyMultiTexImage2DEXT = false;
void stub_glCopyMultiTexImage2DEXT() {
    if(trigger_glCopyMultiTexImage2DEXT) return;
    trigger_glCopyMultiTexImage2DEXT = true;
    printf("Stub: glCopyMultiTexImage2DEXT\n");
}
static bool trigger_glCopyMultiTexSubImage1DEXT = false;
void stub_glCopyMultiTexSubImage1DEXT() {
    if(trigger_glCopyMultiTexSubImage1DEXT) return;
    trigger_glCopyMultiTexSubImage1DEXT = true;
    printf("Stub: glCopyMultiTexSubImage1DEXT\n");
}
static bool trigger_glCopyMultiTexSubImage2DEXT = false;
void stub_glCopyMultiTexSubImage2DEXT() {
    if(trigger_glCopyMultiTexSubImage2DEXT) return;
    trigger_glCopyMultiTexSubImage2DEXT = true;
    printf("Stub: glCopyMultiTexSubImage2DEXT\n");
}
static bool trigger_glGetMultiTexImageEXT = false;
void stub_glGetMultiTexImageEXT() {
    if(trigger_glGetMultiTexImageEXT) return;
    trigger_glGetMultiTexImageEXT = true;
    printf("Stub: glGetMultiTexImageEXT\n");
}
static bool trigger_glGetMultiTexParameterfvEXT = false;
void stub_glGetMultiTexParameterfvEXT() {
    if(trigger_glGetMultiTexParameterfvEXT) return;
    trigger_glGetMultiTexParameterfvEXT = true;
    printf("Stub: glGetMultiTexParameterfvEXT\n");
}
static bool trigger_glGetMultiTexParameterivEXT = false;
void stub_glGetMultiTexParameterivEXT() {
    if(trigger_glGetMultiTexParameterivEXT) return;
    trigger_glGetMultiTexParameterivEXT = true;
    printf("Stub: glGetMultiTexParameterivEXT\n");
}
static bool trigger_glGetMultiTexLevelParameterfvEXT = false;
void stub_glGetMultiTexLevelParameterfvEXT() {
    if(trigger_glGetMultiTexLevelParameterfvEXT) return;
    trigger_glGetMultiTexLevelParameterfvEXT = true;
    printf("Stub: glGetMultiTexLevelParameterfvEXT\n");
}
static bool trigger_glGetMultiTexLevelParameterivEXT = false;
void stub_glGetMultiTexLevelParameterivEXT() {
    if(trigger_glGetMultiTexLevelParameterivEXT) return;
    trigger_glGetMultiTexLevelParameterivEXT = true;
    printf("Stub: glGetMultiTexLevelParameterivEXT\n");
}
static bool trigger_glMultiTexImage3DEXT = false;
void stub_glMultiTexImage3DEXT() {
    if(trigger_glMultiTexImage3DEXT) return;
    trigger_glMultiTexImage3DEXT = true;
    printf("Stub: glMultiTexImage3DEXT\n");
}
static bool trigger_glMultiTexSubImage3DEXT = false;
void stub_glMultiTexSubImage3DEXT() {
    if(trigger_glMultiTexSubImage3DEXT) return;
    trigger_glMultiTexSubImage3DEXT = true;
    printf("Stub: glMultiTexSubImage3DEXT\n");
}
static bool trigger_glCopyMultiTexSubImage3DEXT = false;
void stub_glCopyMultiTexSubImage3DEXT() {
    if(trigger_glCopyMultiTexSubImage3DEXT) return;
    trigger_glCopyMultiTexSubImage3DEXT = true;
    printf("Stub: glCopyMultiTexSubImage3DEXT\n");
}
static bool trigger_glEnableClientStateIndexedEXT = false;
void stub_glEnableClientStateIndexedEXT() {
    if(trigger_glEnableClientStateIndexedEXT) return;
    trigger_glEnableClientStateIndexedEXT = true;
    printf("Stub: glEnableClientStateIndexedEXT\n");
}
static bool trigger_glDisableClientStateIndexedEXT = false;
void stub_glDisableClientStateIndexedEXT() {
    if(trigger_glDisableClientStateIndexedEXT) return;
    trigger_glDisableClientStateIndexedEXT = true;
    printf("Stub: glDisableClientStateIndexedEXT\n");
}
static bool trigger_glGetFloatIndexedvEXT = false;
void stub_glGetFloatIndexedvEXT() {
    if(trigger_glGetFloatIndexedvEXT) return;
    trigger_glGetFloatIndexedvEXT = true;
    printf("Stub: glGetFloatIndexedvEXT\n");
}
static bool trigger_glGetDoubleIndexedvEXT = false;
void stub_glGetDoubleIndexedvEXT() {
    if(trigger_glGetDoubleIndexedvEXT) return;
    trigger_glGetDoubleIndexedvEXT = true;
    printf("Stub: glGetDoubleIndexedvEXT\n");
}
static bool trigger_glGetPointerIndexedvEXT = false;
void stub_glGetPointerIndexedvEXT() {
    if(trigger_glGetPointerIndexedvEXT) return;
    trigger_glGetPointerIndexedvEXT = true;
    printf("Stub: glGetPointerIndexedvEXT\n");
}
static bool trigger_glEnableIndexedEXT = false;
void stub_glEnableIndexedEXT() {
    if(trigger_glEnableIndexedEXT) return;
    trigger_glEnableIndexedEXT = true;
    printf("Stub: glEnableIndexedEXT\n");
}
static bool trigger_glDisableIndexedEXT = false;
void stub_glDisableIndexedEXT() {
    if(trigger_glDisableIndexedEXT) return;
    trigger_glDisableIndexedEXT = true;
    printf("Stub: glDisableIndexedEXT\n");
}
static bool trigger_glIsEnabledIndexedEXT = false;
void stub_glIsEnabledIndexedEXT() {
    if(trigger_glIsEnabledIndexedEXT) return;
    trigger_glIsEnabledIndexedEXT = true;
    printf("Stub: glIsEnabledIndexedEXT\n");
}
static bool trigger_glGetIntegerIndexedvEXT = false;
void stub_glGetIntegerIndexedvEXT() {
    if(trigger_glGetIntegerIndexedvEXT) return;
    trigger_glGetIntegerIndexedvEXT = true;
    printf("Stub: glGetIntegerIndexedvEXT\n");
}
static bool trigger_glGetBooleanIndexedvEXT = false;
void stub_glGetBooleanIndexedvEXT() {
    if(trigger_glGetBooleanIndexedvEXT) return;
    trigger_glGetBooleanIndexedvEXT = true;
    printf("Stub: glGetBooleanIndexedvEXT\n");
}
static bool trigger_glCompressedTextureImage3DEXT = false;
void stub_glCompressedTextureImage3DEXT() {
    if(trigger_glCompressedTextureImage3DEXT) return;
    trigger_glCompressedTextureImage3DEXT = true;
    printf("Stub: glCompressedTextureImage3DEXT\n");
}
static bool trigger_glCompressedTextureImage2DEXT = false;
void stub_glCompressedTextureImage2DEXT() {
    if(trigger_glCompressedTextureImage2DEXT) return;
    trigger_glCompressedTextureImage2DEXT = true;
    printf("Stub: glCompressedTextureImage2DEXT\n");
}
static bool trigger_glCompressedTextureImage1DEXT = false;
void stub_glCompressedTextureImage1DEXT() {
    if(trigger_glCompressedTextureImage1DEXT) return;
    trigger_glCompressedTextureImage1DEXT = true;
    printf("Stub: glCompressedTextureImage1DEXT\n");
}
static bool trigger_glCompressedTextureSubImage3DEXT = false;
void stub_glCompressedTextureSubImage3DEXT() {
    if(trigger_glCompressedTextureSubImage3DEXT) return;
    trigger_glCompressedTextureSubImage3DEXT = true;
    printf("Stub: glCompressedTextureSubImage3DEXT\n");
}
static bool trigger_glCompressedTextureSubImage2DEXT = false;
void stub_glCompressedTextureSubImage2DEXT() {
    if(trigger_glCompressedTextureSubImage2DEXT) return;
    trigger_glCompressedTextureSubImage2DEXT = true;
    printf("Stub: glCompressedTextureSubImage2DEXT\n");
}
static bool trigger_glCompressedTextureSubImage1DEXT = false;
void stub_glCompressedTextureSubImage1DEXT() {
    if(trigger_glCompressedTextureSubImage1DEXT) return;
    trigger_glCompressedTextureSubImage1DEXT = true;
    printf("Stub: glCompressedTextureSubImage1DEXT\n");
}
static bool trigger_glGetCompressedTextureImageEXT = false;
void stub_glGetCompressedTextureImageEXT() {
    if(trigger_glGetCompressedTextureImageEXT) return;
    trigger_glGetCompressedTextureImageEXT = true;
    printf("Stub: glGetCompressedTextureImageEXT\n");
}
static bool trigger_glCompressedMultiTexImage3DEXT = false;
void stub_glCompressedMultiTexImage3DEXT() {
    if(trigger_glCompressedMultiTexImage3DEXT) return;
    trigger_glCompressedMultiTexImage3DEXT = true;
    printf("Stub: glCompressedMultiTexImage3DEXT\n");
}
static bool trigger_glCompressedMultiTexImage2DEXT = false;
void stub_glCompressedMultiTexImage2DEXT() {
    if(trigger_glCompressedMultiTexImage2DEXT) return;
    trigger_glCompressedMultiTexImage2DEXT = true;
    printf("Stub: glCompressedMultiTexImage2DEXT\n");
}
static bool trigger_glCompressedMultiTexImage1DEXT = false;
void stub_glCompressedMultiTexImage1DEXT() {
    if(trigger_glCompressedMultiTexImage1DEXT) return;
    trigger_glCompressedMultiTexImage1DEXT = true;
    printf("Stub: glCompressedMultiTexImage1DEXT\n");
}
static bool trigger_glCompressedMultiTexSubImage3DEXT = false;
void stub_glCompressedMultiTexSubImage3DEXT() {
    if(trigger_glCompressedMultiTexSubImage3DEXT) return;
    trigger_glCompressedMultiTexSubImage3DEXT = true;
    printf("Stub: glCompressedMultiTexSubImage3DEXT\n");
}
static bool trigger_glCompressedMultiTexSubImage2DEXT = false;
void stub_glCompressedMultiTexSubImage2DEXT() {
    if(trigger_glCompressedMultiTexSubImage2DEXT) return;
    trigger_glCompressedMultiTexSubImage2DEXT = true;
    printf("Stub: glCompressedMultiTexSubImage2DEXT\n");
}
static bool trigger_glCompressedMultiTexSubImage1DEXT = false;
void stub_glCompressedMultiTexSubImage1DEXT() {
    if(trigger_glCompressedMultiTexSubImage1DEXT) return;
    trigger_glCompressedMultiTexSubImage1DEXT = true;
    printf("Stub: glCompressedMultiTexSubImage1DEXT\n");
}
static bool trigger_glGetCompressedMultiTexImageEXT = false;
void stub_glGetCompressedMultiTexImageEXT() {
    if(trigger_glGetCompressedMultiTexImageEXT) return;
    trigger_glGetCompressedMultiTexImageEXT = true;
    printf("Stub: glGetCompressedMultiTexImageEXT\n");
}
static bool trigger_glMatrixLoadTransposefEXT = false;
void stub_glMatrixLoadTransposefEXT() {
    if(trigger_glMatrixLoadTransposefEXT) return;
    trigger_glMatrixLoadTransposefEXT = true;
    printf("Stub: glMatrixLoadTransposefEXT\n");
}
static bool trigger_glMatrixLoadTransposedEXT = false;
void stub_glMatrixLoadTransposedEXT() {
    if(trigger_glMatrixLoadTransposedEXT) return;
    trigger_glMatrixLoadTransposedEXT = true;
    printf("Stub: glMatrixLoadTransposedEXT\n");
}
static bool trigger_glMatrixMultTransposefEXT = false;
void stub_glMatrixMultTransposefEXT() {
    if(trigger_glMatrixMultTransposefEXT) return;
    trigger_glMatrixMultTransposefEXT = true;
    printf("Stub: glMatrixMultTransposefEXT\n");
}
static bool trigger_glMatrixMultTransposedEXT = false;
void stub_glMatrixMultTransposedEXT() {
    if(trigger_glMatrixMultTransposedEXT) return;
    trigger_glMatrixMultTransposedEXT = true;
    printf("Stub: glMatrixMultTransposedEXT\n");
}
static bool trigger_glNamedBufferDataEXT = false;
void stub_glNamedBufferDataEXT() {
    if(trigger_glNamedBufferDataEXT) return;
    trigger_glNamedBufferDataEXT = true;
    printf("Stub: glNamedBufferDataEXT\n");
}
static bool trigger_glNamedBufferSubDataEXT = false;
void stub_glNamedBufferSubDataEXT() {
    if(trigger_glNamedBufferSubDataEXT) return;
    trigger_glNamedBufferSubDataEXT = true;
    printf("Stub: glNamedBufferSubDataEXT\n");
}
static bool trigger_glMapNamedBufferEXT = false;
void stub_glMapNamedBufferEXT() {
    if(trigger_glMapNamedBufferEXT) return;
    trigger_glMapNamedBufferEXT = true;
    printf("Stub: glMapNamedBufferEXT\n");
}
static bool trigger_glUnmapNamedBufferEXT = false;
void stub_glUnmapNamedBufferEXT() {
    if(trigger_glUnmapNamedBufferEXT) return;
    trigger_glUnmapNamedBufferEXT = true;
    printf("Stub: glUnmapNamedBufferEXT\n");
}
static bool trigger_glGetNamedBufferParameterivEXT = false;
void stub_glGetNamedBufferParameterivEXT() {
    if(trigger_glGetNamedBufferParameterivEXT) return;
    trigger_glGetNamedBufferParameterivEXT = true;
    printf("Stub: glGetNamedBufferParameterivEXT\n");
}
static bool trigger_glGetNamedBufferPointervEXT = false;
void stub_glGetNamedBufferPointervEXT() {
    if(trigger_glGetNamedBufferPointervEXT) return;
    trigger_glGetNamedBufferPointervEXT = true;
    printf("Stub: glGetNamedBufferPointervEXT\n");
}
static bool trigger_glGetNamedBufferSubDataEXT = false;
void stub_glGetNamedBufferSubDataEXT() {
    if(trigger_glGetNamedBufferSubDataEXT) return;
    trigger_glGetNamedBufferSubDataEXT = true;
    printf("Stub: glGetNamedBufferSubDataEXT\n");
}
static bool trigger_glProgramUniform1fEXT = false;
void stub_glProgramUniform1fEXT() {
    if(trigger_glProgramUniform1fEXT) return;
    trigger_glProgramUniform1fEXT = true;
    printf("Stub: glProgramUniform1fEXT\n");
}
static bool trigger_glProgramUniform2fEXT = false;
void stub_glProgramUniform2fEXT() {
    if(trigger_glProgramUniform2fEXT) return;
    trigger_glProgramUniform2fEXT = true;
    printf("Stub: glProgramUniform2fEXT\n");
}
static bool trigger_glProgramUniform3fEXT = false;
void stub_glProgramUniform3fEXT() {
    if(trigger_glProgramUniform3fEXT) return;
    trigger_glProgramUniform3fEXT = true;
    printf("Stub: glProgramUniform3fEXT\n");
}
static bool trigger_glProgramUniform4fEXT = false;
void stub_glProgramUniform4fEXT() {
    if(trigger_glProgramUniform4fEXT) return;
    trigger_glProgramUniform4fEXT = true;
    printf("Stub: glProgramUniform4fEXT\n");
}
static bool trigger_glProgramUniform1iEXT = false;
void stub_glProgramUniform1iEXT() {
    if(trigger_glProgramUniform1iEXT) return;
    trigger_glProgramUniform1iEXT = true;
    printf("Stub: glProgramUniform1iEXT\n");
}
static bool trigger_glProgramUniform2iEXT = false;
void stub_glProgramUniform2iEXT() {
    if(trigger_glProgramUniform2iEXT) return;
    trigger_glProgramUniform2iEXT = true;
    printf("Stub: glProgramUniform2iEXT\n");
}
static bool trigger_glProgramUniform3iEXT = false;
void stub_glProgramUniform3iEXT() {
    if(trigger_glProgramUniform3iEXT) return;
    trigger_glProgramUniform3iEXT = true;
    printf("Stub: glProgramUniform3iEXT\n");
}
static bool trigger_glProgramUniform4iEXT = false;
void stub_glProgramUniform4iEXT() {
    if(trigger_glProgramUniform4iEXT) return;
    trigger_glProgramUniform4iEXT = true;
    printf("Stub: glProgramUniform4iEXT\n");
}
static bool trigger_glProgramUniform1fvEXT = false;
void stub_glProgramUniform1fvEXT() {
    if(trigger_glProgramUniform1fvEXT) return;
    trigger_glProgramUniform1fvEXT = true;
    printf("Stub: glProgramUniform1fvEXT\n");
}
static bool trigger_glProgramUniform2fvEXT = false;
void stub_glProgramUniform2fvEXT() {
    if(trigger_glProgramUniform2fvEXT) return;
    trigger_glProgramUniform2fvEXT = true;
    printf("Stub: glProgramUniform2fvEXT\n");
}
static bool trigger_glProgramUniform3fvEXT = false;
void stub_glProgramUniform3fvEXT() {
    if(trigger_glProgramUniform3fvEXT) return;
    trigger_glProgramUniform3fvEXT = true;
    printf("Stub: glProgramUniform3fvEXT\n");
}
static bool trigger_glProgramUniform4fvEXT = false;
void stub_glProgramUniform4fvEXT() {
    if(trigger_glProgramUniform4fvEXT) return;
    trigger_glProgramUniform4fvEXT = true;
    printf("Stub: glProgramUniform4fvEXT\n");
}
static bool trigger_glProgramUniform1ivEXT = false;
void stub_glProgramUniform1ivEXT() {
    if(trigger_glProgramUniform1ivEXT) return;
    trigger_glProgramUniform1ivEXT = true;
    printf("Stub: glProgramUniform1ivEXT\n");
}
static bool trigger_glProgramUniform2ivEXT = false;
void stub_glProgramUniform2ivEXT() {
    if(trigger_glProgramUniform2ivEXT) return;
    trigger_glProgramUniform2ivEXT = true;
    printf("Stub: glProgramUniform2ivEXT\n");
}
static bool trigger_glProgramUniform3ivEXT = false;
void stub_glProgramUniform3ivEXT() {
    if(trigger_glProgramUniform3ivEXT) return;
    trigger_glProgramUniform3ivEXT = true;
    printf("Stub: glProgramUniform3ivEXT\n");
}
static bool trigger_glProgramUniform4ivEXT = false;
void stub_glProgramUniform4ivEXT() {
    if(trigger_glProgramUniform4ivEXT) return;
    trigger_glProgramUniform4ivEXT = true;
    printf("Stub: glProgramUniform4ivEXT\n");
}
static bool trigger_glProgramUniformMatrix2fvEXT = false;
void stub_glProgramUniformMatrix2fvEXT() {
    if(trigger_glProgramUniformMatrix2fvEXT) return;
    trigger_glProgramUniformMatrix2fvEXT = true;
    printf("Stub: glProgramUniformMatrix2fvEXT\n");
}
static bool trigger_glProgramUniformMatrix3fvEXT = false;
void stub_glProgramUniformMatrix3fvEXT() {
    if(trigger_glProgramUniformMatrix3fvEXT) return;
    trigger_glProgramUniformMatrix3fvEXT = true;
    printf("Stub: glProgramUniformMatrix3fvEXT\n");
}
static bool trigger_glProgramUniformMatrix4fvEXT = false;
void stub_glProgramUniformMatrix4fvEXT() {
    if(trigger_glProgramUniformMatrix4fvEXT) return;
    trigger_glProgramUniformMatrix4fvEXT = true;
    printf("Stub: glProgramUniformMatrix4fvEXT\n");
}
static bool trigger_glProgramUniformMatrix2x3fvEXT = false;
void stub_glProgramUniformMatrix2x3fvEXT() {
    if(trigger_glProgramUniformMatrix2x3fvEXT) return;
    trigger_glProgramUniformMatrix2x3fvEXT = true;
    printf("Stub: glProgramUniformMatrix2x3fvEXT\n");
}
static bool trigger_glProgramUniformMatrix3x2fvEXT = false;
void stub_glProgramUniformMatrix3x2fvEXT() {
    if(trigger_glProgramUniformMatrix3x2fvEXT) return;
    trigger_glProgramUniformMatrix3x2fvEXT = true;
    printf("Stub: glProgramUniformMatrix3x2fvEXT\n");
}
static bool trigger_glProgramUniformMatrix2x4fvEXT = false;
void stub_glProgramUniformMatrix2x4fvEXT() {
    if(trigger_glProgramUniformMatrix2x4fvEXT) return;
    trigger_glProgramUniformMatrix2x4fvEXT = true;
    printf("Stub: glProgramUniformMatrix2x4fvEXT\n");
}
static bool trigger_glProgramUniformMatrix4x2fvEXT = false;
void stub_glProgramUniformMatrix4x2fvEXT() {
    if(trigger_glProgramUniformMatrix4x2fvEXT) return;
    trigger_glProgramUniformMatrix4x2fvEXT = true;
    printf("Stub: glProgramUniformMatrix4x2fvEXT\n");
}
static bool trigger_glProgramUniformMatrix3x4fvEXT = false;
void stub_glProgramUniformMatrix3x4fvEXT() {
    if(trigger_glProgramUniformMatrix3x4fvEXT) return;
    trigger_glProgramUniformMatrix3x4fvEXT = true;
    printf("Stub: glProgramUniformMatrix3x4fvEXT\n");
}
static bool trigger_glProgramUniformMatrix4x3fvEXT = false;
void stub_glProgramUniformMatrix4x3fvEXT() {
    if(trigger_glProgramUniformMatrix4x3fvEXT) return;
    trigger_glProgramUniformMatrix4x3fvEXT = true;
    printf("Stub: glProgramUniformMatrix4x3fvEXT\n");
}
static bool trigger_glTextureBufferEXT = false;
void stub_glTextureBufferEXT() {
    if(trigger_glTextureBufferEXT) return;
    trigger_glTextureBufferEXT = true;
    printf("Stub: glTextureBufferEXT\n");
}
static bool trigger_glMultiTexBufferEXT = false;
void stub_glMultiTexBufferEXT() {
    if(trigger_glMultiTexBufferEXT) return;
    trigger_glMultiTexBufferEXT = true;
    printf("Stub: glMultiTexBufferEXT\n");
}
static bool trigger_glTextureParameterIivEXT = false;
void stub_glTextureParameterIivEXT() {
    if(trigger_glTextureParameterIivEXT) return;
    trigger_glTextureParameterIivEXT = true;
    printf("Stub: glTextureParameterIivEXT\n");
}
static bool trigger_glTextureParameterIuivEXT = false;
void stub_glTextureParameterIuivEXT() {
    if(trigger_glTextureParameterIuivEXT) return;
    trigger_glTextureParameterIuivEXT = true;
    printf("Stub: glTextureParameterIuivEXT\n");
}
static bool trigger_glGetTextureParameterIivEXT = false;
void stub_glGetTextureParameterIivEXT() {
    if(trigger_glGetTextureParameterIivEXT) return;
    trigger_glGetTextureParameterIivEXT = true;
    printf("Stub: glGetTextureParameterIivEXT\n");
}
static bool trigger_glGetTextureParameterIuivEXT = false;
void stub_glGetTextureParameterIuivEXT() {
    if(trigger_glGetTextureParameterIuivEXT) return;
    trigger_glGetTextureParameterIuivEXT = true;
    printf("Stub: glGetTextureParameterIuivEXT\n");
}
static bool trigger_glMultiTexParameterIivEXT = false;
void stub_glMultiTexParameterIivEXT() {
    if(trigger_glMultiTexParameterIivEXT) return;
    trigger_glMultiTexParameterIivEXT = true;
    printf("Stub: glMultiTexParameterIivEXT\n");
}
static bool trigger_glMultiTexParameterIuivEXT = false;
void stub_glMultiTexParameterIuivEXT() {
    if(trigger_glMultiTexParameterIuivEXT) return;
    trigger_glMultiTexParameterIuivEXT = true;
    printf("Stub: glMultiTexParameterIuivEXT\n");
}
static bool trigger_glGetMultiTexParameterIivEXT = false;
void stub_glGetMultiTexParameterIivEXT() {
    if(trigger_glGetMultiTexParameterIivEXT) return;
    trigger_glGetMultiTexParameterIivEXT = true;
    printf("Stub: glGetMultiTexParameterIivEXT\n");
}
static bool trigger_glGetMultiTexParameterIuivEXT = false;
void stub_glGetMultiTexParameterIuivEXT() {
    if(trigger_glGetMultiTexParameterIuivEXT) return;
    trigger_glGetMultiTexParameterIuivEXT = true;
    printf("Stub: glGetMultiTexParameterIuivEXT\n");
}
static bool trigger_glProgramUniform1uiEXT = false;
void stub_glProgramUniform1uiEXT() {
    if(trigger_glProgramUniform1uiEXT) return;
    trigger_glProgramUniform1uiEXT = true;
    printf("Stub: glProgramUniform1uiEXT\n");
}
static bool trigger_glProgramUniform2uiEXT = false;
void stub_glProgramUniform2uiEXT() {
    if(trigger_glProgramUniform2uiEXT) return;
    trigger_glProgramUniform2uiEXT = true;
    printf("Stub: glProgramUniform2uiEXT\n");
}
static bool trigger_glProgramUniform3uiEXT = false;
void stub_glProgramUniform3uiEXT() {
    if(trigger_glProgramUniform3uiEXT) return;
    trigger_glProgramUniform3uiEXT = true;
    printf("Stub: glProgramUniform3uiEXT\n");
}
static bool trigger_glProgramUniform4uiEXT = false;
void stub_glProgramUniform4uiEXT() {
    if(trigger_glProgramUniform4uiEXT) return;
    trigger_glProgramUniform4uiEXT = true;
    printf("Stub: glProgramUniform4uiEXT\n");
}
static bool trigger_glProgramUniform1uivEXT = false;
void stub_glProgramUniform1uivEXT() {
    if(trigger_glProgramUniform1uivEXT) return;
    trigger_glProgramUniform1uivEXT = true;
    printf("Stub: glProgramUniform1uivEXT\n");
}
static bool trigger_glProgramUniform2uivEXT = false;
void stub_glProgramUniform2uivEXT() {
    if(trigger_glProgramUniform2uivEXT) return;
    trigger_glProgramUniform2uivEXT = true;
    printf("Stub: glProgramUniform2uivEXT\n");
}
static bool trigger_glProgramUniform3uivEXT = false;
void stub_glProgramUniform3uivEXT() {
    if(trigger_glProgramUniform3uivEXT) return;
    trigger_glProgramUniform3uivEXT = true;
    printf("Stub: glProgramUniform3uivEXT\n");
}
static bool trigger_glProgramUniform4uivEXT = false;
void stub_glProgramUniform4uivEXT() {
    if(trigger_glProgramUniform4uivEXT) return;
    trigger_glProgramUniform4uivEXT = true;
    printf("Stub: glProgramUniform4uivEXT\n");
}
static bool trigger_glNamedProgramLocalParameters4fvEXT = false;
void stub_glNamedProgramLocalParameters4fvEXT() {
    if(trigger_glNamedProgramLocalParameters4fvEXT) return;
    trigger_glNamedProgramLocalParameters4fvEXT = true;
    printf("Stub: glNamedProgramLocalParameters4fvEXT\n");
}
static bool trigger_glNamedProgramLocalParameterI4iEXT = false;
void stub_glNamedProgramLocalParameterI4iEXT() {
    if(trigger_glNamedProgramLocalParameterI4iEXT) return;
    trigger_glNamedProgramLocalParameterI4iEXT = true;
    printf("Stub: glNamedProgramLocalParameterI4iEXT\n");
}
static bool trigger_glNamedProgramLocalParameterI4ivEXT = false;
void stub_glNamedProgramLocalParameterI4ivEXT() {
    if(trigger_glNamedProgramLocalParameterI4ivEXT) return;
    trigger_glNamedProgramLocalParameterI4ivEXT = true;
    printf("Stub: glNamedProgramLocalParameterI4ivEXT\n");
}
static bool trigger_glNamedProgramLocalParametersI4ivEXT = false;
void stub_glNamedProgramLocalParametersI4ivEXT() {
    if(trigger_glNamedProgramLocalParametersI4ivEXT) return;
    trigger_glNamedProgramLocalParametersI4ivEXT = true;
    printf("Stub: glNamedProgramLocalParametersI4ivEXT\n");
}
static bool trigger_glNamedProgramLocalParameterI4uiEXT = false;
void stub_glNamedProgramLocalParameterI4uiEXT() {
    if(trigger_glNamedProgramLocalParameterI4uiEXT) return;
    trigger_glNamedProgramLocalParameterI4uiEXT = true;
    printf("Stub: glNamedProgramLocalParameterI4uiEXT\n");
}
static bool trigger_glNamedProgramLocalParameterI4uivEXT = false;
void stub_glNamedProgramLocalParameterI4uivEXT() {
    if(trigger_glNamedProgramLocalParameterI4uivEXT) return;
    trigger_glNamedProgramLocalParameterI4uivEXT = true;
    printf("Stub: glNamedProgramLocalParameterI4uivEXT\n");
}
static bool trigger_glNamedProgramLocalParametersI4uivEXT = false;
void stub_glNamedProgramLocalParametersI4uivEXT() {
    if(trigger_glNamedProgramLocalParametersI4uivEXT) return;
    trigger_glNamedProgramLocalParametersI4uivEXT = true;
    printf("Stub: glNamedProgramLocalParametersI4uivEXT\n");
}
static bool trigger_glGetNamedProgramLocalParameterIivEXT = false;
void stub_glGetNamedProgramLocalParameterIivEXT() {
    if(trigger_glGetNamedProgramLocalParameterIivEXT) return;
    trigger_glGetNamedProgramLocalParameterIivEXT = true;
    printf("Stub: glGetNamedProgramLocalParameterIivEXT\n");
}
static bool trigger_glGetNamedProgramLocalParameterIuivEXT = false;
void stub_glGetNamedProgramLocalParameterIuivEXT() {
    if(trigger_glGetNamedProgramLocalParameterIuivEXT) return;
    trigger_glGetNamedProgramLocalParameterIuivEXT = true;
    printf("Stub: glGetNamedProgramLocalParameterIuivEXT\n");
}
static bool trigger_glEnableClientStateiEXT = false;
void stub_glEnableClientStateiEXT() {
    if(trigger_glEnableClientStateiEXT) return;
    trigger_glEnableClientStateiEXT = true;
    printf("Stub: glEnableClientStateiEXT\n");
}
static bool trigger_glDisableClientStateiEXT = false;
void stub_glDisableClientStateiEXT() {
    if(trigger_glDisableClientStateiEXT) return;
    trigger_glDisableClientStateiEXT = true;
    printf("Stub: glDisableClientStateiEXT\n");
}
static bool trigger_glGetFloati_vEXT = false;
void stub_glGetFloati_vEXT() {
    if(trigger_glGetFloati_vEXT) return;
    trigger_glGetFloati_vEXT = true;
    printf("Stub: glGetFloati_vEXT\n");
}
static bool trigger_glGetDoublei_vEXT = false;
void stub_glGetDoublei_vEXT() {
    if(trigger_glGetDoublei_vEXT) return;
    trigger_glGetDoublei_vEXT = true;
    printf("Stub: glGetDoublei_vEXT\n");
}
static bool trigger_glGetPointeri_vEXT = false;
void stub_glGetPointeri_vEXT() {
    if(trigger_glGetPointeri_vEXT) return;
    trigger_glGetPointeri_vEXT = true;
    printf("Stub: glGetPointeri_vEXT\n");
}
static bool trigger_glNamedProgramStringEXT = false;
void stub_glNamedProgramStringEXT() {
    if(trigger_glNamedProgramStringEXT) return;
    trigger_glNamedProgramStringEXT = true;
    printf("Stub: glNamedProgramStringEXT\n");
}
static bool trigger_glNamedProgramLocalParameter4dEXT = false;
void stub_glNamedProgramLocalParameter4dEXT() {
    if(trigger_glNamedProgramLocalParameter4dEXT) return;
    trigger_glNamedProgramLocalParameter4dEXT = true;
    printf("Stub: glNamedProgramLocalParameter4dEXT\n");
}
static bool trigger_glNamedProgramLocalParameter4dvEXT = false;
void stub_glNamedProgramLocalParameter4dvEXT() {
    if(trigger_glNamedProgramLocalParameter4dvEXT) return;
    trigger_glNamedProgramLocalParameter4dvEXT = true;
    printf("Stub: glNamedProgramLocalParameter4dvEXT\n");
}
static bool trigger_glNamedProgramLocalParameter4fEXT = false;
void stub_glNamedProgramLocalParameter4fEXT() {
    if(trigger_glNamedProgramLocalParameter4fEXT) return;
    trigger_glNamedProgramLocalParameter4fEXT = true;
    printf("Stub: glNamedProgramLocalParameter4fEXT\n");
}
static bool trigger_glNamedProgramLocalParameter4fvEXT = false;
void stub_glNamedProgramLocalParameter4fvEXT() {
    if(trigger_glNamedProgramLocalParameter4fvEXT) return;
    trigger_glNamedProgramLocalParameter4fvEXT = true;
    printf("Stub: glNamedProgramLocalParameter4fvEXT\n");
}
static bool trigger_glGetNamedProgramLocalParameterdvEXT = false;
void stub_glGetNamedProgramLocalParameterdvEXT() {
    if(trigger_glGetNamedProgramLocalParameterdvEXT) return;
    trigger_glGetNamedProgramLocalParameterdvEXT = true;
    printf("Stub: glGetNamedProgramLocalParameterdvEXT\n");
}
static bool trigger_glGetNamedProgramLocalParameterfvEXT = false;
void stub_glGetNamedProgramLocalParameterfvEXT() {
    if(trigger_glGetNamedProgramLocalParameterfvEXT) return;
    trigger_glGetNamedProgramLocalParameterfvEXT = true;
    printf("Stub: glGetNamedProgramLocalParameterfvEXT\n");
}
static bool trigger_glGetNamedProgramivEXT = false;
void stub_glGetNamedProgramivEXT() {
    if(trigger_glGetNamedProgramivEXT) return;
    trigger_glGetNamedProgramivEXT = true;
    printf("Stub: glGetNamedProgramivEXT\n");
}
static bool trigger_glGetNamedProgramStringEXT = false;
void stub_glGetNamedProgramStringEXT() {
    if(trigger_glGetNamedProgramStringEXT) return;
    trigger_glGetNamedProgramStringEXT = true;
    printf("Stub: glGetNamedProgramStringEXT\n");
}
static bool trigger_glNamedRenderbufferStorageEXT = false;
void stub_glNamedRenderbufferStorageEXT() {
    if(trigger_glNamedRenderbufferStorageEXT) return;
    trigger_glNamedRenderbufferStorageEXT = true;
    printf("Stub: glNamedRenderbufferStorageEXT\n");
}
static bool trigger_glGetNamedRenderbufferParameterivEXT = false;
void stub_glGetNamedRenderbufferParameterivEXT() {
    if(trigger_glGetNamedRenderbufferParameterivEXT) return;
    trigger_glGetNamedRenderbufferParameterivEXT = true;
    printf("Stub: glGetNamedRenderbufferParameterivEXT\n");
}
static bool trigger_glNamedRenderbufferStorageMultisampleEXT = false;
void stub_glNamedRenderbufferStorageMultisampleEXT() {
    if(trigger_glNamedRenderbufferStorageMultisampleEXT) return;
    trigger_glNamedRenderbufferStorageMultisampleEXT = true;
    printf("Stub: glNamedRenderbufferStorageMultisampleEXT\n");
}
static bool trigger_glNamedRenderbufferStorageMultisampleCoverageEXT = false;
void stub_glNamedRenderbufferStorageMultisampleCoverageEXT() {
    if(trigger_glNamedRenderbufferStorageMultisampleCoverageEXT) return;
    trigger_glNamedRenderbufferStorageMultisampleCoverageEXT = true;
    printf("Stub: glNamedRenderbufferStorageMultisampleCoverageEXT\n");
}
static bool trigger_glCheckNamedFramebufferStatusEXT = false;
void stub_glCheckNamedFramebufferStatusEXT() {
    if(trigger_glCheckNamedFramebufferStatusEXT) return;
    trigger_glCheckNamedFramebufferStatusEXT = true;
    printf("Stub: glCheckNamedFramebufferStatusEXT\n");
}
static bool trigger_glNamedFramebufferTexture1DEXT = false;
void stub_glNamedFramebufferTexture1DEXT() {
    if(trigger_glNamedFramebufferTexture1DEXT) return;
    trigger_glNamedFramebufferTexture1DEXT = true;
    printf("Stub: glNamedFramebufferTexture1DEXT\n");
}
static bool trigger_glNamedFramebufferTexture2DEXT = false;
void stub_glNamedFramebufferTexture2DEXT() {
    if(trigger_glNamedFramebufferTexture2DEXT) return;
    trigger_glNamedFramebufferTexture2DEXT = true;
    printf("Stub: glNamedFramebufferTexture2DEXT\n");
}
static bool trigger_glNamedFramebufferTexture3DEXT = false;
void stub_glNamedFramebufferTexture3DEXT() {
    if(trigger_glNamedFramebufferTexture3DEXT) return;
    trigger_glNamedFramebufferTexture3DEXT = true;
    printf("Stub: glNamedFramebufferTexture3DEXT\n");
}
static bool trigger_glNamedFramebufferRenderbufferEXT = false;
void stub_glNamedFramebufferRenderbufferEXT() {
    if(trigger_glNamedFramebufferRenderbufferEXT) return;
    trigger_glNamedFramebufferRenderbufferEXT = true;
    printf("Stub: glNamedFramebufferRenderbufferEXT\n");
}
static bool trigger_glGetNamedFramebufferAttachmentParameterivEXT = false;
void stub_glGetNamedFramebufferAttachmentParameterivEXT() {
    if(trigger_glGetNamedFramebufferAttachmentParameterivEXT) return;
    trigger_glGetNamedFramebufferAttachmentParameterivEXT = true;
    printf("Stub: glGetNamedFramebufferAttachmentParameterivEXT\n");
}
static bool trigger_glGenerateTextureMipmapEXT = false;
void stub_glGenerateTextureMipmapEXT() {
    if(trigger_glGenerateTextureMipmapEXT) return;
    trigger_glGenerateTextureMipmapEXT = true;
    printf("Stub: glGenerateTextureMipmapEXT\n");
}
static bool trigger_glGenerateMultiTexMipmapEXT = false;
void stub_glGenerateMultiTexMipmapEXT() {
    if(trigger_glGenerateMultiTexMipmapEXT) return;
    trigger_glGenerateMultiTexMipmapEXT = true;
    printf("Stub: glGenerateMultiTexMipmapEXT\n");
}
static bool trigger_glFramebufferDrawBufferEXT = false;
void stub_glFramebufferDrawBufferEXT() {
    if(trigger_glFramebufferDrawBufferEXT) return;
    trigger_glFramebufferDrawBufferEXT = true;
    printf("Stub: glFramebufferDrawBufferEXT\n");
}
static bool trigger_glFramebufferDrawBuffersEXT = false;
void stub_glFramebufferDrawBuffersEXT() {
    if(trigger_glFramebufferDrawBuffersEXT) return;
    trigger_glFramebufferDrawBuffersEXT = true;
    printf("Stub: glFramebufferDrawBuffersEXT\n");
}
static bool trigger_glFramebufferReadBufferEXT = false;
void stub_glFramebufferReadBufferEXT() {
    if(trigger_glFramebufferReadBufferEXT) return;
    trigger_glFramebufferReadBufferEXT = true;
    printf("Stub: glFramebufferReadBufferEXT\n");
}
static bool trigger_glGetFramebufferParameterivEXT = false;
void stub_glGetFramebufferParameterivEXT() {
    if(trigger_glGetFramebufferParameterivEXT) return;
    trigger_glGetFramebufferParameterivEXT = true;
    printf("Stub: glGetFramebufferParameterivEXT\n");
}
static bool trigger_glNamedCopyBufferSubDataEXT = false;
void stub_glNamedCopyBufferSubDataEXT() {
    if(trigger_glNamedCopyBufferSubDataEXT) return;
    trigger_glNamedCopyBufferSubDataEXT = true;
    printf("Stub: glNamedCopyBufferSubDataEXT\n");
}
static bool trigger_glNamedFramebufferTextureEXT = false;
void stub_glNamedFramebufferTextureEXT() {
    if(trigger_glNamedFramebufferTextureEXT) return;
    trigger_glNamedFramebufferTextureEXT = true;
    printf("Stub: glNamedFramebufferTextureEXT\n");
}
static bool trigger_glNamedFramebufferTextureLayerEXT = false;
void stub_glNamedFramebufferTextureLayerEXT() {
    if(trigger_glNamedFramebufferTextureLayerEXT) return;
    trigger_glNamedFramebufferTextureLayerEXT = true;
    printf("Stub: glNamedFramebufferTextureLayerEXT\n");
}
static bool trigger_glNamedFramebufferTextureFaceEXT = false;
void stub_glNamedFramebufferTextureFaceEXT() {
    if(trigger_glNamedFramebufferTextureFaceEXT) return;
    trigger_glNamedFramebufferTextureFaceEXT = true;
    printf("Stub: glNamedFramebufferTextureFaceEXT\n");
}
static bool trigger_glTextureRenderbufferEXT = false;
void stub_glTextureRenderbufferEXT() {
    if(trigger_glTextureRenderbufferEXT) return;
    trigger_glTextureRenderbufferEXT = true;
    printf("Stub: glTextureRenderbufferEXT\n");
}
static bool trigger_glMultiTexRenderbufferEXT = false;
void stub_glMultiTexRenderbufferEXT() {
    if(trigger_glMultiTexRenderbufferEXT) return;
    trigger_glMultiTexRenderbufferEXT = true;
    printf("Stub: glMultiTexRenderbufferEXT\n");
}
static bool trigger_glVertexArrayVertexOffsetEXT = false;
void stub_glVertexArrayVertexOffsetEXT() {
    if(trigger_glVertexArrayVertexOffsetEXT) return;
    trigger_glVertexArrayVertexOffsetEXT = true;
    printf("Stub: glVertexArrayVertexOffsetEXT\n");
}
static bool trigger_glVertexArrayColorOffsetEXT = false;
void stub_glVertexArrayColorOffsetEXT() {
    if(trigger_glVertexArrayColorOffsetEXT) return;
    trigger_glVertexArrayColorOffsetEXT = true;
    printf("Stub: glVertexArrayColorOffsetEXT\n");
}
static bool trigger_glVertexArrayEdgeFlagOffsetEXT = false;
void stub_glVertexArrayEdgeFlagOffsetEXT() {
    if(trigger_glVertexArrayEdgeFlagOffsetEXT) return;
    trigger_glVertexArrayEdgeFlagOffsetEXT = true;
    printf("Stub: glVertexArrayEdgeFlagOffsetEXT\n");
}
static bool trigger_glVertexArrayIndexOffsetEXT = false;
void stub_glVertexArrayIndexOffsetEXT() {
    if(trigger_glVertexArrayIndexOffsetEXT) return;
    trigger_glVertexArrayIndexOffsetEXT = true;
    printf("Stub: glVertexArrayIndexOffsetEXT\n");
}
static bool trigger_glVertexArrayNormalOffsetEXT = false;
void stub_glVertexArrayNormalOffsetEXT() {
    if(trigger_glVertexArrayNormalOffsetEXT) return;
    trigger_glVertexArrayNormalOffsetEXT = true;
    printf("Stub: glVertexArrayNormalOffsetEXT\n");
}
static bool trigger_glVertexArrayTexCoordOffsetEXT = false;
void stub_glVertexArrayTexCoordOffsetEXT() {
    if(trigger_glVertexArrayTexCoordOffsetEXT) return;
    trigger_glVertexArrayTexCoordOffsetEXT = true;
    printf("Stub: glVertexArrayTexCoordOffsetEXT\n");
}
static bool trigger_glVertexArrayMultiTexCoordOffsetEXT = false;
void stub_glVertexArrayMultiTexCoordOffsetEXT() {
    if(trigger_glVertexArrayMultiTexCoordOffsetEXT) return;
    trigger_glVertexArrayMultiTexCoordOffsetEXT = true;
    printf("Stub: glVertexArrayMultiTexCoordOffsetEXT\n");
}
static bool trigger_glVertexArrayFogCoordOffsetEXT = false;
void stub_glVertexArrayFogCoordOffsetEXT() {
    if(trigger_glVertexArrayFogCoordOffsetEXT) return;
    trigger_glVertexArrayFogCoordOffsetEXT = true;
    printf("Stub: glVertexArrayFogCoordOffsetEXT\n");
}
static bool trigger_glVertexArraySecondaryColorOffsetEXT = false;
void stub_glVertexArraySecondaryColorOffsetEXT() {
    if(trigger_glVertexArraySecondaryColorOffsetEXT) return;
    trigger_glVertexArraySecondaryColorOffsetEXT = true;
    printf("Stub: glVertexArraySecondaryColorOffsetEXT\n");
}
static bool trigger_glVertexArrayVertexAttribOffsetEXT = false;
void stub_glVertexArrayVertexAttribOffsetEXT() {
    if(trigger_glVertexArrayVertexAttribOffsetEXT) return;
    trigger_glVertexArrayVertexAttribOffsetEXT = true;
    printf("Stub: glVertexArrayVertexAttribOffsetEXT\n");
}
static bool trigger_glVertexArrayVertexAttribIOffsetEXT = false;
void stub_glVertexArrayVertexAttribIOffsetEXT() {
    if(trigger_glVertexArrayVertexAttribIOffsetEXT) return;
    trigger_glVertexArrayVertexAttribIOffsetEXT = true;
    printf("Stub: glVertexArrayVertexAttribIOffsetEXT\n");
}
static bool trigger_glEnableVertexArrayEXT = false;
void stub_glEnableVertexArrayEXT() {
    if(trigger_glEnableVertexArrayEXT) return;
    trigger_glEnableVertexArrayEXT = true;
    printf("Stub: glEnableVertexArrayEXT\n");
}
static bool trigger_glDisableVertexArrayEXT = false;
void stub_glDisableVertexArrayEXT() {
    if(trigger_glDisableVertexArrayEXT) return;
    trigger_glDisableVertexArrayEXT = true;
    printf("Stub: glDisableVertexArrayEXT\n");
}
static bool trigger_glEnableVertexArrayAttribEXT = false;
void stub_glEnableVertexArrayAttribEXT() {
    if(trigger_glEnableVertexArrayAttribEXT) return;
    trigger_glEnableVertexArrayAttribEXT = true;
    printf("Stub: glEnableVertexArrayAttribEXT\n");
}
static bool trigger_glDisableVertexArrayAttribEXT = false;
void stub_glDisableVertexArrayAttribEXT() {
    if(trigger_glDisableVertexArrayAttribEXT) return;
    trigger_glDisableVertexArrayAttribEXT = true;
    printf("Stub: glDisableVertexArrayAttribEXT\n");
}
static bool trigger_glGetVertexArrayIntegervEXT = false;
void stub_glGetVertexArrayIntegervEXT() {
    if(trigger_glGetVertexArrayIntegervEXT) return;
    trigger_glGetVertexArrayIntegervEXT = true;
    printf("Stub: glGetVertexArrayIntegervEXT\n");
}
static bool trigger_glGetVertexArrayPointervEXT = false;
void stub_glGetVertexArrayPointervEXT() {
    if(trigger_glGetVertexArrayPointervEXT) return;
    trigger_glGetVertexArrayPointervEXT = true;
    printf("Stub: glGetVertexArrayPointervEXT\n");
}
static bool trigger_glGetVertexArrayIntegeri_vEXT = false;
void stub_glGetVertexArrayIntegeri_vEXT() {
    if(trigger_glGetVertexArrayIntegeri_vEXT) return;
    trigger_glGetVertexArrayIntegeri_vEXT = true;
    printf("Stub: glGetVertexArrayIntegeri_vEXT\n");
}
static bool trigger_glGetVertexArrayPointeri_vEXT = false;
void stub_glGetVertexArrayPointeri_vEXT() {
    if(trigger_glGetVertexArrayPointeri_vEXT) return;
    trigger_glGetVertexArrayPointeri_vEXT = true;
    printf("Stub: glGetVertexArrayPointeri_vEXT\n");
}
static bool trigger_glMapNamedBufferRangeEXT = false;
void stub_glMapNamedBufferRangeEXT() {
    if(trigger_glMapNamedBufferRangeEXT) return;
    trigger_glMapNamedBufferRangeEXT = true;
    printf("Stub: glMapNamedBufferRangeEXT\n");
}
static bool trigger_glFlushMappedNamedBufferRangeEXT = false;
void stub_glFlushMappedNamedBufferRangeEXT() {
    if(trigger_glFlushMappedNamedBufferRangeEXT) return;
    trigger_glFlushMappedNamedBufferRangeEXT = true;
    printf("Stub: glFlushMappedNamedBufferRangeEXT\n");
}
static bool trigger_glNamedBufferStorageEXT = false;
void stub_glNamedBufferStorageEXT() {
    if(trigger_glNamedBufferStorageEXT) return;
    trigger_glNamedBufferStorageEXT = true;
    printf("Stub: glNamedBufferStorageEXT\n");
}
static bool trigger_glClearNamedBufferDataEXT = false;
void stub_glClearNamedBufferDataEXT() {
    if(trigger_glClearNamedBufferDataEXT) return;
    trigger_glClearNamedBufferDataEXT = true;
    printf("Stub: glClearNamedBufferDataEXT\n");
}
static bool trigger_glClearNamedBufferSubDataEXT = false;
void stub_glClearNamedBufferSubDataEXT() {
    if(trigger_glClearNamedBufferSubDataEXT) return;
    trigger_glClearNamedBufferSubDataEXT = true;
    printf("Stub: glClearNamedBufferSubDataEXT\n");
}
static bool trigger_glNamedFramebufferParameteriEXT = false;
void stub_glNamedFramebufferParameteriEXT() {
    if(trigger_glNamedFramebufferParameteriEXT) return;
    trigger_glNamedFramebufferParameteriEXT = true;
    printf("Stub: glNamedFramebufferParameteriEXT\n");
}
static bool trigger_glGetNamedFramebufferParameterivEXT = false;
void stub_glGetNamedFramebufferParameterivEXT() {
    if(trigger_glGetNamedFramebufferParameterivEXT) return;
    trigger_glGetNamedFramebufferParameterivEXT = true;
    printf("Stub: glGetNamedFramebufferParameterivEXT\n");
}
static bool trigger_glProgramUniform1dEXT = false;
void stub_glProgramUniform1dEXT() {
    if(trigger_glProgramUniform1dEXT) return;
    trigger_glProgramUniform1dEXT = true;
    printf("Stub: glProgramUniform1dEXT\n");
}
static bool trigger_glProgramUniform2dEXT = false;
void stub_glProgramUniform2dEXT() {
    if(trigger_glProgramUniform2dEXT) return;
    trigger_glProgramUniform2dEXT = true;
    printf("Stub: glProgramUniform2dEXT\n");
}
static bool trigger_glProgramUniform3dEXT = false;
void stub_glProgramUniform3dEXT() {
    if(trigger_glProgramUniform3dEXT) return;
    trigger_glProgramUniform3dEXT = true;
    printf("Stub: glProgramUniform3dEXT\n");
}
static bool trigger_glProgramUniform4dEXT = false;
void stub_glProgramUniform4dEXT() {
    if(trigger_glProgramUniform4dEXT) return;
    trigger_glProgramUniform4dEXT = true;
    printf("Stub: glProgramUniform4dEXT\n");
}
static bool trigger_glProgramUniform1dvEXT = false;
void stub_glProgramUniform1dvEXT() {
    if(trigger_glProgramUniform1dvEXT) return;
    trigger_glProgramUniform1dvEXT = true;
    printf("Stub: glProgramUniform1dvEXT\n");
}
static bool trigger_glProgramUniform2dvEXT = false;
void stub_glProgramUniform2dvEXT() {
    if(trigger_glProgramUniform2dvEXT) return;
    trigger_glProgramUniform2dvEXT = true;
    printf("Stub: glProgramUniform2dvEXT\n");
}
static bool trigger_glProgramUniform3dvEXT = false;
void stub_glProgramUniform3dvEXT() {
    if(trigger_glProgramUniform3dvEXT) return;
    trigger_glProgramUniform3dvEXT = true;
    printf("Stub: glProgramUniform3dvEXT\n");
}
static bool trigger_glProgramUniform4dvEXT = false;
void stub_glProgramUniform4dvEXT() {
    if(trigger_glProgramUniform4dvEXT) return;
    trigger_glProgramUniform4dvEXT = true;
    printf("Stub: glProgramUniform4dvEXT\n");
}
static bool trigger_glProgramUniformMatrix2dvEXT = false;
void stub_glProgramUniformMatrix2dvEXT() {
    if(trigger_glProgramUniformMatrix2dvEXT) return;
    trigger_glProgramUniformMatrix2dvEXT = true;
    printf("Stub: glProgramUniformMatrix2dvEXT\n");
}
static bool trigger_glProgramUniformMatrix3dvEXT = false;
void stub_glProgramUniformMatrix3dvEXT() {
    if(trigger_glProgramUniformMatrix3dvEXT) return;
    trigger_glProgramUniformMatrix3dvEXT = true;
    printf("Stub: glProgramUniformMatrix3dvEXT\n");
}
static bool trigger_glProgramUniformMatrix4dvEXT = false;
void stub_glProgramUniformMatrix4dvEXT() {
    if(trigger_glProgramUniformMatrix4dvEXT) return;
    trigger_glProgramUniformMatrix4dvEXT = true;
    printf("Stub: glProgramUniformMatrix4dvEXT\n");
}
static bool trigger_glProgramUniformMatrix2x3dvEXT = false;
void stub_glProgramUniformMatrix2x3dvEXT() {
    if(trigger_glProgramUniformMatrix2x3dvEXT) return;
    trigger_glProgramUniformMatrix2x3dvEXT = true;
    printf("Stub: glProgramUniformMatrix2x3dvEXT\n");
}
static bool trigger_glProgramUniformMatrix2x4dvEXT = false;
void stub_glProgramUniformMatrix2x4dvEXT() {
    if(trigger_glProgramUniformMatrix2x4dvEXT) return;
    trigger_glProgramUniformMatrix2x4dvEXT = true;
    printf("Stub: glProgramUniformMatrix2x4dvEXT\n");
}
static bool trigger_glProgramUniformMatrix3x2dvEXT = false;
void stub_glProgramUniformMatrix3x2dvEXT() {
    if(trigger_glProgramUniformMatrix3x2dvEXT) return;
    trigger_glProgramUniformMatrix3x2dvEXT = true;
    printf("Stub: glProgramUniformMatrix3x2dvEXT\n");
}
static bool trigger_glProgramUniformMatrix3x4dvEXT = false;
void stub_glProgramUniformMatrix3x4dvEXT() {
    if(trigger_glProgramUniformMatrix3x4dvEXT) return;
    trigger_glProgramUniformMatrix3x4dvEXT = true;
    printf("Stub: glProgramUniformMatrix3x4dvEXT\n");
}
static bool trigger_glProgramUniformMatrix4x2dvEXT = false;
void stub_glProgramUniformMatrix4x2dvEXT() {
    if(trigger_glProgramUniformMatrix4x2dvEXT) return;
    trigger_glProgramUniformMatrix4x2dvEXT = true;
    printf("Stub: glProgramUniformMatrix4x2dvEXT\n");
}
static bool trigger_glProgramUniformMatrix4x3dvEXT = false;
void stub_glProgramUniformMatrix4x3dvEXT() {
    if(trigger_glProgramUniformMatrix4x3dvEXT) return;
    trigger_glProgramUniformMatrix4x3dvEXT = true;
    printf("Stub: glProgramUniformMatrix4x3dvEXT\n");
}
static bool trigger_glTextureBufferRangeEXT = false;
void stub_glTextureBufferRangeEXT() {
    if(trigger_glTextureBufferRangeEXT) return;
    trigger_glTextureBufferRangeEXT = true;
    printf("Stub: glTextureBufferRangeEXT\n");
}
static bool trigger_glTextureStorage1DEXT = false;
void stub_glTextureStorage1DEXT() {
    if(trigger_glTextureStorage1DEXT) return;
    trigger_glTextureStorage1DEXT = true;
    printf("Stub: glTextureStorage1DEXT\n");
}
static bool trigger_glTextureStorage2DEXT = false;
void stub_glTextureStorage2DEXT() {
    if(trigger_glTextureStorage2DEXT) return;
    trigger_glTextureStorage2DEXT = true;
    printf("Stub: glTextureStorage2DEXT\n");
}
static bool trigger_glTextureStorage3DEXT = false;
void stub_glTextureStorage3DEXT() {
    if(trigger_glTextureStorage3DEXT) return;
    trigger_glTextureStorage3DEXT = true;
    printf("Stub: glTextureStorage3DEXT\n");
}
static bool trigger_glTextureStorage2DMultisampleEXT = false;
void stub_glTextureStorage2DMultisampleEXT() {
    if(trigger_glTextureStorage2DMultisampleEXT) return;
    trigger_glTextureStorage2DMultisampleEXT = true;
    printf("Stub: glTextureStorage2DMultisampleEXT\n");
}
static bool trigger_glTextureStorage3DMultisampleEXT = false;
void stub_glTextureStorage3DMultisampleEXT() {
    if(trigger_glTextureStorage3DMultisampleEXT) return;
    trigger_glTextureStorage3DMultisampleEXT = true;
    printf("Stub: glTextureStorage3DMultisampleEXT\n");
}
static bool trigger_glVertexArrayBindVertexBufferEXT = false;
void stub_glVertexArrayBindVertexBufferEXT() {
    if(trigger_glVertexArrayBindVertexBufferEXT) return;
    trigger_glVertexArrayBindVertexBufferEXT = true;
    printf("Stub: glVertexArrayBindVertexBufferEXT\n");
}
static bool trigger_glVertexArrayVertexAttribFormatEXT = false;
void stub_glVertexArrayVertexAttribFormatEXT() {
    if(trigger_glVertexArrayVertexAttribFormatEXT) return;
    trigger_glVertexArrayVertexAttribFormatEXT = true;
    printf("Stub: glVertexArrayVertexAttribFormatEXT\n");
}
static bool trigger_glVertexArrayVertexAttribIFormatEXT = false;
void stub_glVertexArrayVertexAttribIFormatEXT() {
    if(trigger_glVertexArrayVertexAttribIFormatEXT) return;
    trigger_glVertexArrayVertexAttribIFormatEXT = true;
    printf("Stub: glVertexArrayVertexAttribIFormatEXT\n");
}
static bool trigger_glVertexArrayVertexAttribLFormatEXT = false;
void stub_glVertexArrayVertexAttribLFormatEXT() {
    if(trigger_glVertexArrayVertexAttribLFormatEXT) return;
    trigger_glVertexArrayVertexAttribLFormatEXT = true;
    printf("Stub: glVertexArrayVertexAttribLFormatEXT\n");
}
static bool trigger_glVertexArrayVertexAttribBindingEXT = false;
void stub_glVertexArrayVertexAttribBindingEXT() {
    if(trigger_glVertexArrayVertexAttribBindingEXT) return;
    trigger_glVertexArrayVertexAttribBindingEXT = true;
    printf("Stub: glVertexArrayVertexAttribBindingEXT\n");
}
static bool trigger_glVertexArrayVertexBindingDivisorEXT = false;
void stub_glVertexArrayVertexBindingDivisorEXT() {
    if(trigger_glVertexArrayVertexBindingDivisorEXT) return;
    trigger_glVertexArrayVertexBindingDivisorEXT = true;
    printf("Stub: glVertexArrayVertexBindingDivisorEXT\n");
}
static bool trigger_glVertexArrayVertexAttribLOffsetEXT = false;
void stub_glVertexArrayVertexAttribLOffsetEXT() {
    if(trigger_glVertexArrayVertexAttribLOffsetEXT) return;
    trigger_glVertexArrayVertexAttribLOffsetEXT = true;
    printf("Stub: glVertexArrayVertexAttribLOffsetEXT\n");
}
static bool trigger_glTexturePageCommitmentEXT = false;
void stub_glTexturePageCommitmentEXT() {
    if(trigger_glTexturePageCommitmentEXT) return;
    trigger_glTexturePageCommitmentEXT = true;
    printf("Stub: glTexturePageCommitmentEXT\n");
}
static bool trigger_glVertexArrayVertexAttribDivisorEXT = false;
void stub_glVertexArrayVertexAttribDivisorEXT() {
    if(trigger_glVertexArrayVertexAttribDivisorEXT) return;
    trigger_glVertexArrayVertexAttribDivisorEXT = true;
    printf("Stub: glVertexArrayVertexAttribDivisorEXT\n");
}
static bool trigger_glDrawArraysInstancedEXT = false;
void stub_glDrawArraysInstancedEXT() {
    if(trigger_glDrawArraysInstancedEXT) return;
    trigger_glDrawArraysInstancedEXT = true;
    printf("Stub: glDrawArraysInstancedEXT\n");
}
static bool trigger_glDrawElementsInstancedEXT = false;
void stub_glDrawElementsInstancedEXT() {
    if(trigger_glDrawElementsInstancedEXT) return;
    trigger_glDrawElementsInstancedEXT = true;
    printf("Stub: glDrawElementsInstancedEXT\n");
}
static bool trigger_glPolygonOffsetClampEXT = false;
void stub_glPolygonOffsetClampEXT() {
    if(trigger_glPolygonOffsetClampEXT) return;
    trigger_glPolygonOffsetClampEXT = true;
    printf("Stub: glPolygonOffsetClampEXT\n");
}
static bool trigger_glRasterSamplesEXT = false;
void stub_glRasterSamplesEXT() {
    if(trigger_glRasterSamplesEXT) return;
    trigger_glRasterSamplesEXT = true;
    printf("Stub: glRasterSamplesEXT\n");
}
static bool trigger_glUseShaderProgramEXT = false;
void stub_glUseShaderProgramEXT() {
    if(trigger_glUseShaderProgramEXT) return;
    trigger_glUseShaderProgramEXT = true;
    printf("Stub: glUseShaderProgramEXT\n");
}
static bool trigger_glActiveProgramEXT = false;
void stub_glActiveProgramEXT() {
    if(trigger_glActiveProgramEXT) return;
    trigger_glActiveProgramEXT = true;
    printf("Stub: glActiveProgramEXT\n");
}
static bool trigger_glCreateShaderProgramEXT = false;
void stub_glCreateShaderProgramEXT() {
    if(trigger_glCreateShaderProgramEXT) return;
    trigger_glCreateShaderProgramEXT = true;
    printf("Stub: glCreateShaderProgramEXT\n");
}
static bool trigger_glFramebufferFetchBarrierEXT = false;
void stub_glFramebufferFetchBarrierEXT() {
    if(trigger_glFramebufferFetchBarrierEXT) return;
    trigger_glFramebufferFetchBarrierEXT = true;
    printf("Stub: glFramebufferFetchBarrierEXT\n");
}
static bool trigger_glTexStorage1DEXT = false;
void stub_glTexStorage1DEXT() {
    if(trigger_glTexStorage1DEXT) return;
    trigger_glTexStorage1DEXT = true;
    printf("Stub: glTexStorage1DEXT\n");
}
static bool trigger_glTexStorage2DEXT = false;
void stub_glTexStorage2DEXT() {
    if(trigger_glTexStorage2DEXT) return;
    trigger_glTexStorage2DEXT = true;
    printf("Stub: glTexStorage2DEXT\n");
}
static bool trigger_glTexStorage3DEXT = false;
void stub_glTexStorage3DEXT() {
    if(trigger_glTexStorage3DEXT) return;
    trigger_glTexStorage3DEXT = true;
    printf("Stub: glTexStorage3DEXT\n");
}
static bool trigger_glWindowRectanglesEXT = false;
void stub_glWindowRectanglesEXT() {
    if(trigger_glWindowRectanglesEXT) return;
    trigger_glWindowRectanglesEXT = true;
    printf("Stub: glWindowRectanglesEXT\n");
}
static bool trigger_glApplyFramebufferAttachmentCMAAINTEL = false;
void stub_glApplyFramebufferAttachmentCMAAINTEL() {
    if(trigger_glApplyFramebufferAttachmentCMAAINTEL) return;
    trigger_glApplyFramebufferAttachmentCMAAINTEL = true;
    printf("Stub: glApplyFramebufferAttachmentCMAAINTEL\n");
}
static bool trigger_glBeginPerfQueryINTEL = false;
void stub_glBeginPerfQueryINTEL() {
    if(trigger_glBeginPerfQueryINTEL) return;
    trigger_glBeginPerfQueryINTEL = true;
    printf("Stub: glBeginPerfQueryINTEL\n");
}
static bool trigger_glCreatePerfQueryINTEL = false;
void stub_glCreatePerfQueryINTEL() {
    if(trigger_glCreatePerfQueryINTEL) return;
    trigger_glCreatePerfQueryINTEL = true;
    printf("Stub: glCreatePerfQueryINTEL\n");
}
static bool trigger_glDeletePerfQueryINTEL = false;
void stub_glDeletePerfQueryINTEL() {
    if(trigger_glDeletePerfQueryINTEL) return;
    trigger_glDeletePerfQueryINTEL = true;
    printf("Stub: glDeletePerfQueryINTEL\n");
}
static bool trigger_glEndPerfQueryINTEL = false;
void stub_glEndPerfQueryINTEL() {
    if(trigger_glEndPerfQueryINTEL) return;
    trigger_glEndPerfQueryINTEL = true;
    printf("Stub: glEndPerfQueryINTEL\n");
}
static bool trigger_glGetFirstPerfQueryIdINTEL = false;
void stub_glGetFirstPerfQueryIdINTEL() {
    if(trigger_glGetFirstPerfQueryIdINTEL) return;
    trigger_glGetFirstPerfQueryIdINTEL = true;
    printf("Stub: glGetFirstPerfQueryIdINTEL\n");
}
static bool trigger_glGetNextPerfQueryIdINTEL = false;
void stub_glGetNextPerfQueryIdINTEL() {
    if(trigger_glGetNextPerfQueryIdINTEL) return;
    trigger_glGetNextPerfQueryIdINTEL = true;
    printf("Stub: glGetNextPerfQueryIdINTEL\n");
}
static bool trigger_glGetPerfCounterInfoINTEL = false;
void stub_glGetPerfCounterInfoINTEL() {
    if(trigger_glGetPerfCounterInfoINTEL) return;
    trigger_glGetPerfCounterInfoINTEL = true;
    printf("Stub: glGetPerfCounterInfoINTEL\n");
}
static bool trigger_glGetPerfQueryDataINTEL = false;
void stub_glGetPerfQueryDataINTEL() {
    if(trigger_glGetPerfQueryDataINTEL) return;
    trigger_glGetPerfQueryDataINTEL = true;
    printf("Stub: glGetPerfQueryDataINTEL\n");
}
static bool trigger_glGetPerfQueryIdByNameINTEL = false;
void stub_glGetPerfQueryIdByNameINTEL() {
    if(trigger_glGetPerfQueryIdByNameINTEL) return;
    trigger_glGetPerfQueryIdByNameINTEL = true;
    printf("Stub: glGetPerfQueryIdByNameINTEL\n");
}
static bool trigger_glGetPerfQueryInfoINTEL = false;
void stub_glGetPerfQueryInfoINTEL() {
    if(trigger_glGetPerfQueryInfoINTEL) return;
    trigger_glGetPerfQueryInfoINTEL = true;
    printf("Stub: glGetPerfQueryInfoINTEL\n");
}
static bool trigger_glFramebufferParameteriMESA = false;
void stub_glFramebufferParameteriMESA() {
    if(trigger_glFramebufferParameteriMESA) return;
    trigger_glFramebufferParameteriMESA = true;
    printf("Stub: glFramebufferParameteriMESA\n");
}
static bool trigger_glGetFramebufferParameterivMESA = false;
void stub_glGetFramebufferParameterivMESA() {
    if(trigger_glGetFramebufferParameterivMESA) return;
    trigger_glGetFramebufferParameterivMESA = true;
    printf("Stub: glGetFramebufferParameterivMESA\n");
}
static bool trigger_glMultiDrawArraysIndirectBindlessNV = false;
void stub_glMultiDrawArraysIndirectBindlessNV() {
    if(trigger_glMultiDrawArraysIndirectBindlessNV) return;
    trigger_glMultiDrawArraysIndirectBindlessNV = true;
    printf("Stub: glMultiDrawArraysIndirectBindlessNV\n");
}
static bool trigger_glMultiDrawElementsIndirectBindlessNV = false;
void stub_glMultiDrawElementsIndirectBindlessNV() {
    if(trigger_glMultiDrawElementsIndirectBindlessNV) return;
    trigger_glMultiDrawElementsIndirectBindlessNV = true;
    printf("Stub: glMultiDrawElementsIndirectBindlessNV\n");
}
static bool trigger_glMultiDrawArraysIndirectBindlessCountNV = false;
void stub_glMultiDrawArraysIndirectBindlessCountNV() {
    if(trigger_glMultiDrawArraysIndirectBindlessCountNV) return;
    trigger_glMultiDrawArraysIndirectBindlessCountNV = true;
    printf("Stub: glMultiDrawArraysIndirectBindlessCountNV\n");
}
static bool trigger_glMultiDrawElementsIndirectBindlessCountNV = false;
void stub_glMultiDrawElementsIndirectBindlessCountNV() {
    if(trigger_glMultiDrawElementsIndirectBindlessCountNV) return;
    trigger_glMultiDrawElementsIndirectBindlessCountNV = true;
    printf("Stub: glMultiDrawElementsIndirectBindlessCountNV\n");
}
static bool trigger_glGetTextureHandleNV = false;
void stub_glGetTextureHandleNV() {
    if(trigger_glGetTextureHandleNV) return;
    trigger_glGetTextureHandleNV = true;
    printf("Stub: glGetTextureHandleNV\n");
}
static bool trigger_glGetTextureSamplerHandleNV = false;
void stub_glGetTextureSamplerHandleNV() {
    if(trigger_glGetTextureSamplerHandleNV) return;
    trigger_glGetTextureSamplerHandleNV = true;
    printf("Stub: glGetTextureSamplerHandleNV\n");
}
static bool trigger_glMakeTextureHandleResidentNV = false;
void stub_glMakeTextureHandleResidentNV() {
    if(trigger_glMakeTextureHandleResidentNV) return;
    trigger_glMakeTextureHandleResidentNV = true;
    printf("Stub: glMakeTextureHandleResidentNV\n");
}
static bool trigger_glMakeTextureHandleNonResidentNV = false;
void stub_glMakeTextureHandleNonResidentNV() {
    if(trigger_glMakeTextureHandleNonResidentNV) return;
    trigger_glMakeTextureHandleNonResidentNV = true;
    printf("Stub: glMakeTextureHandleNonResidentNV\n");
}
static bool trigger_glGetImageHandleNV = false;
void stub_glGetImageHandleNV() {
    if(trigger_glGetImageHandleNV) return;
    trigger_glGetImageHandleNV = true;
    printf("Stub: glGetImageHandleNV\n");
}
static bool trigger_glMakeImageHandleResidentNV = false;
void stub_glMakeImageHandleResidentNV() {
    if(trigger_glMakeImageHandleResidentNV) return;
    trigger_glMakeImageHandleResidentNV = true;
    printf("Stub: glMakeImageHandleResidentNV\n");
}
static bool trigger_glMakeImageHandleNonResidentNV = false;
void stub_glMakeImageHandleNonResidentNV() {
    if(trigger_glMakeImageHandleNonResidentNV) return;
    trigger_glMakeImageHandleNonResidentNV = true;
    printf("Stub: glMakeImageHandleNonResidentNV\n");
}
static bool trigger_glUniformHandleui64NV = false;
void stub_glUniformHandleui64NV() {
    if(trigger_glUniformHandleui64NV) return;
    trigger_glUniformHandleui64NV = true;
    printf("Stub: glUniformHandleui64NV\n");
}
static bool trigger_glUniformHandleui64vNV = false;
void stub_glUniformHandleui64vNV() {
    if(trigger_glUniformHandleui64vNV) return;
    trigger_glUniformHandleui64vNV = true;
    printf("Stub: glUniformHandleui64vNV\n");
}
static bool trigger_glProgramUniformHandleui64NV = false;
void stub_glProgramUniformHandleui64NV() {
    if(trigger_glProgramUniformHandleui64NV) return;
    trigger_glProgramUniformHandleui64NV = true;
    printf("Stub: glProgramUniformHandleui64NV\n");
}
static bool trigger_glProgramUniformHandleui64vNV = false;
void stub_glProgramUniformHandleui64vNV() {
    if(trigger_glProgramUniformHandleui64vNV) return;
    trigger_glProgramUniformHandleui64vNV = true;
    printf("Stub: glProgramUniformHandleui64vNV\n");
}
static bool trigger_glIsTextureHandleResidentNV = false;
void stub_glIsTextureHandleResidentNV() {
    if(trigger_glIsTextureHandleResidentNV) return;
    trigger_glIsTextureHandleResidentNV = true;
    printf("Stub: glIsTextureHandleResidentNV\n");
}
static bool trigger_glIsImageHandleResidentNV = false;
void stub_glIsImageHandleResidentNV() {
    if(trigger_glIsImageHandleResidentNV) return;
    trigger_glIsImageHandleResidentNV = true;
    printf("Stub: glIsImageHandleResidentNV\n");
}
static bool trigger_glBlendParameteriNV = false;
void stub_glBlendParameteriNV() {
    if(trigger_glBlendParameteriNV) return;
    trigger_glBlendParameteriNV = true;
    printf("Stub: glBlendParameteriNV\n");
}
static bool trigger_glBlendBarrierNV = false;
void stub_glBlendBarrierNV() {
    if(trigger_glBlendBarrierNV) return;
    trigger_glBlendBarrierNV = true;
    printf("Stub: glBlendBarrierNV\n");
}
static bool trigger_glViewportPositionWScaleNV = false;
void stub_glViewportPositionWScaleNV() {
    if(trigger_glViewportPositionWScaleNV) return;
    trigger_glViewportPositionWScaleNV = true;
    printf("Stub: glViewportPositionWScaleNV\n");
}
static bool trigger_glCreateStatesNV = false;
void stub_glCreateStatesNV() {
    if(trigger_glCreateStatesNV) return;
    trigger_glCreateStatesNV = true;
    printf("Stub: glCreateStatesNV\n");
}
static bool trigger_glDeleteStatesNV = false;
void stub_glDeleteStatesNV() {
    if(trigger_glDeleteStatesNV) return;
    trigger_glDeleteStatesNV = true;
    printf("Stub: glDeleteStatesNV\n");
}
static bool trigger_glIsStateNV = false;
void stub_glIsStateNV() {
    if(trigger_glIsStateNV) return;
    trigger_glIsStateNV = true;
    printf("Stub: glIsStateNV\n");
}
static bool trigger_glStateCaptureNV = false;
void stub_glStateCaptureNV() {
    if(trigger_glStateCaptureNV) return;
    trigger_glStateCaptureNV = true;
    printf("Stub: glStateCaptureNV\n");
}
static bool trigger_glGetCommandHeaderNV = false;
void stub_glGetCommandHeaderNV() {
    if(trigger_glGetCommandHeaderNV) return;
    trigger_glGetCommandHeaderNV = true;
    printf("Stub: glGetCommandHeaderNV\n");
}
static bool trigger_glGetStageIndexNV = false;
void stub_glGetStageIndexNV() {
    if(trigger_glGetStageIndexNV) return;
    trigger_glGetStageIndexNV = true;
    printf("Stub: glGetStageIndexNV\n");
}
static bool trigger_glDrawCommandsNV = false;
void stub_glDrawCommandsNV() {
    if(trigger_glDrawCommandsNV) return;
    trigger_glDrawCommandsNV = true;
    printf("Stub: glDrawCommandsNV\n");
}
static bool trigger_glDrawCommandsAddressNV = false;
void stub_glDrawCommandsAddressNV() {
    if(trigger_glDrawCommandsAddressNV) return;
    trigger_glDrawCommandsAddressNV = true;
    printf("Stub: glDrawCommandsAddressNV\n");
}
static bool trigger_glDrawCommandsStatesNV = false;
void stub_glDrawCommandsStatesNV() {
    if(trigger_glDrawCommandsStatesNV) return;
    trigger_glDrawCommandsStatesNV = true;
    printf("Stub: glDrawCommandsStatesNV\n");
}
static bool trigger_glDrawCommandsStatesAddressNV = false;
void stub_glDrawCommandsStatesAddressNV() {
    if(trigger_glDrawCommandsStatesAddressNV) return;
    trigger_glDrawCommandsStatesAddressNV = true;
    printf("Stub: glDrawCommandsStatesAddressNV\n");
}
static bool trigger_glCreateCommandListsNV = false;
void stub_glCreateCommandListsNV() {
    if(trigger_glCreateCommandListsNV) return;
    trigger_glCreateCommandListsNV = true;
    printf("Stub: glCreateCommandListsNV\n");
}
static bool trigger_glDeleteCommandListsNV = false;
void stub_glDeleteCommandListsNV() {
    if(trigger_glDeleteCommandListsNV) return;
    trigger_glDeleteCommandListsNV = true;
    printf("Stub: glDeleteCommandListsNV\n");
}
static bool trigger_glIsCommandListNV = false;
void stub_glIsCommandListNV() {
    if(trigger_glIsCommandListNV) return;
    trigger_glIsCommandListNV = true;
    printf("Stub: glIsCommandListNV\n");
}
static bool trigger_glListDrawCommandsStatesClientNV = false;
void stub_glListDrawCommandsStatesClientNV() {
    if(trigger_glListDrawCommandsStatesClientNV) return;
    trigger_glListDrawCommandsStatesClientNV = true;
    printf("Stub: glListDrawCommandsStatesClientNV\n");
}
static bool trigger_glCommandListSegmentsNV = false;
void stub_glCommandListSegmentsNV() {
    if(trigger_glCommandListSegmentsNV) return;
    trigger_glCommandListSegmentsNV = true;
    printf("Stub: glCommandListSegmentsNV\n");
}
static bool trigger_glCompileCommandListNV = false;
void stub_glCompileCommandListNV() {
    if(trigger_glCompileCommandListNV) return;
    trigger_glCompileCommandListNV = true;
    printf("Stub: glCompileCommandListNV\n");
}
static bool trigger_glCallCommandListNV = false;
void stub_glCallCommandListNV() {
    if(trigger_glCallCommandListNV) return;
    trigger_glCallCommandListNV = true;
    printf("Stub: glCallCommandListNV\n");
}
static bool trigger_glBeginConditionalRenderNV = false;
void stub_glBeginConditionalRenderNV() {
    if(trigger_glBeginConditionalRenderNV) return;
    trigger_glBeginConditionalRenderNV = true;
    printf("Stub: glBeginConditionalRenderNV\n");
}
static bool trigger_glEndConditionalRenderNV = false;
void stub_glEndConditionalRenderNV() {
    if(trigger_glEndConditionalRenderNV) return;
    trigger_glEndConditionalRenderNV = true;
    printf("Stub: glEndConditionalRenderNV\n");
}
static bool trigger_glSubpixelPrecisionBiasNV = false;
void stub_glSubpixelPrecisionBiasNV() {
    if(trigger_glSubpixelPrecisionBiasNV) return;
    trigger_glSubpixelPrecisionBiasNV = true;
    printf("Stub: glSubpixelPrecisionBiasNV\n");
}
static bool trigger_glConservativeRasterParameterfNV = false;
void stub_glConservativeRasterParameterfNV() {
    if(trigger_glConservativeRasterParameterfNV) return;
    trigger_glConservativeRasterParameterfNV = true;
    printf("Stub: glConservativeRasterParameterfNV\n");
}
static bool trigger_glConservativeRasterParameteriNV = false;
void stub_glConservativeRasterParameteriNV() {
    if(trigger_glConservativeRasterParameteriNV) return;
    trigger_glConservativeRasterParameteriNV = true;
    printf("Stub: glConservativeRasterParameteriNV\n");
}
static bool trigger_glDepthRangedNV = false;
void stub_glDepthRangedNV() {
    if(trigger_glDepthRangedNV) return;
    trigger_glDepthRangedNV = true;
    printf("Stub: glDepthRangedNV\n");
}
static bool trigger_glClearDepthdNV = false;
void stub_glClearDepthdNV() {
    if(trigger_glClearDepthdNV) return;
    trigger_glClearDepthdNV = true;
    printf("Stub: glClearDepthdNV\n");
}
static bool trigger_glDepthBoundsdNV = false;
void stub_glDepthBoundsdNV() {
    if(trigger_glDepthBoundsdNV) return;
    trigger_glDepthBoundsdNV = true;
    printf("Stub: glDepthBoundsdNV\n");
}
static bool trigger_glDrawVkImageNV = false;
void stub_glDrawVkImageNV() {
    if(trigger_glDrawVkImageNV) return;
    trigger_glDrawVkImageNV = true;
    printf("Stub: glDrawVkImageNV\n");
}
static bool trigger_glGetVkProcAddrNV = false;
void stub_glGetVkProcAddrNV() {
    if(trigger_glGetVkProcAddrNV) return;
    trigger_glGetVkProcAddrNV = true;
    printf("Stub: glGetVkProcAddrNV\n");
}
static bool trigger_glWaitVkSemaphoreNV = false;
void stub_glWaitVkSemaphoreNV() {
    if(trigger_glWaitVkSemaphoreNV) return;
    trigger_glWaitVkSemaphoreNV = true;
    printf("Stub: glWaitVkSemaphoreNV\n");
}
static bool trigger_glSignalVkSemaphoreNV = false;
void stub_glSignalVkSemaphoreNV() {
    if(trigger_glSignalVkSemaphoreNV) return;
    trigger_glSignalVkSemaphoreNV = true;
    printf("Stub: glSignalVkSemaphoreNV\n");
}
static bool trigger_glSignalVkFenceNV = false;
void stub_glSignalVkFenceNV() {
    if(trigger_glSignalVkFenceNV) return;
    trigger_glSignalVkFenceNV = true;
    printf("Stub: glSignalVkFenceNV\n");
}
static bool trigger_glFragmentCoverageColorNV = false;
void stub_glFragmentCoverageColorNV() {
    if(trigger_glFragmentCoverageColorNV) return;
    trigger_glFragmentCoverageColorNV = true;
    printf("Stub: glFragmentCoverageColorNV\n");
}
static bool trigger_glCoverageModulationTableNV = false;
void stub_glCoverageModulationTableNV() {
    if(trigger_glCoverageModulationTableNV) return;
    trigger_glCoverageModulationTableNV = true;
    printf("Stub: glCoverageModulationTableNV\n");
}
static bool trigger_glGetCoverageModulationTableNV = false;
void stub_glGetCoverageModulationTableNV() {
    if(trigger_glGetCoverageModulationTableNV) return;
    trigger_glGetCoverageModulationTableNV = true;
    printf("Stub: glGetCoverageModulationTableNV\n");
}
static bool trigger_glCoverageModulationNV = false;
void stub_glCoverageModulationNV() {
    if(trigger_glCoverageModulationNV) return;
    trigger_glCoverageModulationNV = true;
    printf("Stub: glCoverageModulationNV\n");
}
static bool trigger_glRenderbufferStorageMultisampleCoverageNV = false;
void stub_glRenderbufferStorageMultisampleCoverageNV() {
    if(trigger_glRenderbufferStorageMultisampleCoverageNV) return;
    trigger_glRenderbufferStorageMultisampleCoverageNV = true;
    printf("Stub: glRenderbufferStorageMultisampleCoverageNV\n");
}
static bool trigger_glUniform1i64NV = false;
void stub_glUniform1i64NV() {
    if(trigger_glUniform1i64NV) return;
    trigger_glUniform1i64NV = true;
    printf("Stub: glUniform1i64NV\n");
}
static bool trigger_glUniform2i64NV = false;
void stub_glUniform2i64NV() {
    if(trigger_glUniform2i64NV) return;
    trigger_glUniform2i64NV = true;
    printf("Stub: glUniform2i64NV\n");
}
static bool trigger_glUniform3i64NV = false;
void stub_glUniform3i64NV() {
    if(trigger_glUniform3i64NV) return;
    trigger_glUniform3i64NV = true;
    printf("Stub: glUniform3i64NV\n");
}
static bool trigger_glUniform4i64NV = false;
void stub_glUniform4i64NV() {
    if(trigger_glUniform4i64NV) return;
    trigger_glUniform4i64NV = true;
    printf("Stub: glUniform4i64NV\n");
}
static bool trigger_glUniform1i64vNV = false;
void stub_glUniform1i64vNV() {
    if(trigger_glUniform1i64vNV) return;
    trigger_glUniform1i64vNV = true;
    printf("Stub: glUniform1i64vNV\n");
}
static bool trigger_glUniform2i64vNV = false;
void stub_glUniform2i64vNV() {
    if(trigger_glUniform2i64vNV) return;
    trigger_glUniform2i64vNV = true;
    printf("Stub: glUniform2i64vNV\n");
}
static bool trigger_glUniform3i64vNV = false;
void stub_glUniform3i64vNV() {
    if(trigger_glUniform3i64vNV) return;
    trigger_glUniform3i64vNV = true;
    printf("Stub: glUniform3i64vNV\n");
}
static bool trigger_glUniform4i64vNV = false;
void stub_glUniform4i64vNV() {
    if(trigger_glUniform4i64vNV) return;
    trigger_glUniform4i64vNV = true;
    printf("Stub: glUniform4i64vNV\n");
}
static bool trigger_glUniform1ui64NV = false;
void stub_glUniform1ui64NV() {
    if(trigger_glUniform1ui64NV) return;
    trigger_glUniform1ui64NV = true;
    printf("Stub: glUniform1ui64NV\n");
}
static bool trigger_glUniform2ui64NV = false;
void stub_glUniform2ui64NV() {
    if(trigger_glUniform2ui64NV) return;
    trigger_glUniform2ui64NV = true;
    printf("Stub: glUniform2ui64NV\n");
}
static bool trigger_glUniform3ui64NV = false;
void stub_glUniform3ui64NV() {
    if(trigger_glUniform3ui64NV) return;
    trigger_glUniform3ui64NV = true;
    printf("Stub: glUniform3ui64NV\n");
}
static bool trigger_glUniform4ui64NV = false;
void stub_glUniform4ui64NV() {
    if(trigger_glUniform4ui64NV) return;
    trigger_glUniform4ui64NV = true;
    printf("Stub: glUniform4ui64NV\n");
}
static bool trigger_glUniform1ui64vNV = false;
void stub_glUniform1ui64vNV() {
    if(trigger_glUniform1ui64vNV) return;
    trigger_glUniform1ui64vNV = true;
    printf("Stub: glUniform1ui64vNV\n");
}
static bool trigger_glUniform2ui64vNV = false;
void stub_glUniform2ui64vNV() {
    if(trigger_glUniform2ui64vNV) return;
    trigger_glUniform2ui64vNV = true;
    printf("Stub: glUniform2ui64vNV\n");
}
static bool trigger_glUniform3ui64vNV = false;
void stub_glUniform3ui64vNV() {
    if(trigger_glUniform3ui64vNV) return;
    trigger_glUniform3ui64vNV = true;
    printf("Stub: glUniform3ui64vNV\n");
}
static bool trigger_glUniform4ui64vNV = false;
void stub_glUniform4ui64vNV() {
    if(trigger_glUniform4ui64vNV) return;
    trigger_glUniform4ui64vNV = true;
    printf("Stub: glUniform4ui64vNV\n");
}
static bool trigger_glGetUniformi64vNV = false;
void stub_glGetUniformi64vNV() {
    if(trigger_glGetUniformi64vNV) return;
    trigger_glGetUniformi64vNV = true;
    printf("Stub: glGetUniformi64vNV\n");
}
static bool trigger_glProgramUniform1i64NV = false;
void stub_glProgramUniform1i64NV() {
    if(trigger_glProgramUniform1i64NV) return;
    trigger_glProgramUniform1i64NV = true;
    printf("Stub: glProgramUniform1i64NV\n");
}
static bool trigger_glProgramUniform2i64NV = false;
void stub_glProgramUniform2i64NV() {
    if(trigger_glProgramUniform2i64NV) return;
    trigger_glProgramUniform2i64NV = true;
    printf("Stub: glProgramUniform2i64NV\n");
}
static bool trigger_glProgramUniform3i64NV = false;
void stub_glProgramUniform3i64NV() {
    if(trigger_glProgramUniform3i64NV) return;
    trigger_glProgramUniform3i64NV = true;
    printf("Stub: glProgramUniform3i64NV\n");
}
static bool trigger_glProgramUniform4i64NV = false;
void stub_glProgramUniform4i64NV() {
    if(trigger_glProgramUniform4i64NV) return;
    trigger_glProgramUniform4i64NV = true;
    printf("Stub: glProgramUniform4i64NV\n");
}
static bool trigger_glProgramUniform1i64vNV = false;
void stub_glProgramUniform1i64vNV() {
    if(trigger_glProgramUniform1i64vNV) return;
    trigger_glProgramUniform1i64vNV = true;
    printf("Stub: glProgramUniform1i64vNV\n");
}
static bool trigger_glProgramUniform2i64vNV = false;
void stub_glProgramUniform2i64vNV() {
    if(trigger_glProgramUniform2i64vNV) return;
    trigger_glProgramUniform2i64vNV = true;
    printf("Stub: glProgramUniform2i64vNV\n");
}
static bool trigger_glProgramUniform3i64vNV = false;
void stub_glProgramUniform3i64vNV() {
    if(trigger_glProgramUniform3i64vNV) return;
    trigger_glProgramUniform3i64vNV = true;
    printf("Stub: glProgramUniform3i64vNV\n");
}
static bool trigger_glProgramUniform4i64vNV = false;
void stub_glProgramUniform4i64vNV() {
    if(trigger_glProgramUniform4i64vNV) return;
    trigger_glProgramUniform4i64vNV = true;
    printf("Stub: glProgramUniform4i64vNV\n");
}
static bool trigger_glProgramUniform1ui64NV = false;
void stub_glProgramUniform1ui64NV() {
    if(trigger_glProgramUniform1ui64NV) return;
    trigger_glProgramUniform1ui64NV = true;
    printf("Stub: glProgramUniform1ui64NV\n");
}
static bool trigger_glProgramUniform2ui64NV = false;
void stub_glProgramUniform2ui64NV() {
    if(trigger_glProgramUniform2ui64NV) return;
    trigger_glProgramUniform2ui64NV = true;
    printf("Stub: glProgramUniform2ui64NV\n");
}
static bool trigger_glProgramUniform3ui64NV = false;
void stub_glProgramUniform3ui64NV() {
    if(trigger_glProgramUniform3ui64NV) return;
    trigger_glProgramUniform3ui64NV = true;
    printf("Stub: glProgramUniform3ui64NV\n");
}
static bool trigger_glProgramUniform4ui64NV = false;
void stub_glProgramUniform4ui64NV() {
    if(trigger_glProgramUniform4ui64NV) return;
    trigger_glProgramUniform4ui64NV = true;
    printf("Stub: glProgramUniform4ui64NV\n");
}
static bool trigger_glProgramUniform1ui64vNV = false;
void stub_glProgramUniform1ui64vNV() {
    if(trigger_glProgramUniform1ui64vNV) return;
    trigger_glProgramUniform1ui64vNV = true;
    printf("Stub: glProgramUniform1ui64vNV\n");
}
static bool trigger_glProgramUniform2ui64vNV = false;
void stub_glProgramUniform2ui64vNV() {
    if(trigger_glProgramUniform2ui64vNV) return;
    trigger_glProgramUniform2ui64vNV = true;
    printf("Stub: glProgramUniform2ui64vNV\n");
}
static bool trigger_glProgramUniform3ui64vNV = false;
void stub_glProgramUniform3ui64vNV() {
    if(trigger_glProgramUniform3ui64vNV) return;
    trigger_glProgramUniform3ui64vNV = true;
    printf("Stub: glProgramUniform3ui64vNV\n");
}
static bool trigger_glProgramUniform4ui64vNV = false;
void stub_glProgramUniform4ui64vNV() {
    if(trigger_glProgramUniform4ui64vNV) return;
    trigger_glProgramUniform4ui64vNV = true;
    printf("Stub: glProgramUniform4ui64vNV\n");
}
static bool trigger_glGetInternalformatSampleivNV = false;
void stub_glGetInternalformatSampleivNV() {
    if(trigger_glGetInternalformatSampleivNV) return;
    trigger_glGetInternalformatSampleivNV = true;
    printf("Stub: glGetInternalformatSampleivNV\n");
}
static bool trigger_glGetMemoryObjectDetachedResourcesuivNV = false;
void stub_glGetMemoryObjectDetachedResourcesuivNV() {
    if(trigger_glGetMemoryObjectDetachedResourcesuivNV) return;
    trigger_glGetMemoryObjectDetachedResourcesuivNV = true;
    printf("Stub: glGetMemoryObjectDetachedResourcesuivNV\n");
}
static bool trigger_glResetMemoryObjectParameterNV = false;
void stub_glResetMemoryObjectParameterNV() {
    if(trigger_glResetMemoryObjectParameterNV) return;
    trigger_glResetMemoryObjectParameterNV = true;
    printf("Stub: glResetMemoryObjectParameterNV\n");
}
static bool trigger_glTexAttachMemoryNV = false;
void stub_glTexAttachMemoryNV() {
    if(trigger_glTexAttachMemoryNV) return;
    trigger_glTexAttachMemoryNV = true;
    printf("Stub: glTexAttachMemoryNV\n");
}
static bool trigger_glBufferAttachMemoryNV = false;
void stub_glBufferAttachMemoryNV() {
    if(trigger_glBufferAttachMemoryNV) return;
    trigger_glBufferAttachMemoryNV = true;
    printf("Stub: glBufferAttachMemoryNV\n");
}
static bool trigger_glTextureAttachMemoryNV = false;
void stub_glTextureAttachMemoryNV() {
    if(trigger_glTextureAttachMemoryNV) return;
    trigger_glTextureAttachMemoryNV = true;
    printf("Stub: glTextureAttachMemoryNV\n");
}
static bool trigger_glNamedBufferAttachMemoryNV = false;
void stub_glNamedBufferAttachMemoryNV() {
    if(trigger_glNamedBufferAttachMemoryNV) return;
    trigger_glNamedBufferAttachMemoryNV = true;
    printf("Stub: glNamedBufferAttachMemoryNV\n");
}
static bool trigger_glBufferPageCommitmentMemNV = false;
void stub_glBufferPageCommitmentMemNV() {
    if(trigger_glBufferPageCommitmentMemNV) return;
    trigger_glBufferPageCommitmentMemNV = true;
    printf("Stub: glBufferPageCommitmentMemNV\n");
}
static bool trigger_glTexPageCommitmentMemNV = false;
void stub_glTexPageCommitmentMemNV() {
    if(trigger_glTexPageCommitmentMemNV) return;
    trigger_glTexPageCommitmentMemNV = true;
    printf("Stub: glTexPageCommitmentMemNV\n");
}
static bool trigger_glNamedBufferPageCommitmentMemNV = false;
void stub_glNamedBufferPageCommitmentMemNV() {
    if(trigger_glNamedBufferPageCommitmentMemNV) return;
    trigger_glNamedBufferPageCommitmentMemNV = true;
    printf("Stub: glNamedBufferPageCommitmentMemNV\n");
}
static bool trigger_glTexturePageCommitmentMemNV = false;
void stub_glTexturePageCommitmentMemNV() {
    if(trigger_glTexturePageCommitmentMemNV) return;
    trigger_glTexturePageCommitmentMemNV = true;
    printf("Stub: glTexturePageCommitmentMemNV\n");
}
static bool trigger_glDrawMeshTasksNV = false;
void stub_glDrawMeshTasksNV() {
    if(trigger_glDrawMeshTasksNV) return;
    trigger_glDrawMeshTasksNV = true;
    printf("Stub: glDrawMeshTasksNV\n");
}
static bool trigger_glDrawMeshTasksIndirectNV = false;
void stub_glDrawMeshTasksIndirectNV() {
    if(trigger_glDrawMeshTasksIndirectNV) return;
    trigger_glDrawMeshTasksIndirectNV = true;
    printf("Stub: glDrawMeshTasksIndirectNV\n");
}
static bool trigger_glMultiDrawMeshTasksIndirectNV = false;
void stub_glMultiDrawMeshTasksIndirectNV() {
    if(trigger_glMultiDrawMeshTasksIndirectNV) return;
    trigger_glMultiDrawMeshTasksIndirectNV = true;
    printf("Stub: glMultiDrawMeshTasksIndirectNV\n");
}
static bool trigger_glMultiDrawMeshTasksIndirectCountNV = false;
void stub_glMultiDrawMeshTasksIndirectCountNV() {
    if(trigger_glMultiDrawMeshTasksIndirectCountNV) return;
    trigger_glMultiDrawMeshTasksIndirectCountNV = true;
    printf("Stub: glMultiDrawMeshTasksIndirectCountNV\n");
}
static bool trigger_glGenPathsNV = false;
void stub_glGenPathsNV() {
    if(trigger_glGenPathsNV) return;
    trigger_glGenPathsNV = true;
    printf("Stub: glGenPathsNV\n");
}
static bool trigger_glDeletePathsNV = false;
void stub_glDeletePathsNV() {
    if(trigger_glDeletePathsNV) return;
    trigger_glDeletePathsNV = true;
    printf("Stub: glDeletePathsNV\n");
}
static bool trigger_glIsPathNV = false;
void stub_glIsPathNV() {
    if(trigger_glIsPathNV) return;
    trigger_glIsPathNV = true;
    printf("Stub: glIsPathNV\n");
}
static bool trigger_glPathCommandsNV = false;
void stub_glPathCommandsNV() {
    if(trigger_glPathCommandsNV) return;
    trigger_glPathCommandsNV = true;
    printf("Stub: glPathCommandsNV\n");
}
static bool trigger_glPathCoordsNV = false;
void stub_glPathCoordsNV() {
    if(trigger_glPathCoordsNV) return;
    trigger_glPathCoordsNV = true;
    printf("Stub: glPathCoordsNV\n");
}
static bool trigger_glPathSubCommandsNV = false;
void stub_glPathSubCommandsNV() {
    if(trigger_glPathSubCommandsNV) return;
    trigger_glPathSubCommandsNV = true;
    printf("Stub: glPathSubCommandsNV\n");
}
static bool trigger_glPathSubCoordsNV = false;
void stub_glPathSubCoordsNV() {
    if(trigger_glPathSubCoordsNV) return;
    trigger_glPathSubCoordsNV = true;
    printf("Stub: glPathSubCoordsNV\n");
}
static bool trigger_glPathStringNV = false;
void stub_glPathStringNV() {
    if(trigger_glPathStringNV) return;
    trigger_glPathStringNV = true;
    printf("Stub: glPathStringNV\n");
}
static bool trigger_glPathGlyphsNV = false;
void stub_glPathGlyphsNV() {
    if(trigger_glPathGlyphsNV) return;
    trigger_glPathGlyphsNV = true;
    printf("Stub: glPathGlyphsNV\n");
}
static bool trigger_glPathGlyphRangeNV = false;
void stub_glPathGlyphRangeNV() {
    if(trigger_glPathGlyphRangeNV) return;
    trigger_glPathGlyphRangeNV = true;
    printf("Stub: glPathGlyphRangeNV\n");
}
static bool trigger_glWeightPathsNV = false;
void stub_glWeightPathsNV() {
    if(trigger_glWeightPathsNV) return;
    trigger_glWeightPathsNV = true;
    printf("Stub: glWeightPathsNV\n");
}
static bool trigger_glCopyPathNV = false;
void stub_glCopyPathNV() {
    if(trigger_glCopyPathNV) return;
    trigger_glCopyPathNV = true;
    printf("Stub: glCopyPathNV\n");
}
static bool trigger_glInterpolatePathsNV = false;
void stub_glInterpolatePathsNV() {
    if(trigger_glInterpolatePathsNV) return;
    trigger_glInterpolatePathsNV = true;
    printf("Stub: glInterpolatePathsNV\n");
}
static bool trigger_glTransformPathNV = false;
void stub_glTransformPathNV() {
    if(trigger_glTransformPathNV) return;
    trigger_glTransformPathNV = true;
    printf("Stub: glTransformPathNV\n");
}
static bool trigger_glPathParameterivNV = false;
void stub_glPathParameterivNV() {
    if(trigger_glPathParameterivNV) return;
    trigger_glPathParameterivNV = true;
    printf("Stub: glPathParameterivNV\n");
}
static bool trigger_glPathParameteriNV = false;
void stub_glPathParameteriNV() {
    if(trigger_glPathParameteriNV) return;
    trigger_glPathParameteriNV = true;
    printf("Stub: glPathParameteriNV\n");
}
static bool trigger_glPathParameterfvNV = false;
void stub_glPathParameterfvNV() {
    if(trigger_glPathParameterfvNV) return;
    trigger_glPathParameterfvNV = true;
    printf("Stub: glPathParameterfvNV\n");
}
static bool trigger_glPathParameterfNV = false;
void stub_glPathParameterfNV() {
    if(trigger_glPathParameterfNV) return;
    trigger_glPathParameterfNV = true;
    printf("Stub: glPathParameterfNV\n");
}
static bool trigger_glPathDashArrayNV = false;
void stub_glPathDashArrayNV() {
    if(trigger_glPathDashArrayNV) return;
    trigger_glPathDashArrayNV = true;
    printf("Stub: glPathDashArrayNV\n");
}
static bool trigger_glPathStencilFuncNV = false;
void stub_glPathStencilFuncNV() {
    if(trigger_glPathStencilFuncNV) return;
    trigger_glPathStencilFuncNV = true;
    printf("Stub: glPathStencilFuncNV\n");
}
static bool trigger_glPathStencilDepthOffsetNV = false;
void stub_glPathStencilDepthOffsetNV() {
    if(trigger_glPathStencilDepthOffsetNV) return;
    trigger_glPathStencilDepthOffsetNV = true;
    printf("Stub: glPathStencilDepthOffsetNV\n");
}
static bool trigger_glStencilFillPathNV = false;
void stub_glStencilFillPathNV() {
    if(trigger_glStencilFillPathNV) return;
    trigger_glStencilFillPathNV = true;
    printf("Stub: glStencilFillPathNV\n");
}
static bool trigger_glStencilStrokePathNV = false;
void stub_glStencilStrokePathNV() {
    if(trigger_glStencilStrokePathNV) return;
    trigger_glStencilStrokePathNV = true;
    printf("Stub: glStencilStrokePathNV\n");
}
static bool trigger_glStencilFillPathInstancedNV = false;
void stub_glStencilFillPathInstancedNV() {
    if(trigger_glStencilFillPathInstancedNV) return;
    trigger_glStencilFillPathInstancedNV = true;
    printf("Stub: glStencilFillPathInstancedNV\n");
}
static bool trigger_glStencilStrokePathInstancedNV = false;
void stub_glStencilStrokePathInstancedNV() {
    if(trigger_glStencilStrokePathInstancedNV) return;
    trigger_glStencilStrokePathInstancedNV = true;
    printf("Stub: glStencilStrokePathInstancedNV\n");
}
static bool trigger_glPathCoverDepthFuncNV = false;
void stub_glPathCoverDepthFuncNV() {
    if(trigger_glPathCoverDepthFuncNV) return;
    trigger_glPathCoverDepthFuncNV = true;
    printf("Stub: glPathCoverDepthFuncNV\n");
}
static bool trigger_glCoverFillPathNV = false;
void stub_glCoverFillPathNV() {
    if(trigger_glCoverFillPathNV) return;
    trigger_glCoverFillPathNV = true;
    printf("Stub: glCoverFillPathNV\n");
}
static bool trigger_glCoverStrokePathNV = false;
void stub_glCoverStrokePathNV() {
    if(trigger_glCoverStrokePathNV) return;
    trigger_glCoverStrokePathNV = true;
    printf("Stub: glCoverStrokePathNV\n");
}
static bool trigger_glCoverFillPathInstancedNV = false;
void stub_glCoverFillPathInstancedNV() {
    if(trigger_glCoverFillPathInstancedNV) return;
    trigger_glCoverFillPathInstancedNV = true;
    printf("Stub: glCoverFillPathInstancedNV\n");
}
static bool trigger_glCoverStrokePathInstancedNV = false;
void stub_glCoverStrokePathInstancedNV() {
    if(trigger_glCoverStrokePathInstancedNV) return;
    trigger_glCoverStrokePathInstancedNV = true;
    printf("Stub: glCoverStrokePathInstancedNV\n");
}
static bool trigger_glGetPathParameterivNV = false;
void stub_glGetPathParameterivNV() {
    if(trigger_glGetPathParameterivNV) return;
    trigger_glGetPathParameterivNV = true;
    printf("Stub: glGetPathParameterivNV\n");
}
static bool trigger_glGetPathParameterfvNV = false;
void stub_glGetPathParameterfvNV() {
    if(trigger_glGetPathParameterfvNV) return;
    trigger_glGetPathParameterfvNV = true;
    printf("Stub: glGetPathParameterfvNV\n");
}
static bool trigger_glGetPathCommandsNV = false;
void stub_glGetPathCommandsNV() {
    if(trigger_glGetPathCommandsNV) return;
    trigger_glGetPathCommandsNV = true;
    printf("Stub: glGetPathCommandsNV\n");
}
static bool trigger_glGetPathCoordsNV = false;
void stub_glGetPathCoordsNV() {
    if(trigger_glGetPathCoordsNV) return;
    trigger_glGetPathCoordsNV = true;
    printf("Stub: glGetPathCoordsNV\n");
}
static bool trigger_glGetPathDashArrayNV = false;
void stub_glGetPathDashArrayNV() {
    if(trigger_glGetPathDashArrayNV) return;
    trigger_glGetPathDashArrayNV = true;
    printf("Stub: glGetPathDashArrayNV\n");
}
static bool trigger_glGetPathMetricsNV = false;
void stub_glGetPathMetricsNV() {
    if(trigger_glGetPathMetricsNV) return;
    trigger_glGetPathMetricsNV = true;
    printf("Stub: glGetPathMetricsNV\n");
}
static bool trigger_glGetPathMetricRangeNV = false;
void stub_glGetPathMetricRangeNV() {
    if(trigger_glGetPathMetricRangeNV) return;
    trigger_glGetPathMetricRangeNV = true;
    printf("Stub: glGetPathMetricRangeNV\n");
}
static bool trigger_glGetPathSpacingNV = false;
void stub_glGetPathSpacingNV() {
    if(trigger_glGetPathSpacingNV) return;
    trigger_glGetPathSpacingNV = true;
    printf("Stub: glGetPathSpacingNV\n");
}
static bool trigger_glIsPointInFillPathNV = false;
void stub_glIsPointInFillPathNV() {
    if(trigger_glIsPointInFillPathNV) return;
    trigger_glIsPointInFillPathNV = true;
    printf("Stub: glIsPointInFillPathNV\n");
}
static bool trigger_glIsPointInStrokePathNV = false;
void stub_glIsPointInStrokePathNV() {
    if(trigger_glIsPointInStrokePathNV) return;
    trigger_glIsPointInStrokePathNV = true;
    printf("Stub: glIsPointInStrokePathNV\n");
}
static bool trigger_glGetPathLengthNV = false;
void stub_glGetPathLengthNV() {
    if(trigger_glGetPathLengthNV) return;
    trigger_glGetPathLengthNV = true;
    printf("Stub: glGetPathLengthNV\n");
}
static bool trigger_glPointAlongPathNV = false;
void stub_glPointAlongPathNV() {
    if(trigger_glPointAlongPathNV) return;
    trigger_glPointAlongPathNV = true;
    printf("Stub: glPointAlongPathNV\n");
}
static bool trigger_glMatrixLoad3x2fNV = false;
void stub_glMatrixLoad3x2fNV() {
    if(trigger_glMatrixLoad3x2fNV) return;
    trigger_glMatrixLoad3x2fNV = true;
    printf("Stub: glMatrixLoad3x2fNV\n");
}
static bool trigger_glMatrixLoad3x3fNV = false;
void stub_glMatrixLoad3x3fNV() {
    if(trigger_glMatrixLoad3x3fNV) return;
    trigger_glMatrixLoad3x3fNV = true;
    printf("Stub: glMatrixLoad3x3fNV\n");
}
static bool trigger_glMatrixLoadTranspose3x3fNV = false;
void stub_glMatrixLoadTranspose3x3fNV() {
    if(trigger_glMatrixLoadTranspose3x3fNV) return;
    trigger_glMatrixLoadTranspose3x3fNV = true;
    printf("Stub: glMatrixLoadTranspose3x3fNV\n");
}
static bool trigger_glMatrixMult3x2fNV = false;
void stub_glMatrixMult3x2fNV() {
    if(trigger_glMatrixMult3x2fNV) return;
    trigger_glMatrixMult3x2fNV = true;
    printf("Stub: glMatrixMult3x2fNV\n");
}
static bool trigger_glMatrixMult3x3fNV = false;
void stub_glMatrixMult3x3fNV() {
    if(trigger_glMatrixMult3x3fNV) return;
    trigger_glMatrixMult3x3fNV = true;
    printf("Stub: glMatrixMult3x3fNV\n");
}
static bool trigger_glMatrixMultTranspose3x3fNV = false;
void stub_glMatrixMultTranspose3x3fNV() {
    if(trigger_glMatrixMultTranspose3x3fNV) return;
    trigger_glMatrixMultTranspose3x3fNV = true;
    printf("Stub: glMatrixMultTranspose3x3fNV\n");
}
static bool trigger_glStencilThenCoverFillPathNV = false;
void stub_glStencilThenCoverFillPathNV() {
    if(trigger_glStencilThenCoverFillPathNV) return;
    trigger_glStencilThenCoverFillPathNV = true;
    printf("Stub: glStencilThenCoverFillPathNV\n");
}
static bool trigger_glStencilThenCoverStrokePathNV = false;
void stub_glStencilThenCoverStrokePathNV() {
    if(trigger_glStencilThenCoverStrokePathNV) return;
    trigger_glStencilThenCoverStrokePathNV = true;
    printf("Stub: glStencilThenCoverStrokePathNV\n");
}
static bool trigger_glStencilThenCoverFillPathInstancedNV = false;
void stub_glStencilThenCoverFillPathInstancedNV() {
    if(trigger_glStencilThenCoverFillPathInstancedNV) return;
    trigger_glStencilThenCoverFillPathInstancedNV = true;
    printf("Stub: glStencilThenCoverFillPathInstancedNV\n");
}
static bool trigger_glStencilThenCoverStrokePathInstancedNV = false;
void stub_glStencilThenCoverStrokePathInstancedNV() {
    if(trigger_glStencilThenCoverStrokePathInstancedNV) return;
    trigger_glStencilThenCoverStrokePathInstancedNV = true;
    printf("Stub: glStencilThenCoverStrokePathInstancedNV\n");
}
static bool trigger_glPathGlyphIndexRangeNV = false;
void stub_glPathGlyphIndexRangeNV() {
    if(trigger_glPathGlyphIndexRangeNV) return;
    trigger_glPathGlyphIndexRangeNV = true;
    printf("Stub: glPathGlyphIndexRangeNV\n");
}
static bool trigger_glPathGlyphIndexArrayNV = false;
void stub_glPathGlyphIndexArrayNV() {
    if(trigger_glPathGlyphIndexArrayNV) return;
    trigger_glPathGlyphIndexArrayNV = true;
    printf("Stub: glPathGlyphIndexArrayNV\n");
}
static bool trigger_glPathMemoryGlyphIndexArrayNV = false;
void stub_glPathMemoryGlyphIndexArrayNV() {
    if(trigger_glPathMemoryGlyphIndexArrayNV) return;
    trigger_glPathMemoryGlyphIndexArrayNV = true;
    printf("Stub: glPathMemoryGlyphIndexArrayNV\n");
}
static bool trigger_glProgramPathFragmentInputGenNV = false;
void stub_glProgramPathFragmentInputGenNV() {
    if(trigger_glProgramPathFragmentInputGenNV) return;
    trigger_glProgramPathFragmentInputGenNV = true;
    printf("Stub: glProgramPathFragmentInputGenNV\n");
}
static bool trigger_glGetProgramResourcefvNV = false;
void stub_glGetProgramResourcefvNV() {
    if(trigger_glGetProgramResourcefvNV) return;
    trigger_glGetProgramResourcefvNV = true;
    printf("Stub: glGetProgramResourcefvNV\n");
}
static bool trigger_glFramebufferSampleLocationsfvNV = false;
void stub_glFramebufferSampleLocationsfvNV() {
    if(trigger_glFramebufferSampleLocationsfvNV) return;
    trigger_glFramebufferSampleLocationsfvNV = true;
    printf("Stub: glFramebufferSampleLocationsfvNV\n");
}
static bool trigger_glNamedFramebufferSampleLocationsfvNV = false;
void stub_glNamedFramebufferSampleLocationsfvNV() {
    if(trigger_glNamedFramebufferSampleLocationsfvNV) return;
    trigger_glNamedFramebufferSampleLocationsfvNV = true;
    printf("Stub: glNamedFramebufferSampleLocationsfvNV\n");
}
static bool trigger_glResolveDepthValuesNV = false;
void stub_glResolveDepthValuesNV() {
    if(trigger_glResolveDepthValuesNV) return;
    trigger_glResolveDepthValuesNV = true;
    printf("Stub: glResolveDepthValuesNV\n");
}
static bool trigger_glScissorExclusiveNV = false;
void stub_glScissorExclusiveNV() {
    if(trigger_glScissorExclusiveNV) return;
    trigger_glScissorExclusiveNV = true;
    printf("Stub: glScissorExclusiveNV\n");
}
static bool trigger_glScissorExclusiveArrayvNV = false;
void stub_glScissorExclusiveArrayvNV() {
    if(trigger_glScissorExclusiveArrayvNV) return;
    trigger_glScissorExclusiveArrayvNV = true;
    printf("Stub: glScissorExclusiveArrayvNV\n");
}
static bool trigger_glMakeBufferResidentNV = false;
void stub_glMakeBufferResidentNV() {
    if(trigger_glMakeBufferResidentNV) return;
    trigger_glMakeBufferResidentNV = true;
    printf("Stub: glMakeBufferResidentNV\n");
}
static bool trigger_glMakeBufferNonResidentNV = false;
void stub_glMakeBufferNonResidentNV() {
    if(trigger_glMakeBufferNonResidentNV) return;
    trigger_glMakeBufferNonResidentNV = true;
    printf("Stub: glMakeBufferNonResidentNV\n");
}
static bool trigger_glIsBufferResidentNV = false;
void stub_glIsBufferResidentNV() {
    if(trigger_glIsBufferResidentNV) return;
    trigger_glIsBufferResidentNV = true;
    printf("Stub: glIsBufferResidentNV\n");
}
static bool trigger_glMakeNamedBufferResidentNV = false;
void stub_glMakeNamedBufferResidentNV() {
    if(trigger_glMakeNamedBufferResidentNV) return;
    trigger_glMakeNamedBufferResidentNV = true;
    printf("Stub: glMakeNamedBufferResidentNV\n");
}
static bool trigger_glMakeNamedBufferNonResidentNV = false;
void stub_glMakeNamedBufferNonResidentNV() {
    if(trigger_glMakeNamedBufferNonResidentNV) return;
    trigger_glMakeNamedBufferNonResidentNV = true;
    printf("Stub: glMakeNamedBufferNonResidentNV\n");
}
static bool trigger_glIsNamedBufferResidentNV = false;
void stub_glIsNamedBufferResidentNV() {
    if(trigger_glIsNamedBufferResidentNV) return;
    trigger_glIsNamedBufferResidentNV = true;
    printf("Stub: glIsNamedBufferResidentNV\n");
}
static bool trigger_glGetBufferParameterui64vNV = false;
void stub_glGetBufferParameterui64vNV() {
    if(trigger_glGetBufferParameterui64vNV) return;
    trigger_glGetBufferParameterui64vNV = true;
    printf("Stub: glGetBufferParameterui64vNV\n");
}
static bool trigger_glGetNamedBufferParameterui64vNV = false;
void stub_glGetNamedBufferParameterui64vNV() {
    if(trigger_glGetNamedBufferParameterui64vNV) return;
    trigger_glGetNamedBufferParameterui64vNV = true;
    printf("Stub: glGetNamedBufferParameterui64vNV\n");
}
static bool trigger_glGetIntegerui64vNV = false;
void stub_glGetIntegerui64vNV() {
    if(trigger_glGetIntegerui64vNV) return;
    trigger_glGetIntegerui64vNV = true;
    printf("Stub: glGetIntegerui64vNV\n");
}
static bool trigger_glUniformui64NV = false;
void stub_glUniformui64NV() {
    if(trigger_glUniformui64NV) return;
    trigger_glUniformui64NV = true;
    printf("Stub: glUniformui64NV\n");
}
static bool trigger_glUniformui64vNV = false;
void stub_glUniformui64vNV() {
    if(trigger_glUniformui64vNV) return;
    trigger_glUniformui64vNV = true;
    printf("Stub: glUniformui64vNV\n");
}
static bool trigger_glGetUniformui64vNV = false;
void stub_glGetUniformui64vNV() {
    if(trigger_glGetUniformui64vNV) return;
    trigger_glGetUniformui64vNV = true;
    printf("Stub: glGetUniformui64vNV\n");
}
static bool trigger_glProgramUniformui64NV = false;
void stub_glProgramUniformui64NV() {
    if(trigger_glProgramUniformui64NV) return;
    trigger_glProgramUniformui64NV = true;
    printf("Stub: glProgramUniformui64NV\n");
}
static bool trigger_glProgramUniformui64vNV = false;
void stub_glProgramUniformui64vNV() {
    if(trigger_glProgramUniformui64vNV) return;
    trigger_glProgramUniformui64vNV = true;
    printf("Stub: glProgramUniformui64vNV\n");
}
static bool trigger_glBindShadingRateImageNV = false;
void stub_glBindShadingRateImageNV() {
    if(trigger_glBindShadingRateImageNV) return;
    trigger_glBindShadingRateImageNV = true;
    printf("Stub: glBindShadingRateImageNV\n");
}
static bool trigger_glGetShadingRateImagePaletteNV = false;
void stub_glGetShadingRateImagePaletteNV() {
    if(trigger_glGetShadingRateImagePaletteNV) return;
    trigger_glGetShadingRateImagePaletteNV = true;
    printf("Stub: glGetShadingRateImagePaletteNV\n");
}
static bool trigger_glGetShadingRateSampleLocationivNV = false;
void stub_glGetShadingRateSampleLocationivNV() {
    if(trigger_glGetShadingRateSampleLocationivNV) return;
    trigger_glGetShadingRateSampleLocationivNV = true;
    printf("Stub: glGetShadingRateSampleLocationivNV\n");
}
static bool trigger_glShadingRateImageBarrierNV = false;
void stub_glShadingRateImageBarrierNV() {
    if(trigger_glShadingRateImageBarrierNV) return;
    trigger_glShadingRateImageBarrierNV = true;
    printf("Stub: glShadingRateImageBarrierNV\n");
}
static bool trigger_glShadingRateImagePaletteNV = false;
void stub_glShadingRateImagePaletteNV() {
    if(trigger_glShadingRateImagePaletteNV) return;
    trigger_glShadingRateImagePaletteNV = true;
    printf("Stub: glShadingRateImagePaletteNV\n");
}
static bool trigger_glShadingRateSampleOrderNV = false;
void stub_glShadingRateSampleOrderNV() {
    if(trigger_glShadingRateSampleOrderNV) return;
    trigger_glShadingRateSampleOrderNV = true;
    printf("Stub: glShadingRateSampleOrderNV\n");
}
static bool trigger_glShadingRateSampleOrderCustomNV = false;
void stub_glShadingRateSampleOrderCustomNV() {
    if(trigger_glShadingRateSampleOrderCustomNV) return;
    trigger_glShadingRateSampleOrderCustomNV = true;
    printf("Stub: glShadingRateSampleOrderCustomNV\n");
}
static bool trigger_glTextureBarrierNV = false;
void stub_glTextureBarrierNV() {
    if(trigger_glTextureBarrierNV) return;
    trigger_glTextureBarrierNV = true;
    printf("Stub: glTextureBarrierNV\n");
}
static bool trigger_glVertexAttribL1i64NV = false;
void stub_glVertexAttribL1i64NV() {
    if(trigger_glVertexAttribL1i64NV) return;
    trigger_glVertexAttribL1i64NV = true;
    printf("Stub: glVertexAttribL1i64NV\n");
}
static bool trigger_glVertexAttribL2i64NV = false;
void stub_glVertexAttribL2i64NV() {
    if(trigger_glVertexAttribL2i64NV) return;
    trigger_glVertexAttribL2i64NV = true;
    printf("Stub: glVertexAttribL2i64NV\n");
}
static bool trigger_glVertexAttribL3i64NV = false;
void stub_glVertexAttribL3i64NV() {
    if(trigger_glVertexAttribL3i64NV) return;
    trigger_glVertexAttribL3i64NV = true;
    printf("Stub: glVertexAttribL3i64NV\n");
}
static bool trigger_glVertexAttribL4i64NV = false;
void stub_glVertexAttribL4i64NV() {
    if(trigger_glVertexAttribL4i64NV) return;
    trigger_glVertexAttribL4i64NV = true;
    printf("Stub: glVertexAttribL4i64NV\n");
}
static bool trigger_glVertexAttribL1i64vNV = false;
void stub_glVertexAttribL1i64vNV() {
    if(trigger_glVertexAttribL1i64vNV) return;
    trigger_glVertexAttribL1i64vNV = true;
    printf("Stub: glVertexAttribL1i64vNV\n");
}
static bool trigger_glVertexAttribL2i64vNV = false;
void stub_glVertexAttribL2i64vNV() {
    if(trigger_glVertexAttribL2i64vNV) return;
    trigger_glVertexAttribL2i64vNV = true;
    printf("Stub: glVertexAttribL2i64vNV\n");
}
static bool trigger_glVertexAttribL3i64vNV = false;
void stub_glVertexAttribL3i64vNV() {
    if(trigger_glVertexAttribL3i64vNV) return;
    trigger_glVertexAttribL3i64vNV = true;
    printf("Stub: glVertexAttribL3i64vNV\n");
}
static bool trigger_glVertexAttribL4i64vNV = false;
void stub_glVertexAttribL4i64vNV() {
    if(trigger_glVertexAttribL4i64vNV) return;
    trigger_glVertexAttribL4i64vNV = true;
    printf("Stub: glVertexAttribL4i64vNV\n");
}
static bool trigger_glVertexAttribL1ui64NV = false;
void stub_glVertexAttribL1ui64NV() {
    if(trigger_glVertexAttribL1ui64NV) return;
    trigger_glVertexAttribL1ui64NV = true;
    printf("Stub: glVertexAttribL1ui64NV\n");
}
static bool trigger_glVertexAttribL2ui64NV = false;
void stub_glVertexAttribL2ui64NV() {
    if(trigger_glVertexAttribL2ui64NV) return;
    trigger_glVertexAttribL2ui64NV = true;
    printf("Stub: glVertexAttribL2ui64NV\n");
}
static bool trigger_glVertexAttribL3ui64NV = false;
void stub_glVertexAttribL3ui64NV() {
    if(trigger_glVertexAttribL3ui64NV) return;
    trigger_glVertexAttribL3ui64NV = true;
    printf("Stub: glVertexAttribL3ui64NV\n");
}
static bool trigger_glVertexAttribL4ui64NV = false;
void stub_glVertexAttribL4ui64NV() {
    if(trigger_glVertexAttribL4ui64NV) return;
    trigger_glVertexAttribL4ui64NV = true;
    printf("Stub: glVertexAttribL4ui64NV\n");
}
static bool trigger_glVertexAttribL1ui64vNV = false;
void stub_glVertexAttribL1ui64vNV() {
    if(trigger_glVertexAttribL1ui64vNV) return;
    trigger_glVertexAttribL1ui64vNV = true;
    printf("Stub: glVertexAttribL1ui64vNV\n");
}
static bool trigger_glVertexAttribL2ui64vNV = false;
void stub_glVertexAttribL2ui64vNV() {
    if(trigger_glVertexAttribL2ui64vNV) return;
    trigger_glVertexAttribL2ui64vNV = true;
    printf("Stub: glVertexAttribL2ui64vNV\n");
}
static bool trigger_glVertexAttribL3ui64vNV = false;
void stub_glVertexAttribL3ui64vNV() {
    if(trigger_glVertexAttribL3ui64vNV) return;
    trigger_glVertexAttribL3ui64vNV = true;
    printf("Stub: glVertexAttribL3ui64vNV\n");
}
static bool trigger_glVertexAttribL4ui64vNV = false;
void stub_glVertexAttribL4ui64vNV() {
    if(trigger_glVertexAttribL4ui64vNV) return;
    trigger_glVertexAttribL4ui64vNV = true;
    printf("Stub: glVertexAttribL4ui64vNV\n");
}
static bool trigger_glGetVertexAttribLi64vNV = false;
void stub_glGetVertexAttribLi64vNV() {
    if(trigger_glGetVertexAttribLi64vNV) return;
    trigger_glGetVertexAttribLi64vNV = true;
    printf("Stub: glGetVertexAttribLi64vNV\n");
}
static bool trigger_glGetVertexAttribLui64vNV = false;
void stub_glGetVertexAttribLui64vNV() {
    if(trigger_glGetVertexAttribLui64vNV) return;
    trigger_glGetVertexAttribLui64vNV = true;
    printf("Stub: glGetVertexAttribLui64vNV\n");
}
static bool trigger_glVertexAttribLFormatNV = false;
void stub_glVertexAttribLFormatNV() {
    if(trigger_glVertexAttribLFormatNV) return;
    trigger_glVertexAttribLFormatNV = true;
    printf("Stub: glVertexAttribLFormatNV\n");
}
static bool trigger_glBufferAddressRangeNV = false;
void stub_glBufferAddressRangeNV() {
    if(trigger_glBufferAddressRangeNV) return;
    trigger_glBufferAddressRangeNV = true;
    printf("Stub: glBufferAddressRangeNV\n");
}
static bool trigger_glVertexFormatNV = false;
void stub_glVertexFormatNV() {
    if(trigger_glVertexFormatNV) return;
    trigger_glVertexFormatNV = true;
    printf("Stub: glVertexFormatNV\n");
}
static bool trigger_glNormalFormatNV = false;
void stub_glNormalFormatNV() {
    if(trigger_glNormalFormatNV) return;
    trigger_glNormalFormatNV = true;
    printf("Stub: glNormalFormatNV\n");
}
static bool trigger_glColorFormatNV = false;
void stub_glColorFormatNV() {
    if(trigger_glColorFormatNV) return;
    trigger_glColorFormatNV = true;
    printf("Stub: glColorFormatNV\n");
}
static bool trigger_glIndexFormatNV = false;
void stub_glIndexFormatNV() {
    if(trigger_glIndexFormatNV) return;
    trigger_glIndexFormatNV = true;
    printf("Stub: glIndexFormatNV\n");
}
static bool trigger_glTexCoordFormatNV = false;
void stub_glTexCoordFormatNV() {
    if(trigger_glTexCoordFormatNV) return;
    trigger_glTexCoordFormatNV = true;
    printf("Stub: glTexCoordFormatNV\n");
}
static bool trigger_glEdgeFlagFormatNV = false;
void stub_glEdgeFlagFormatNV() {
    if(trigger_glEdgeFlagFormatNV) return;
    trigger_glEdgeFlagFormatNV = true;
    printf("Stub: glEdgeFlagFormatNV\n");
}
static bool trigger_glSecondaryColorFormatNV = false;
void stub_glSecondaryColorFormatNV() {
    if(trigger_glSecondaryColorFormatNV) return;
    trigger_glSecondaryColorFormatNV = true;
    printf("Stub: glSecondaryColorFormatNV\n");
}
static bool trigger_glFogCoordFormatNV = false;
void stub_glFogCoordFormatNV() {
    if(trigger_glFogCoordFormatNV) return;
    trigger_glFogCoordFormatNV = true;
    printf("Stub: glFogCoordFormatNV\n");
}
static bool trigger_glVertexAttribFormatNV = false;
void stub_glVertexAttribFormatNV() {
    if(trigger_glVertexAttribFormatNV) return;
    trigger_glVertexAttribFormatNV = true;
    printf("Stub: glVertexAttribFormatNV\n");
}
static bool trigger_glVertexAttribIFormatNV = false;
void stub_glVertexAttribIFormatNV() {
    if(trigger_glVertexAttribIFormatNV) return;
    trigger_glVertexAttribIFormatNV = true;
    printf("Stub: glVertexAttribIFormatNV\n");
}
static bool trigger_glGetIntegerui64i_vNV = false;
void stub_glGetIntegerui64i_vNV() {
    if(trigger_glGetIntegerui64i_vNV) return;
    trigger_glGetIntegerui64i_vNV = true;
    printf("Stub: glGetIntegerui64i_vNV\n");
}
static bool trigger_glViewportSwizzleNV = false;
void stub_glViewportSwizzleNV() {
    if(trigger_glViewportSwizzleNV) return;
    trigger_glViewportSwizzleNV = true;
    printf("Stub: glViewportSwizzleNV\n");
}
static bool trigger_glFramebufferTextureMultiviewOVR = false;
void stub_glFramebufferTextureMultiviewOVR() {
    if(trigger_glFramebufferTextureMultiviewOVR) return;
    trigger_glFramebufferTextureMultiviewOVR = true;
    printf("Stub: glFramebufferTextureMultiviewOVR\n");
}
static bool trigger_glNamedFramebufferTextureMultiviewOVR = false;
void stub_glNamedFramebufferTextureMultiviewOVR() {
    if(trigger_glNamedFramebufferTextureMultiviewOVR) return;
    trigger_glNamedFramebufferTextureMultiviewOVR = true;
    printf("Stub: glNamedFramebufferTextureMultiviewOVR\n");
}

//BUILD SUCCESSFUL (total time: 0 seconds)
