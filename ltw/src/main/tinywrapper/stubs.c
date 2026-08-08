/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

/**
 * 文件功能：未实现桌面 GL 函数的空实现（stub_*）。
 *
 * 游戏探针请求到 GLES 不存在的函数时，返回空实现而不是
 * functionMissingAbort 直接中止；同时提供少量固定管线杂项入口
 * （glGetDoublev、glMaterial* 等）的桌面语义兜底。
 */
#include <stdbool.h>
#include <stdio.h>
void stub_glCullFace() {
}
void stub_glFrontFace() {
}
void stub_glHint() {
}
void stub_glLineWidth() {
}
void stub_glPointSize() {
}
void stub_glPolygonMode() {
}
void stub_glScissor() {
}
void stub_glTexParameterf() {
}
void stub_glTexParameterfv() {
}
void stub_glTexParameteri() {
}
void stub_glTexParameteriv() {
}
void stub_glTexImage1D() {
}
void stub_glTexImage2D() {
}
void stub_glDrawBuffer() {
}
void stub_glClear() {
}
void stub_glClearColor() {
}
void stub_glClearStencil() {
}
void stub_glClearDepth() {
}
void stub_glStencilMask() {
}
void stub_glColorMask() {
}
void stub_glDepthMask() {
}
void stub_glDisable() {
}
void stub_glEnable() {
}
void stub_glFinish() {
}
void stub_glFlush() {
}
void stub_glBlendFunc() {
}
void stub_glLogicOp() {
}
void stub_glStencilFunc() {
}
void stub_glStencilOp() {
}
void stub_glDepthFunc() {
}
void stub_glPixelStoref() {
}
void stub_glPixelStorei() {
}
void stub_glReadBuffer() {
}
void stub_glReadPixels() {
}
__attribute__((used)) void stub_glGetBooleanv() {
}
__attribute__((used)) void stub_glGetDoublev() {
}
void stub_glGetError() {
}
__attribute__((used)) void stub_glGetFloatv() {
}
void stub_glGetIntegerv() {
}
void stub_glGetString() {
}
void stub_glGetTexImage() {
}
void stub_glGetTexParameterfv() {
}
void stub_glGetTexParameteriv() {
}
void stub_glGetTexLevelParameterfv() {
}
void stub_glGetTexLevelParameteriv() {
}
void stub_glIsEnabled() {
}
void stub_glDepthRange() {
}
void stub_glViewport() {
}
void stub_glDrawArrays() {
}
void stub_glDrawElements() {
}
void stub_glGetPointerv() {
}
void stub_glPolygonOffset() {
}
void stub_glCopyTexImage1D() {
}
void stub_glCopyTexImage2D() {
}
void stub_glCopyTexSubImage1D() {
}
void stub_glCopyTexSubImage2D() {
}
void stub_glTexSubImage1D() {
}
void stub_glTexSubImage2D() {
}
void stub_glBindTexture() {
}
void stub_glDeleteTextures() {
}
void stub_glGenTextures() {
}
void stub_glIsTexture() {
}
void stub_glDrawRangeElements() {
}
void stub_glTexImage3D() {
}
void stub_glTexSubImage3D() {
}
void stub_glCopyTexSubImage3D() {
}
void stub_glActiveTexture() {
}
void stub_glSampleCoverage() {
}
void stub_glCompressedTexImage3D() {
}
void stub_glCompressedTexImage2D() {
}
void stub_glCompressedTexImage1D() {
}
void stub_glCompressedTexSubImage3D() {
}
void stub_glCompressedTexSubImage2D() {
}
void stub_glCompressedTexSubImage1D() {
}
void stub_glGetCompressedTexImage() {
}
void stub_glBlendFuncSeparate() {
}
void stub_glMultiDrawArrays() {
}
void stub_glMultiDrawElements() {
}
void stub_glPointParameterf() {
}
void stub_glPointParameterfv() {
}
void stub_glPointParameteri() {
}
void stub_glPointParameteriv() {
}
void stub_glBlendColor() {
}
void stub_glBlendEquation() {
}
void stub_glGenQueries() {
}
void stub_glDeleteQueries() {
}
void stub_glIsQuery() {
}
void stub_glBeginQuery() {
}
void stub_glEndQuery() {
}
void stub_glGetQueryiv() {
}
void stub_glGetQueryObjectiv() {
}
void stub_glGetQueryObjectuiv() {
}
void stub_glBindBuffer() {
}
void stub_glDeleteBuffers() {
}
void stub_glGenBuffers() {
}
void stub_glIsBuffer() {
}
void stub_glBufferData() {
}
void stub_glBufferSubData() {
}
void stub_glGetBufferSubData() {
}
void stub_glMapBuffer() {
}
void stub_glUnmapBuffer() {
}
void stub_glGetBufferParameteriv() {
}
void stub_glGetBufferPointerv() {
}
void stub_glBlendEquationSeparate() {
}
void stub_glDrawBuffers() {
}
void stub_glStencilOpSeparate() {
}
void stub_glStencilFuncSeparate() {
}
void stub_glStencilMaskSeparate() {
}
void stub_glAttachShader() {
}
void stub_glBindAttribLocation() {
}
void stub_glCompileShader() {
}
void stub_glCreateProgram() {
}
void stub_glCreateShader() {
}
void stub_glDeleteProgram() {
}
void stub_glDeleteShader() {
}
void stub_glDetachShader() {
}
void stub_glDisableVertexAttribArray() {
}
void stub_glEnableVertexAttribArray() {
}
void stub_glGetActiveAttrib() {
}
void stub_glGetActiveUniform() {
}
void stub_glGetAttachedShaders() {
}
void stub_glGetAttribLocation() {
}
void stub_glGetProgramiv() {
}
void stub_glGetProgramInfoLog() {
}
void stub_glGetShaderiv() {
}
void stub_glGetShaderInfoLog() {
}
void stub_glGetShaderSource() {
}
void stub_glGetUniformLocation() {
}
void stub_glGetUniformfv() {
}
void stub_glGetUniformiv() {
}
void stub_glGetVertexAttribdv() {
}
void stub_glGetVertexAttribfv() {
}
void stub_glGetVertexAttribiv() {
}
void stub_glGetVertexAttribPointerv() {
}
void stub_glIsProgram() {
}
void stub_glIsShader() {
}
void stub_glLinkProgram() {
}
void stub_glShaderSource() {
}
void stub_glUseProgram() {
}
void stub_glUniform1f() {
}
void stub_glUniform2f() {
}
void stub_glUniform3f() {
}
void stub_glUniform4f() {
}
void stub_glUniform1i() {
}
void stub_glUniform2i() {
}
void stub_glUniform3i() {
}
void stub_glUniform4i() {
}
void stub_glUniform1fv() {
}
void stub_glUniform2fv() {
}
void stub_glUniform3fv() {
}
void stub_glUniform4fv() {
}
void stub_glUniform1iv() {
}
void stub_glUniform2iv() {
}
void stub_glUniform3iv() {
}
void stub_glUniform4iv() {
}
void stub_glUniformMatrix2fv() {
}
void stub_glUniformMatrix3fv() {
}
void stub_glUniformMatrix4fv() {
}
void stub_glValidateProgram() {
}
void stub_glVertexAttrib1d() {
}
void stub_glVertexAttrib1dv() {
}
void stub_glVertexAttrib1f() {
}
void stub_glVertexAttrib1fv() {
}
void stub_glVertexAttrib1s() {
}
void stub_glVertexAttrib1sv() {
}
void stub_glVertexAttrib2d() {
}
void stub_glVertexAttrib2dv() {
}
void stub_glVertexAttrib2f() {
}
void stub_glVertexAttrib2fv() {
}
void stub_glVertexAttrib2s() {
}
void stub_glVertexAttrib2sv() {
}
void stub_glVertexAttrib3d() {
}
void stub_glVertexAttrib3dv() {
}
void stub_glVertexAttrib3f() {
}
void stub_glVertexAttrib3fv() {
}
void stub_glVertexAttrib3s() {
}
void stub_glVertexAttrib3sv() {
}
void stub_glVertexAttrib4Nbv() {
}
void stub_glVertexAttrib4Niv() {
}
void stub_glVertexAttrib4Nsv() {
}
void stub_glVertexAttrib4Nub() {
}
void stub_glVertexAttrib4Nubv() {
}
void stub_glVertexAttrib4Nuiv() {
}
void stub_glVertexAttrib4Nusv() {
}
void stub_glVertexAttrib4bv() {
}
void stub_glVertexAttrib4d() {
}
void stub_glVertexAttrib4dv() {
}
void stub_glVertexAttrib4f() {
}
void stub_glVertexAttrib4fv() {
}
void stub_glVertexAttrib4iv() {
}
void stub_glVertexAttrib4s() {
}
void stub_glVertexAttrib4sv() {
}
void stub_glVertexAttrib4ubv() {
}
void stub_glVertexAttrib4uiv() {
}
void stub_glVertexAttrib4usv() {
}
void stub_glVertexAttribPointer() {
}
void stub_glUniformMatrix2x3fv() {
}
void stub_glUniformMatrix3x2fv() {
}
void stub_glUniformMatrix2x4fv() {
}
void stub_glUniformMatrix4x2fv() {
}
void stub_glUniformMatrix3x4fv() {
}
void stub_glUniformMatrix4x3fv() {
}
void stub_glColorMaski() {
}
void stub_glGetBooleani_v() {
}
void stub_glGetIntegeri_v() {
}
void stub_glEnablei() {
}
void stub_glDisablei() {
}
void stub_glIsEnabledi() {
}
void stub_glBeginTransformFeedback() {
}
void stub_glEndTransformFeedback() {
}
void stub_glBindBufferRange() {
}
void stub_glBindBufferBase() {
}
void stub_glTransformFeedbackVaryings() {
}
void stub_glGetTransformFeedbackVarying() {
}
void stub_glClampColor() {
}
void stub_glBeginConditionalRender() {
}
void stub_glEndConditionalRender() {
}
void stub_glVertexAttribIPointer() {
}
void stub_glGetVertexAttribIiv() {
}
void stub_glGetVertexAttribIuiv() {
}
void stub_glVertexAttribI1i() {
}
void stub_glVertexAttribI2i() {
}
void stub_glVertexAttribI3i() {
}
void stub_glVertexAttribI4i() {
}
void stub_glVertexAttribI1ui() {
}
void stub_glVertexAttribI2ui() {
}
void stub_glVertexAttribI3ui() {
}
void stub_glVertexAttribI4ui() {
}
void stub_glVertexAttribI1iv() {
}
void stub_glVertexAttribI2iv() {
}
void stub_glVertexAttribI3iv() {
}
void stub_glVertexAttribI4iv() {
}
void stub_glVertexAttribI1uiv() {
}
void stub_glVertexAttribI2uiv() {
}
void stub_glVertexAttribI3uiv() {
}
void stub_glVertexAttribI4uiv() {
}
void stub_glVertexAttribI4bv() {
}
void stub_glVertexAttribI4sv() {
}
void stub_glVertexAttribI4ubv() {
}
void stub_glVertexAttribI4usv() {
}
void stub_glGetUniformuiv() {
}
void stub_glBindFragDataLocation() {
}
void stub_glGetFragDataLocation() {
}
void stub_glUniform1ui() {
}
void stub_glUniform2ui() {
}
void stub_glUniform3ui() {
}
void stub_glUniform4ui() {
}
void stub_glUniform1uiv() {
}
void stub_glUniform2uiv() {
}
void stub_glUniform3uiv() {
}
void stub_glUniform4uiv() {
}
void stub_glTexParameterIiv() {
}
void stub_glTexParameterIuiv() {
}
void stub_glGetTexParameterIiv() {
}
void stub_glGetTexParameterIuiv() {
}
void stub_glClearBufferiv() {
}
void stub_glClearBufferuiv() {
}
void stub_glClearBufferfv() {
}
void stub_glClearBufferfi() {
}
void stub_glGetStringi() {
}
void stub_glIsRenderbuffer() {
}
void stub_glBindRenderbuffer() {
}
void stub_glDeleteRenderbuffers() {
}
void stub_glGenRenderbuffers() {
}
void stub_glRenderbufferStorage() {
}
void stub_glGetRenderbufferParameteriv() {
}
void stub_glIsFramebuffer() {
}
void stub_glBindFramebuffer() {
}
void stub_glDeleteFramebuffers() {
}
void stub_glGenFramebuffers() {
}
void stub_glCheckFramebufferStatus() {
}
void stub_glFramebufferTexture1D() {
}
void stub_glFramebufferTexture2D() {
}
void stub_glFramebufferTexture3D() {
}
void stub_glFramebufferRenderbuffer() {
}
void stub_glGetFramebufferAttachmentParameteriv() {
}
void stub_glGenerateMipmap() {
}
void stub_glBlitFramebuffer() {
}
void stub_glRenderbufferStorageMultisample() {
}
void stub_glFramebufferTextureLayer() {
}
void stub_glMapBufferRange() {
}
void stub_glFlushMappedBufferRange() {
}
void stub_glBindVertexArray() {
}
void stub_glDeleteVertexArrays() {
}
void stub_glGenVertexArrays() {
}
void stub_glIsVertexArray() {
}
void stub_glDrawArraysInstanced() {
}
void stub_glDrawElementsInstanced() {
}
void stub_glTexBuffer() {
}
void stub_glPrimitiveRestartIndex() {
}
void stub_glCopyBufferSubData() {
}
void stub_glGetUniformIndices() {
}
void stub_glGetActiveUniformsiv() {
}
void stub_glGetActiveUniformName() {
}
void stub_glGetUniformBlockIndex() {
}
void stub_glGetActiveUniformBlockiv() {
}
void stub_glGetActiveUniformBlockName() {
}
void stub_glUniformBlockBinding() {
}
void stub_glDrawElementsBaseVertex() {
}
void stub_glDrawRangeElementsBaseVertex() {
}
void stub_glDrawElementsInstancedBaseVertex() {
}
void stub_glMultiDrawElementsBaseVertex() {
}
void stub_glProvokingVertex() {
}
void stub_glFenceSync() {
}
void stub_glIsSync() {
}
void stub_glDeleteSync() {
}
void stub_glClientWaitSync() {
}
void stub_glWaitSync() {
}
void stub_glGetInteger64v() {
}
void stub_glGetSynciv() {
}
void stub_glGetInteger64i_v() {
}
void stub_glGetBufferParameteri64v() {
}
void stub_glFramebufferTexture() {
}
void stub_glTexImage2DMultisample() {
}
void stub_glTexImage3DMultisample() {
}
void stub_glGetMultisamplefv() {
}
void stub_glSampleMaski() {
}
void stub_glBindFragDataLocationIndexed() {
}
void stub_glGetFragDataIndex() {
}
void stub_glGenSamplers() {
}
void stub_glDeleteSamplers() {
}
void stub_glIsSampler() {
}
void stub_glBindSampler() {
}
void stub_glSamplerParameteri() {
}
void stub_glSamplerParameteriv() {
}
void stub_glSamplerParameterf() {
}
void stub_glSamplerParameterfv() {
}
void stub_glSamplerParameterIiv() {
}
void stub_glSamplerParameterIuiv() {
}
void stub_glGetSamplerParameteriv() {
}
void stub_glGetSamplerParameterIiv() {
}
void stub_glGetSamplerParameterfv() {
}
void stub_glGetSamplerParameterIuiv() {
}
void stub_glQueryCounter() {
}
void stub_glGetQueryObjecti64v() {
}
void stub_glGetQueryObjectui64v() {
}
void stub_glVertexAttribDivisor() {
}
void stub_glVertexAttribP1ui() {
}
void stub_glVertexAttribP1uiv() {
}
void stub_glVertexAttribP2ui() {
}
void stub_glVertexAttribP2uiv() {
}
void stub_glVertexAttribP3ui() {
}
void stub_glVertexAttribP3uiv() {
}
void stub_glVertexAttribP4ui() {
}
void stub_glVertexAttribP4uiv() {
}
void stub_glMinSampleShading() {
}
void stub_glBlendEquationi() {
}
void stub_glBlendEquationSeparatei() {
}
void stub_glBlendFunci() {
}
void stub_glBlendFuncSeparatei() {
}
void stub_glDrawArraysIndirect() {
}
void stub_glDrawElementsIndirect() {
}
void stub_glUniform1d() {
}
void stub_glUniform2d() {
}
void stub_glUniform3d() {
}
void stub_glUniform4d() {
}
void stub_glUniform1dv() {
}
void stub_glUniform2dv() {
}
void stub_glUniform3dv() {
}
void stub_glUniform4dv() {
}
void stub_glUniformMatrix2dv() {
}
void stub_glUniformMatrix3dv() {
}
void stub_glUniformMatrix4dv() {
}
void stub_glUniformMatrix2x3dv() {
}
void stub_glUniformMatrix2x4dv() {
}
void stub_glUniformMatrix3x2dv() {
}
void stub_glUniformMatrix3x4dv() {
}
void stub_glUniformMatrix4x2dv() {
}
void stub_glUniformMatrix4x3dv() {
}
void stub_glGetUniformdv() {
}
void stub_glGetSubroutineUniformLocation() {
}
void stub_glGetSubroutineIndex() {
}
void stub_glGetActiveSubroutineUniformiv() {
}
void stub_glGetActiveSubroutineUniformName() {
}
void stub_glGetActiveSubroutineName() {
}
void stub_glUniformSubroutinesuiv() {
}
void stub_glGetUniformSubroutineuiv() {
}
void stub_glGetProgramStageiv() {
}
void stub_glPatchParameteri() {
}
void stub_glPatchParameterfv() {
}
void stub_glBindTransformFeedback() {
}
void stub_glDeleteTransformFeedbacks() {
}
void stub_glGenTransformFeedbacks() {
}
void stub_glIsTransformFeedback() {
}
void stub_glPauseTransformFeedback() {
}
void stub_glResumeTransformFeedback() {
}
void stub_glDrawTransformFeedback() {
}
void stub_glDrawTransformFeedbackStream() {
}
void stub_glBeginQueryIndexed() {
}
void stub_glEndQueryIndexed() {
}
void stub_glGetQueryIndexediv() {
}
void stub_glReleaseShaderCompiler() {
}
void stub_glShaderBinary() {
}
void stub_glGetShaderPrecisionFormat() {
}
void stub_glDepthRangef() {
}
void stub_glClearDepthf() {
}
void stub_glGetProgramBinary() {
}
void stub_glProgramBinary() {
}
void stub_glProgramParameteri() {
}
void stub_glUseProgramStages() {
}
void stub_glActiveShaderProgram() {
}
void stub_glCreateShaderProgramv() {
}
void stub_glBindProgramPipeline() {
}
void stub_glDeleteProgramPipelines() {
}
void stub_glGenProgramPipelines() {
}
void stub_glIsProgramPipeline() {
}
void stub_glGetProgramPipelineiv() {
}
void stub_glProgramUniform1i() {
}
void stub_glProgramUniform1iv() {
}
void stub_glProgramUniform1f() {
}
void stub_glProgramUniform1fv() {
}
void stub_glProgramUniform1d() {
}
void stub_glProgramUniform1dv() {
}
void stub_glProgramUniform1ui() {
}
void stub_glProgramUniform1uiv() {
}
void stub_glProgramUniform2i() {
}
void stub_glProgramUniform2iv() {
}
void stub_glProgramUniform2f() {
}
void stub_glProgramUniform2fv() {
}
void stub_glProgramUniform2d() {
}
void stub_glProgramUniform2dv() {
}
void stub_glProgramUniform2ui() {
}
void stub_glProgramUniform2uiv() {
}
void stub_glProgramUniform3i() {
}
void stub_glProgramUniform3iv() {
}
void stub_glProgramUniform3f() {
}
void stub_glProgramUniform3fv() {
}
void stub_glProgramUniform3d() {
}
void stub_glProgramUniform3dv() {
}
void stub_glProgramUniform3ui() {
}
void stub_glProgramUniform3uiv() {
}
void stub_glProgramUniform4i() {
}
void stub_glProgramUniform4iv() {
}
void stub_glProgramUniform4f() {
}
void stub_glProgramUniform4fv() {
}
void stub_glProgramUniform4d() {
}
void stub_glProgramUniform4dv() {
}
void stub_glProgramUniform4ui() {
}
void stub_glProgramUniform4uiv() {
}
void stub_glProgramUniformMatrix2fv() {
}
void stub_glProgramUniformMatrix3fv() {
}
void stub_glProgramUniformMatrix4fv() {
}
void stub_glProgramUniformMatrix2dv() {
}
void stub_glProgramUniformMatrix3dv() {
}
void stub_glProgramUniformMatrix4dv() {
}
void stub_glProgramUniformMatrix2x3fv() {
}
void stub_glProgramUniformMatrix3x2fv() {
}
void stub_glProgramUniformMatrix2x4fv() {
}
void stub_glProgramUniformMatrix4x2fv() {
}
void stub_glProgramUniformMatrix3x4fv() {
}
void stub_glProgramUniformMatrix4x3fv() {
}
void stub_glProgramUniformMatrix2x3dv() {
}
void stub_glProgramUniformMatrix3x2dv() {
}
void stub_glProgramUniformMatrix2x4dv() {
}
void stub_glProgramUniformMatrix4x2dv() {
}
void stub_glProgramUniformMatrix3x4dv() {
}
void stub_glProgramUniformMatrix4x3dv() {
}
void stub_glValidateProgramPipeline() {
}
void stub_glGetProgramPipelineInfoLog() {
}
void stub_glVertexAttribL1d() {
}
void stub_glVertexAttribL2d() {
}
void stub_glVertexAttribL3d() {
}
void stub_glVertexAttribL4d() {
}
void stub_glVertexAttribL1dv() {
}
void stub_glVertexAttribL2dv() {
}
void stub_glVertexAttribL3dv() {
}
void stub_glVertexAttribL4dv() {
}
void stub_glVertexAttribLPointer() {
}
void stub_glGetVertexAttribLdv() {
}
void stub_glViewportArrayv() {
}
void stub_glViewportIndexedf() {
}
void stub_glViewportIndexedfv() {
}
void stub_glScissorArrayv() {
}
void stub_glScissorIndexed() {
}
void stub_glScissorIndexedv() {
}
void stub_glDepthRangeArrayv() {
}
void stub_glDepthRangeIndexed() {
}
void stub_glGetFloati_v() {
}
void stub_glGetDoublei_v() {
}
void stub_glDrawArraysInstancedBaseInstance() {
}
void stub_glDrawElementsInstancedBaseInstance() {
}
void stub_glDrawElementsInstancedBaseVertexBaseInstance() {
}
void stub_glGetInternalformativ() {
}
void stub_glGetActiveAtomicCounterBufferiv() {
}
void stub_glBindImageTexture() {
}
void stub_glMemoryBarrier() {
}
void stub_glTexStorage1D() {
}
void stub_glTexStorage2D() {
}
void stub_glTexStorage3D() {
}
void stub_glDrawTransformFeedbackInstanced() {
}
void stub_glDrawTransformFeedbackStreamInstanced() {
}
void stub_glClearBufferData() {
}
void stub_glClearBufferSubData() {
}
void stub_glDispatchCompute() {
}
void stub_glDispatchComputeIndirect() {
}
void stub_glCopyImageSubData() {
}
void stub_glFramebufferParameteri() {
}
void stub_glGetFramebufferParameteriv() {
}
void stub_glGetInternalformati64v() {
}
void stub_glInvalidateTexSubImage() {
}
void stub_glInvalidateTexImage() {
}
void stub_glInvalidateBufferSubData() {
}
void stub_glInvalidateBufferData() {
}
void stub_glInvalidateFramebuffer() {
}
void stub_glInvalidateSubFramebuffer() {
}
void stub_glMultiDrawArraysIndirect() {
}
void stub_glMultiDrawElementsIndirect() {
}
void stub_glGetProgramInterfaceiv() {
}
void stub_glGetProgramResourceIndex() {
}
void stub_glGetProgramResourceName() {
}
void stub_glGetProgramResourceiv() {
}
void stub_glGetProgramResourceLocation() {
}
void stub_glGetProgramResourceLocationIndex() {
}
void stub_glShaderStorageBlockBinding() {
}
void stub_glTexBufferRange() {
}
void stub_glTexStorage2DMultisample() {
}
void stub_glTexStorage3DMultisample() {
}
void stub_glTextureView() {
}
void stub_glBindVertexBuffer() {
}
void stub_glVertexAttribFormat() {
}
void stub_glVertexAttribIFormat() {
}
void stub_glVertexAttribLFormat() {
}
void stub_glVertexAttribBinding() {
}
void stub_glVertexBindingDivisor() {
}
void stub_glDebugMessageControl() {
}
void stub_glDebugMessageInsert() {
}
void stub_glDebugMessageCallback() {
}
void stub_glGetDebugMessageLog() {
}
void stub_glPushDebugGroup() {
}
void stub_glPopDebugGroup() {
}
void stub_glObjectLabel() {
}
void stub_glGetObjectLabel() {
}
void stub_glObjectPtrLabel() {
}
void stub_glGetObjectPtrLabel() {
}
void stub_glBufferStorage() {
}
void stub_glClearTexImage() {
}
void stub_glClearTexSubImage() {
}
void stub_glBindBuffersBase() {
}
void stub_glBindBuffersRange() {
}
void stub_glBindTextures() {
}
void stub_glBindSamplers() {
}
void stub_glBindImageTextures() {
}
void stub_glBindVertexBuffers() {
}
void stub_glClipControl() {
}
void stub_glCreateTransformFeedbacks() {
}
void stub_glTransformFeedbackBufferBase() {
}
void stub_glTransformFeedbackBufferRange() {
}
void stub_glGetTransformFeedbackiv() {
}
void stub_glGetTransformFeedbacki_v() {
}
void stub_glGetTransformFeedbacki64_v() {
}
void stub_glCreateBuffers() {
}
void stub_glNamedBufferStorage() {
}
void stub_glNamedBufferData() {
}
void stub_glNamedBufferSubData() {
}
void stub_glCopyNamedBufferSubData() {
}
void stub_glClearNamedBufferData() {
}
void stub_glClearNamedBufferSubData() {
}
void stub_glMapNamedBuffer() {
}
void stub_glMapNamedBufferRange() {
}
void stub_glUnmapNamedBuffer() {
}
void stub_glFlushMappedNamedBufferRange() {
}
void stub_glGetNamedBufferParameteriv() {
}
void stub_glGetNamedBufferParameteri64v() {
}
void stub_glGetNamedBufferPointerv() {
}
void stub_glGetNamedBufferSubData() {
}
void stub_glCreateFramebuffers() {
}
void stub_glNamedFramebufferRenderbuffer() {
}
void stub_glNamedFramebufferParameteri() {
}
void stub_glNamedFramebufferTexture() {
}
void stub_glNamedFramebufferTextureLayer() {
}
void stub_glNamedFramebufferDrawBuffer() {
}
void stub_glNamedFramebufferDrawBuffers() {
}
void stub_glNamedFramebufferReadBuffer() {
}
void stub_glInvalidateNamedFramebufferData() {
}
void stub_glInvalidateNamedFramebufferSubData() {
}
void stub_glClearNamedFramebufferiv() {
}
void stub_glClearNamedFramebufferuiv() {
}
void stub_glClearNamedFramebufferfv() {
}
void stub_glClearNamedFramebufferfi() {
}
void stub_glBlitNamedFramebuffer() {
}
void stub_glCheckNamedFramebufferStatus() {
}
void stub_glGetNamedFramebufferParameteriv() {
}
void stub_glGetNamedFramebufferAttachmentParameteriv() {
}
void stub_glCreateRenderbuffers() {
}
void stub_glNamedRenderbufferStorage() {
}
void stub_glNamedRenderbufferStorageMultisample() {
}
void stub_glGetNamedRenderbufferParameteriv() {
}
void stub_glCreateTextures() {
}
void stub_glTextureBuffer() {
}
void stub_glTextureBufferRange() {
}
void stub_glTextureStorage1D() {
}
void stub_glTextureStorage2D() {
}
void stub_glTextureStorage3D() {
}
void stub_glTextureStorage2DMultisample() {
}
void stub_glTextureStorage3DMultisample() {
}
void stub_glTextureSubImage1D() {
}
void stub_glTextureSubImage2D() {
}
void stub_glTextureSubImage3D() {
}
void stub_glCompressedTextureSubImage1D() {
}
void stub_glCompressedTextureSubImage2D() {
}
void stub_glCompressedTextureSubImage3D() {
}
void stub_glCopyTextureSubImage1D() {
}
void stub_glCopyTextureSubImage2D() {
}
void stub_glCopyTextureSubImage3D() {
}
void stub_glTextureParameterf() {
}
void stub_glTextureParameterfv() {
}
void stub_glTextureParameteri() {
}
void stub_glTextureParameterIiv() {
}
void stub_glTextureParameterIuiv() {
}
void stub_glTextureParameteriv() {
}
void stub_glGenerateTextureMipmap() {
}
void stub_glBindTextureUnit() {
}
void stub_glGetTextureImage() {
}
void stub_glGetCompressedTextureImage() {
}
void stub_glGetTextureLevelParameterfv() {
}
void stub_glGetTextureLevelParameteriv() {
}
void stub_glGetTextureParameterfv() {
}
void stub_glGetTextureParameterIiv() {
}
void stub_glGetTextureParameterIuiv() {
}
void stub_glGetTextureParameteriv() {
}
void stub_glCreateVertexArrays() {
}
void stub_glDisableVertexArrayAttrib() {
}
void stub_glEnableVertexArrayAttrib() {
}
void stub_glVertexArrayElementBuffer() {
}
void stub_glVertexArrayVertexBuffer() {
}
void stub_glVertexArrayVertexBuffers() {
}
void stub_glVertexArrayAttribBinding() {
}
void stub_glVertexArrayAttribFormat() {
}
void stub_glVertexArrayAttribIFormat() {
}
void stub_glVertexArrayAttribLFormat() {
}
void stub_glVertexArrayBindingDivisor() {
}
void stub_glGetVertexArrayiv() {
}
void stub_glGetVertexArrayIndexediv() {
}
void stub_glGetVertexArrayIndexed64iv() {
}
void stub_glCreateSamplers() {
}
void stub_glCreateProgramPipelines() {
}
void stub_glCreateQueries() {
}
void stub_glGetQueryBufferObjecti64v() {
}
void stub_glGetQueryBufferObjectiv() {
}
void stub_glGetQueryBufferObjectui64v() {
}
void stub_glGetQueryBufferObjectuiv() {
}
void stub_glMemoryBarrierByRegion() {
}
void stub_glGetTextureSubImage() {
}
void stub_glGetCompressedTextureSubImage() {
}
void stub_glGetGraphicsResetStatus() {
}
void stub_glGetnCompressedTexImage() {
}
void stub_glGetnTexImage() {
}
void stub_glGetnUniformdv() {
}
void stub_glGetnUniformfv() {
}
void stub_glGetnUniformiv() {
}
void stub_glGetnUniformuiv() {
}
void stub_glReadnPixels() {
}
void stub_glTextureBarrier() {
}
void stub_glSpecializeShader() {
}
void stub_glMultiDrawArraysIndirectCount() {
}
void stub_glMultiDrawElementsIndirectCount() {
}
void stub_glPolygonOffsetClamp() {
}
void stub_glPrimitiveBoundingBoxARB() {
}
void stub_glGetTextureHandleARB() {
}
void stub_glGetTextureSamplerHandleARB() {
}
void stub_glMakeTextureHandleResidentARB() {
}
void stub_glMakeTextureHandleNonResidentARB() {
}
void stub_glGetImageHandleARB() {
}
void stub_glMakeImageHandleResidentARB() {
}
void stub_glMakeImageHandleNonResidentARB() {
}
void stub_glUniformHandleui64ARB() {
}
void stub_glUniformHandleui64vARB() {
}
void stub_glProgramUniformHandleui64ARB() {
}
void stub_glProgramUniformHandleui64vARB() {
}
void stub_glIsTextureHandleResidentARB() {
}
void stub_glIsImageHandleResidentARB() {
}
void stub_glVertexAttribL1ui64ARB() {
}
void stub_glVertexAttribL1ui64vARB() {
}
void stub_glGetVertexAttribLui64vARB() {
}
void stub_glCreateSyncFromCLeventARB() {
}
void stub_glDispatchComputeGroupSizeARB() {
}
void stub_glDebugMessageControlARB() {
}
void stub_glDebugMessageInsertARB() {
}
void stub_glDebugMessageCallbackARB() {
}
void stub_glGetDebugMessageLogARB() {
}
void stub_glBlendEquationiARB() {
}
void stub_glBlendEquationSeparateiARB() {
}
void stub_glBlendFunciARB() {
}
void stub_glBlendFuncSeparateiARB() {
}
void stub_glDrawArraysInstancedARB() {
}
void stub_glDrawElementsInstancedARB() {
}
void stub_glProgramParameteriARB() {
}
void stub_glFramebufferTextureARB() {
}
void stub_glFramebufferTextureLayerARB() {
}
void stub_glFramebufferTextureFaceARB() {
}
void stub_glSpecializeShaderARB() {
}
void stub_glUniform1i64ARB() {
}
void stub_glUniform2i64ARB() {
}
void stub_glUniform3i64ARB() {
}
void stub_glUniform4i64ARB() {
}
void stub_glUniform1i64vARB() {
}
void stub_glUniform2i64vARB() {
}
void stub_glUniform3i64vARB() {
}
void stub_glUniform4i64vARB() {
}
void stub_glUniform1ui64ARB() {
}
void stub_glUniform2ui64ARB() {
}
void stub_glUniform3ui64ARB() {
}
void stub_glUniform4ui64ARB() {
}
void stub_glUniform1ui64vARB() {
}
void stub_glUniform2ui64vARB() {
}
void stub_glUniform3ui64vARB() {
}
void stub_glUniform4ui64vARB() {
}
void stub_glGetUniformi64vARB() {
}
void stub_glGetUniformui64vARB() {
}
void stub_glGetnUniformi64vARB() {
}
void stub_glGetnUniformui64vARB() {
}
void stub_glProgramUniform1i64ARB() {
}
void stub_glProgramUniform2i64ARB() {
}
void stub_glProgramUniform3i64ARB() {
}
void stub_glProgramUniform4i64ARB() {
}
void stub_glProgramUniform1i64vARB() {
}
void stub_glProgramUniform2i64vARB() {
}
void stub_glProgramUniform3i64vARB() {
}
void stub_glProgramUniform4i64vARB() {
}
void stub_glProgramUniform1ui64ARB() {
}
void stub_glProgramUniform2ui64ARB() {
}
void stub_glProgramUniform3ui64ARB() {
}
void stub_glProgramUniform4ui64ARB() {
}
void stub_glProgramUniform1ui64vARB() {
}
void stub_glProgramUniform2ui64vARB() {
}
void stub_glProgramUniform3ui64vARB() {
}
void stub_glProgramUniform4ui64vARB() {
}
void stub_glMultiDrawArraysIndirectCountARB() {
}
void stub_glMultiDrawElementsIndirectCountARB() {
}
void stub_glVertexAttribDivisorARB() {
}
void stub_glMaxShaderCompilerThreadsARB() {
}
void stub_glGetGraphicsResetStatusARB() {
}
void stub_glGetnTexImageARB() {
}
void stub_glReadnPixelsARB() {
}
void stub_glGetnCompressedTexImageARB() {
}
void stub_glGetnUniformfvARB() {
}
void stub_glGetnUniformivARB() {
}
void stub_glGetnUniformuivARB() {
}
void stub_glGetnUniformdvARB() {
}
void stub_glFramebufferSampleLocationsfvARB() {
}
void stub_glNamedFramebufferSampleLocationsfvARB() {
}
void stub_glEvaluateDepthValuesARB() {
}
void stub_glMinSampleShadingARB() {
}
void stub_glNamedStringARB() {
}
void stub_glDeleteNamedStringARB() {
}
void stub_glCompileShaderIncludeARB() {
}
void stub_glIsNamedStringARB() {
}
void stub_glGetNamedStringARB() {
}
void stub_glGetNamedStringivARB() {
}
void stub_glBufferPageCommitmentARB() {
}
void stub_glNamedBufferPageCommitmentEXT() {
}
void stub_glNamedBufferPageCommitmentARB() {
}
void stub_glTexPageCommitmentARB() {
}
void stub_glTexBufferARB() {
}
void stub_glDepthRangeArraydvNV() {
}
void stub_glDepthRangeIndexeddNV() {
}
void stub_glBlendBarrierKHR() {
}
void stub_glMaxShaderCompilerThreadsKHR() {
}
void stub_glRenderbufferStorageMultisampleAdvancedAMD() {
}
void stub_glNamedRenderbufferStorageMultisampleAdvancedAMD() {
}
void stub_glGetPerfMonitorGroupsAMD() {
}
void stub_glGetPerfMonitorCountersAMD() {
}
void stub_glGetPerfMonitorGroupStringAMD() {
}
void stub_glGetPerfMonitorCounterStringAMD() {
}
void stub_glGetPerfMonitorCounterInfoAMD() {
}
void stub_glGenPerfMonitorsAMD() {
}
void stub_glDeletePerfMonitorsAMD() {
}
void stub_glSelectPerfMonitorCountersAMD() {
}
void stub_glBeginPerfMonitorAMD() {
}
void stub_glEndPerfMonitorAMD() {
}
void stub_glGetPerfMonitorCounterDataAMD() {
}
void stub_glEGLImageTargetTexStorageEXT() {
}
void stub_glEGLImageTargetTextureStorageEXT() {
}
void stub_glLabelObjectEXT() {
}
void stub_glGetObjectLabelEXT() {
}
void stub_glInsertEventMarkerEXT() {
}
void stub_glPushGroupMarkerEXT() {
}
void stub_glPopGroupMarkerEXT() {
}
void stub_glMatrixLoadfEXT() {
}
void stub_glMatrixLoaddEXT() {
}
void stub_glMatrixMultfEXT() {
}
void stub_glMatrixMultdEXT() {
}
void stub_glMatrixLoadIdentityEXT() {
}
void stub_glMatrixRotatefEXT() {
}
void stub_glMatrixRotatedEXT() {
}
void stub_glMatrixScalefEXT() {
}
void stub_glMatrixScaledEXT() {
}
void stub_glMatrixTranslatefEXT() {
}
void stub_glMatrixTranslatedEXT() {
}
void stub_glMatrixFrustumEXT() {
}
void stub_glMatrixOrthoEXT() {
}
void stub_glMatrixPopEXT() {
}
void stub_glMatrixPushEXT() {
}
void stub_glClientAttribDefaultEXT() {
}
void stub_glPushClientAttribDefaultEXT() {
}
void stub_glTextureParameterfEXT() {
}
void stub_glTextureParameterfvEXT() {
}
void stub_glTextureParameteriEXT() {
}
void stub_glTextureParameterivEXT() {
}
void stub_glTextureImage1DEXT() {
}
void stub_glTextureImage2DEXT() {
}
void stub_glTextureSubImage1DEXT() {
}
void stub_glTextureSubImage2DEXT() {
}
void stub_glCopyTextureImage1DEXT() {
}
void stub_glCopyTextureImage2DEXT() {
}
void stub_glCopyTextureSubImage1DEXT() {
}
void stub_glCopyTextureSubImage2DEXT() {
}
void stub_glGetTextureImageEXT() {
}
void stub_glGetTextureParameterfvEXT() {
}
void stub_glGetTextureParameterivEXT() {
}
void stub_glGetTextureLevelParameterfvEXT() {
}
void stub_glGetTextureLevelParameterivEXT() {
}
void stub_glTextureImage3DEXT() {
}
void stub_glTextureSubImage3DEXT() {
}
void stub_glCopyTextureSubImage3DEXT() {
}
void stub_glBindMultiTextureEXT() {
}
void stub_glMultiTexCoordPointerEXT() {
}
void stub_glMultiTexEnvfEXT() {
}
void stub_glMultiTexEnvfvEXT() {
}
void stub_glMultiTexEnviEXT() {
}
void stub_glMultiTexEnvivEXT() {
}
void stub_glMultiTexGendEXT() {
}
void stub_glMultiTexGendvEXT() {
}
void stub_glMultiTexGenfEXT() {
}
void stub_glMultiTexGenfvEXT() {
}
void stub_glMultiTexGeniEXT() {
}
void stub_glMultiTexGenivEXT() {
}
void stub_glGetMultiTexEnvfvEXT() {
}
void stub_glGetMultiTexEnvivEXT() {
}
void stub_glGetMultiTexGendvEXT() {
}
void stub_glGetMultiTexGenfvEXT() {
}
void stub_glGetMultiTexGenivEXT() {
}
void stub_glMultiTexParameteriEXT() {
}
void stub_glMultiTexParameterivEXT() {
}
void stub_glMultiTexParameterfEXT() {
}
void stub_glMultiTexParameterfvEXT() {
}
void stub_glMultiTexImage1DEXT() {
}
void stub_glMultiTexImage2DEXT() {
}
void stub_glMultiTexSubImage1DEXT() {
}
void stub_glMultiTexSubImage2DEXT() {
}
void stub_glCopyMultiTexImage1DEXT() {
}
void stub_glCopyMultiTexImage2DEXT() {
}
void stub_glCopyMultiTexSubImage1DEXT() {
}
void stub_glCopyMultiTexSubImage2DEXT() {
}
void stub_glGetMultiTexImageEXT() {
}
void stub_glGetMultiTexParameterfvEXT() {
}
void stub_glGetMultiTexParameterivEXT() {
}
void stub_glGetMultiTexLevelParameterfvEXT() {
}
void stub_glGetMultiTexLevelParameterivEXT() {
}
void stub_glMultiTexImage3DEXT() {
}
void stub_glMultiTexSubImage3DEXT() {
}
void stub_glCopyMultiTexSubImage3DEXT() {
}
void stub_glEnableClientStateIndexedEXT() {
}
void stub_glDisableClientStateIndexedEXT() {
}
void stub_glGetFloatIndexedvEXT() {
}
void stub_glGetDoubleIndexedvEXT() {
}
void stub_glGetPointerIndexedvEXT() {
}
void stub_glEnableIndexedEXT() {
}
void stub_glDisableIndexedEXT() {
}
void stub_glIsEnabledIndexedEXT() {
}
void stub_glGetIntegerIndexedvEXT() {
}
void stub_glGetBooleanIndexedvEXT() {
}
void stub_glCompressedTextureImage3DEXT() {
}
void stub_glCompressedTextureImage2DEXT() {
}
void stub_glCompressedTextureImage1DEXT() {
}
void stub_glCompressedTextureSubImage3DEXT() {
}
void stub_glCompressedTextureSubImage2DEXT() {
}
void stub_glCompressedTextureSubImage1DEXT() {
}
void stub_glGetCompressedTextureImageEXT() {
}
void stub_glCompressedMultiTexImage3DEXT() {
}
void stub_glCompressedMultiTexImage2DEXT() {
}
void stub_glCompressedMultiTexImage1DEXT() {
}
void stub_glCompressedMultiTexSubImage3DEXT() {
}
void stub_glCompressedMultiTexSubImage2DEXT() {
}
void stub_glCompressedMultiTexSubImage1DEXT() {
}
void stub_glGetCompressedMultiTexImageEXT() {
}
void stub_glMatrixLoadTransposefEXT() {
}
void stub_glMatrixLoadTransposedEXT() {
}
void stub_glMatrixMultTransposefEXT() {
}
void stub_glMatrixMultTransposedEXT() {
}
void stub_glNamedBufferDataEXT() {
}
void stub_glNamedBufferSubDataEXT() {
}
void stub_glMapNamedBufferEXT() {
}
void stub_glUnmapNamedBufferEXT() {
}
void stub_glGetNamedBufferParameterivEXT() {
}
void stub_glGetNamedBufferPointervEXT() {
}
void stub_glGetNamedBufferSubDataEXT() {
}
void stub_glProgramUniform1fEXT() {
}
void stub_glProgramUniform2fEXT() {
}
void stub_glProgramUniform3fEXT() {
}
void stub_glProgramUniform4fEXT() {
}
void stub_glProgramUniform1iEXT() {
}
void stub_glProgramUniform2iEXT() {
}
void stub_glProgramUniform3iEXT() {
}
void stub_glProgramUniform4iEXT() {
}
void stub_glProgramUniform1fvEXT() {
}
void stub_glProgramUniform2fvEXT() {
}
void stub_glProgramUniform3fvEXT() {
}
void stub_glProgramUniform4fvEXT() {
}
void stub_glProgramUniform1ivEXT() {
}
void stub_glProgramUniform2ivEXT() {
}
void stub_glProgramUniform3ivEXT() {
}
void stub_glProgramUniform4ivEXT() {
}
void stub_glProgramUniformMatrix2fvEXT() {
}
void stub_glProgramUniformMatrix3fvEXT() {
}
void stub_glProgramUniformMatrix4fvEXT() {
}
void stub_glProgramUniformMatrix2x3fvEXT() {
}
void stub_glProgramUniformMatrix3x2fvEXT() {
}
void stub_glProgramUniformMatrix2x4fvEXT() {
}
void stub_glProgramUniformMatrix4x2fvEXT() {
}
void stub_glProgramUniformMatrix3x4fvEXT() {
}
void stub_glProgramUniformMatrix4x3fvEXT() {
}
void stub_glTextureBufferEXT() {
}
void stub_glMultiTexBufferEXT() {
}
void stub_glTextureParameterIivEXT() {
}
void stub_glTextureParameterIuivEXT() {
}
void stub_glGetTextureParameterIivEXT() {
}
void stub_glGetTextureParameterIuivEXT() {
}
void stub_glMultiTexParameterIivEXT() {
}
void stub_glMultiTexParameterIuivEXT() {
}
void stub_glGetMultiTexParameterIivEXT() {
}
void stub_glGetMultiTexParameterIuivEXT() {
}
void stub_glProgramUniform1uiEXT() {
}
void stub_glProgramUniform2uiEXT() {
}
void stub_glProgramUniform3uiEXT() {
}
void stub_glProgramUniform4uiEXT() {
}
void stub_glProgramUniform1uivEXT() {
}
void stub_glProgramUniform2uivEXT() {
}
void stub_glProgramUniform3uivEXT() {
}
void stub_glProgramUniform4uivEXT() {
}
void stub_glNamedProgramLocalParameters4fvEXT() {
}
void stub_glNamedProgramLocalParameterI4iEXT() {
}
void stub_glNamedProgramLocalParameterI4ivEXT() {
}
void stub_glNamedProgramLocalParametersI4ivEXT() {
}
void stub_glNamedProgramLocalParameterI4uiEXT() {
}
void stub_glNamedProgramLocalParameterI4uivEXT() {
}
void stub_glNamedProgramLocalParametersI4uivEXT() {
}
void stub_glGetNamedProgramLocalParameterIivEXT() {
}
void stub_glGetNamedProgramLocalParameterIuivEXT() {
}
void stub_glEnableClientStateiEXT() {
}
void stub_glDisableClientStateiEXT() {
}
void stub_glGetFloati_vEXT() {
}
void stub_glGetDoublei_vEXT() {
}
void stub_glGetPointeri_vEXT() {
}
void stub_glNamedProgramStringEXT() {
}
void stub_glNamedProgramLocalParameter4dEXT() {
}
void stub_glNamedProgramLocalParameter4dvEXT() {
}
void stub_glNamedProgramLocalParameter4fEXT() {
}
void stub_glNamedProgramLocalParameter4fvEXT() {
}
void stub_glGetNamedProgramLocalParameterdvEXT() {
}
void stub_glGetNamedProgramLocalParameterfvEXT() {
}
void stub_glGetNamedProgramivEXT() {
}
void stub_glGetNamedProgramStringEXT() {
}
void stub_glNamedRenderbufferStorageEXT() {
}
void stub_glGetNamedRenderbufferParameterivEXT() {
}
void stub_glNamedRenderbufferStorageMultisampleEXT() {
}
void stub_glNamedRenderbufferStorageMultisampleCoverageEXT() {
}
void stub_glCheckNamedFramebufferStatusEXT() {
}
void stub_glNamedFramebufferTexture1DEXT() {
}
void stub_glNamedFramebufferTexture2DEXT() {
}
void stub_glNamedFramebufferTexture3DEXT() {
}
void stub_glNamedFramebufferRenderbufferEXT() {
}
void stub_glGetNamedFramebufferAttachmentParameterivEXT() {
}
void stub_glGenerateTextureMipmapEXT() {
}
void stub_glGenerateMultiTexMipmapEXT() {
}
void stub_glFramebufferDrawBufferEXT() {
}
void stub_glFramebufferDrawBuffersEXT() {
}
void stub_glFramebufferReadBufferEXT() {
}
void stub_glGetFramebufferParameterivEXT() {
}
void stub_glNamedCopyBufferSubDataEXT() {
}
void stub_glNamedFramebufferTextureEXT() {
}
void stub_glNamedFramebufferTextureLayerEXT() {
}
void stub_glNamedFramebufferTextureFaceEXT() {
}
void stub_glTextureRenderbufferEXT() {
}
void stub_glMultiTexRenderbufferEXT() {
}
void stub_glVertexArrayVertexOffsetEXT() {
}
void stub_glVertexArrayColorOffsetEXT() {
}
void stub_glVertexArrayEdgeFlagOffsetEXT() {
}
void stub_glVertexArrayIndexOffsetEXT() {
}
void stub_glVertexArrayNormalOffsetEXT() {
}
void stub_glVertexArrayTexCoordOffsetEXT() {
}
void stub_glVertexArrayMultiTexCoordOffsetEXT() {
}
void stub_glVertexArrayFogCoordOffsetEXT() {
}
void stub_glVertexArraySecondaryColorOffsetEXT() {
}
void stub_glVertexArrayVertexAttribOffsetEXT() {
}
void stub_glVertexArrayVertexAttribIOffsetEXT() {
}
void stub_glEnableVertexArrayEXT() {
}
void stub_glDisableVertexArrayEXT() {
}
void stub_glEnableVertexArrayAttribEXT() {
}
void stub_glDisableVertexArrayAttribEXT() {
}
void stub_glGetVertexArrayIntegervEXT() {
}
void stub_glGetVertexArrayPointervEXT() {
}
void stub_glGetVertexArrayIntegeri_vEXT() {
}
void stub_glGetVertexArrayPointeri_vEXT() {
}
void stub_glMapNamedBufferRangeEXT() {
}
void stub_glFlushMappedNamedBufferRangeEXT() {
}
void stub_glNamedBufferStorageEXT() {
}
void stub_glClearNamedBufferDataEXT() {
}
void stub_glClearNamedBufferSubDataEXT() {
}
void stub_glNamedFramebufferParameteriEXT() {
}
void stub_glGetNamedFramebufferParameterivEXT() {
}
void stub_glProgramUniform1dEXT() {
}
void stub_glProgramUniform2dEXT() {
}
void stub_glProgramUniform3dEXT() {
}
void stub_glProgramUniform4dEXT() {
}
void stub_glProgramUniform1dvEXT() {
}
void stub_glProgramUniform2dvEXT() {
}
void stub_glProgramUniform3dvEXT() {
}
void stub_glProgramUniform4dvEXT() {
}
void stub_glProgramUniformMatrix2dvEXT() {
}
void stub_glProgramUniformMatrix3dvEXT() {
}
void stub_glProgramUniformMatrix4dvEXT() {
}
void stub_glProgramUniformMatrix2x3dvEXT() {
}
void stub_glProgramUniformMatrix2x4dvEXT() {
}
void stub_glProgramUniformMatrix3x2dvEXT() {
}
void stub_glProgramUniformMatrix3x4dvEXT() {
}
void stub_glProgramUniformMatrix4x2dvEXT() {
}
void stub_glProgramUniformMatrix4x3dvEXT() {
}
void stub_glTextureBufferRangeEXT() {
}
void stub_glTextureStorage1DEXT() {
}
void stub_glTextureStorage2DEXT() {
}
void stub_glTextureStorage3DEXT() {
}
void stub_glTextureStorage2DMultisampleEXT() {
}
void stub_glTextureStorage3DMultisampleEXT() {
}
void stub_glVertexArrayBindVertexBufferEXT() {
}
void stub_glVertexArrayVertexAttribFormatEXT() {
}
void stub_glVertexArrayVertexAttribIFormatEXT() {
}
void stub_glVertexArrayVertexAttribLFormatEXT() {
}
void stub_glVertexArrayVertexAttribBindingEXT() {
}
void stub_glVertexArrayVertexBindingDivisorEXT() {
}
void stub_glVertexArrayVertexAttribLOffsetEXT() {
}
void stub_glTexturePageCommitmentEXT() {
}
void stub_glVertexArrayVertexAttribDivisorEXT() {
}
void stub_glDrawArraysInstancedEXT() {
}
void stub_glDrawElementsInstancedEXT() {
}
void stub_glPolygonOffsetClampEXT() {
}
void stub_glRasterSamplesEXT() {
}
void stub_glUseShaderProgramEXT() {
}
void stub_glActiveProgramEXT() {
}
void stub_glCreateShaderProgramEXT() {
}
void stub_glFramebufferFetchBarrierEXT() {
}
void stub_glTexStorage1DEXT() {
}
void stub_glTexStorage2DEXT() {
}
void stub_glTexStorage3DEXT() {
}
void stub_glWindowRectanglesEXT() {
}
void stub_glApplyFramebufferAttachmentCMAAINTEL() {
}
void stub_glBeginPerfQueryINTEL() {
}
void stub_glCreatePerfQueryINTEL() {
}
void stub_glDeletePerfQueryINTEL() {
}
void stub_glEndPerfQueryINTEL() {
}
void stub_glGetFirstPerfQueryIdINTEL() {
}
void stub_glGetNextPerfQueryIdINTEL() {
}
void stub_glGetPerfCounterInfoINTEL() {
}
void stub_glGetPerfQueryDataINTEL() {
}
void stub_glGetPerfQueryIdByNameINTEL() {
}
void stub_glGetPerfQueryInfoINTEL() {
}
void stub_glFramebufferParameteriMESA() {
}
void stub_glGetFramebufferParameterivMESA() {
}
void stub_glMultiDrawArraysIndirectBindlessNV() {
}
void stub_glMultiDrawElementsIndirectBindlessNV() {
}
void stub_glMultiDrawArraysIndirectBindlessCountNV() {
}
void stub_glMultiDrawElementsIndirectBindlessCountNV() {
}
void stub_glGetTextureHandleNV() {
}
void stub_glGetTextureSamplerHandleNV() {
}
void stub_glMakeTextureHandleResidentNV() {
}
void stub_glMakeTextureHandleNonResidentNV() {
}
void stub_glGetImageHandleNV() {
}
void stub_glMakeImageHandleResidentNV() {
}
void stub_glMakeImageHandleNonResidentNV() {
}
void stub_glUniformHandleui64NV() {
}
void stub_glUniformHandleui64vNV() {
}
void stub_glProgramUniformHandleui64NV() {
}
void stub_glProgramUniformHandleui64vNV() {
}
void stub_glIsTextureHandleResidentNV() {
}
void stub_glIsImageHandleResidentNV() {
}
void stub_glBlendParameteriNV() {
}
void stub_glBlendBarrierNV() {
}
void stub_glViewportPositionWScaleNV() {
}
void stub_glCreateStatesNV() {
}
void stub_glDeleteStatesNV() {
}
void stub_glIsStateNV() {
}
void stub_glStateCaptureNV() {
}
void stub_glGetCommandHeaderNV() {
}
void stub_glGetStageIndexNV() {
}
void stub_glDrawCommandsNV() {
}
void stub_glDrawCommandsAddressNV() {
}
void stub_glDrawCommandsStatesNV() {
}
void stub_glDrawCommandsStatesAddressNV() {
}
void stub_glCreateCommandListsNV() {
}
void stub_glDeleteCommandListsNV() {
}
void stub_glIsCommandListNV() {
}
void stub_glListDrawCommandsStatesClientNV() {
}
void stub_glCommandListSegmentsNV() {
}
void stub_glCompileCommandListNV() {
}
void stub_glCallCommandListNV() {
}
void stub_glBeginConditionalRenderNV() {
}
void stub_glEndConditionalRenderNV() {
}
void stub_glSubpixelPrecisionBiasNV() {
}
void stub_glConservativeRasterParameterfNV() {
}
void stub_glConservativeRasterParameteriNV() {
}
void stub_glDepthRangedNV() {
}
void stub_glClearDepthdNV() {
}
void stub_glDepthBoundsdNV() {
}
void stub_glDrawVkImageNV() {
}
void stub_glGetVkProcAddrNV() {
}
void stub_glWaitVkSemaphoreNV() {
}
void stub_glSignalVkSemaphoreNV() {
}
void stub_glSignalVkFenceNV() {
}
void stub_glFragmentCoverageColorNV() {
}
void stub_glCoverageModulationTableNV() {
}
void stub_glGetCoverageModulationTableNV() {
}
void stub_glCoverageModulationNV() {
}
void stub_glRenderbufferStorageMultisampleCoverageNV() {
}
void stub_glUniform1i64NV() {
}
void stub_glUniform2i64NV() {
}
void stub_glUniform3i64NV() {
}
void stub_glUniform4i64NV() {
}
void stub_glUniform1i64vNV() {
}
void stub_glUniform2i64vNV() {
}
void stub_glUniform3i64vNV() {
}
void stub_glUniform4i64vNV() {
}
void stub_glUniform1ui64NV() {
}
void stub_glUniform2ui64NV() {
}
void stub_glUniform3ui64NV() {
}
void stub_glUniform4ui64NV() {
}
void stub_glUniform1ui64vNV() {
}
void stub_glUniform2ui64vNV() {
}
void stub_glUniform3ui64vNV() {
}
void stub_glUniform4ui64vNV() {
}
void stub_glGetUniformi64vNV() {
}
void stub_glProgramUniform1i64NV() {
}
void stub_glProgramUniform2i64NV() {
}
void stub_glProgramUniform3i64NV() {
}
void stub_glProgramUniform4i64NV() {
}
void stub_glProgramUniform1i64vNV() {
}
void stub_glProgramUniform2i64vNV() {
}
void stub_glProgramUniform3i64vNV() {
}
void stub_glProgramUniform4i64vNV() {
}
void stub_glProgramUniform1ui64NV() {
}
void stub_glProgramUniform2ui64NV() {
}
void stub_glProgramUniform3ui64NV() {
}
void stub_glProgramUniform4ui64NV() {
}
void stub_glProgramUniform1ui64vNV() {
}
void stub_glProgramUniform2ui64vNV() {
}
void stub_glProgramUniform3ui64vNV() {
}
void stub_glProgramUniform4ui64vNV() {
}
void stub_glGetInternalformatSampleivNV() {
}
void stub_glGetMemoryObjectDetachedResourcesuivNV() {
}
void stub_glResetMemoryObjectParameterNV() {
}
void stub_glTexAttachMemoryNV() {
}
void stub_glBufferAttachMemoryNV() {
}
void stub_glTextureAttachMemoryNV() {
}
void stub_glNamedBufferAttachMemoryNV() {
}
void stub_glBufferPageCommitmentMemNV() {
}
void stub_glTexPageCommitmentMemNV() {
}
void stub_glNamedBufferPageCommitmentMemNV() {
}
void stub_glTexturePageCommitmentMemNV() {
}
void stub_glDrawMeshTasksNV() {
}
void stub_glDrawMeshTasksIndirectNV() {
}
void stub_glMultiDrawMeshTasksIndirectNV() {
}
void stub_glMultiDrawMeshTasksIndirectCountNV() {
}
void stub_glGenPathsNV() {
}
void stub_glDeletePathsNV() {
}
void stub_glIsPathNV() {
}
void stub_glPathCommandsNV() {
}
void stub_glPathCoordsNV() {
}
void stub_glPathSubCommandsNV() {
}
void stub_glPathSubCoordsNV() {
}
void stub_glPathStringNV() {
}
void stub_glPathGlyphsNV() {
}
void stub_glPathGlyphRangeNV() {
}
void stub_glWeightPathsNV() {
}
void stub_glCopyPathNV() {
}
void stub_glInterpolatePathsNV() {
}
void stub_glTransformPathNV() {
}
void stub_glPathParameterivNV() {
}
void stub_glPathParameteriNV() {
}
void stub_glPathParameterfvNV() {
}
void stub_glPathParameterfNV() {
}
void stub_glPathDashArrayNV() {
}
void stub_glPathStencilFuncNV() {
}
void stub_glPathStencilDepthOffsetNV() {
}
void stub_glStencilFillPathNV() {
}
void stub_glStencilStrokePathNV() {
}
void stub_glStencilFillPathInstancedNV() {
}
void stub_glStencilStrokePathInstancedNV() {
}
void stub_glPathCoverDepthFuncNV() {
}
void stub_glCoverFillPathNV() {
}
void stub_glCoverStrokePathNV() {
}
void stub_glCoverFillPathInstancedNV() {
}
void stub_glCoverStrokePathInstancedNV() {
}
void stub_glGetPathParameterivNV() {
}
void stub_glGetPathParameterfvNV() {
}
void stub_glGetPathCommandsNV() {
}
void stub_glGetPathCoordsNV() {
}
void stub_glGetPathDashArrayNV() {
}
void stub_glGetPathMetricsNV() {
}
void stub_glGetPathMetricRangeNV() {
}
void stub_glGetPathSpacingNV() {
}
void stub_glIsPointInFillPathNV() {
}
void stub_glIsPointInStrokePathNV() {
}
void stub_glGetPathLengthNV() {
}
void stub_glPointAlongPathNV() {
}
void stub_glMatrixLoad3x2fNV() {
}
void stub_glMatrixLoad3x3fNV() {
}
void stub_glMatrixLoadTranspose3x3fNV() {
}
void stub_glMatrixMult3x2fNV() {
}
void stub_glMatrixMult3x3fNV() {
}
void stub_glMatrixMultTranspose3x3fNV() {
}
void stub_glStencilThenCoverFillPathNV() {
}
void stub_glStencilThenCoverStrokePathNV() {
}
void stub_glStencilThenCoverFillPathInstancedNV() {
}
void stub_glStencilThenCoverStrokePathInstancedNV() {
}
void stub_glPathGlyphIndexRangeNV() {
}
void stub_glPathGlyphIndexArrayNV() {
}
void stub_glPathMemoryGlyphIndexArrayNV() {
}
void stub_glProgramPathFragmentInputGenNV() {
}
void stub_glGetProgramResourcefvNV() {
}
void stub_glFramebufferSampleLocationsfvNV() {
}
void stub_glNamedFramebufferSampleLocationsfvNV() {
}
void stub_glResolveDepthValuesNV() {
}
void stub_glScissorExclusiveNV() {
}
void stub_glScissorExclusiveArrayvNV() {
}
void stub_glMakeBufferResidentNV() {
}
void stub_glMakeBufferNonResidentNV() {
}
void stub_glIsBufferResidentNV() {
}
void stub_glMakeNamedBufferResidentNV() {
}
void stub_glMakeNamedBufferNonResidentNV() {
}
void stub_glIsNamedBufferResidentNV() {
}
void stub_glGetBufferParameterui64vNV() {
}
void stub_glGetNamedBufferParameterui64vNV() {
}
void stub_glGetIntegerui64vNV() {
}
void stub_glUniformui64NV() {
}
void stub_glUniformui64vNV() {
}
void stub_glGetUniformui64vNV() {
}
void stub_glProgramUniformui64NV() {
}
void stub_glProgramUniformui64vNV() {
}
void stub_glBindShadingRateImageNV() {
}
void stub_glGetShadingRateImagePaletteNV() {
}
void stub_glGetShadingRateSampleLocationivNV() {
}
void stub_glShadingRateImageBarrierNV() {
}
void stub_glShadingRateImagePaletteNV() {
}
void stub_glShadingRateSampleOrderNV() {
}
void stub_glShadingRateSampleOrderCustomNV() {
}
void stub_glTextureBarrierNV() {
}
void stub_glVertexAttribL1i64NV() {
}
void stub_glVertexAttribL2i64NV() {
}
void stub_glVertexAttribL3i64NV() {
}
void stub_glVertexAttribL4i64NV() {
}
void stub_glVertexAttribL1i64vNV() {
}
void stub_glVertexAttribL2i64vNV() {
}
void stub_glVertexAttribL3i64vNV() {
}
void stub_glVertexAttribL4i64vNV() {
}
void stub_glVertexAttribL1ui64NV() {
}
void stub_glVertexAttribL2ui64NV() {
}
void stub_glVertexAttribL3ui64NV() {
}
void stub_glVertexAttribL4ui64NV() {
}
void stub_glVertexAttribL1ui64vNV() {
}
void stub_glVertexAttribL2ui64vNV() {
}
void stub_glVertexAttribL3ui64vNV() {
}
void stub_glVertexAttribL4ui64vNV() {
}
void stub_glGetVertexAttribLi64vNV() {
}
void stub_glGetVertexAttribLui64vNV() {
}
void stub_glVertexAttribLFormatNV() {
}
void stub_glBufferAddressRangeNV() {
}
void stub_glVertexFormatNV() {
}
void stub_glNormalFormatNV() {
}
void stub_glColorFormatNV() {
}
void stub_glIndexFormatNV() {
}
void stub_glTexCoordFormatNV() {
}
void stub_glEdgeFlagFormatNV() {
}
void stub_glSecondaryColorFormatNV() {
}
void stub_glFogCoordFormatNV() {
}
void stub_glVertexAttribFormatNV() {
}
void stub_glVertexAttribIFormatNV() {
}
void stub_glGetIntegerui64i_vNV() {
}
void stub_glViewportSwizzleNV() {
}
void stub_glFramebufferTextureMultiviewOVR() {
}
void stub_glNamedFramebufferTextureMultiviewOVR() {
}

//BUILD SUCCESSFUL (total time: 0 seconds)
__attribute__((used)) void stub_glBegin() {
}

__attribute__((used)) void stub_glEnd() {
}

__attribute__((used)) void stub_glVertexPointer() {
}

__attribute__((used)) void stub_glColorPointer() {
}

__attribute__((used)) void stub_glTexCoordPointer() {
}

__attribute__((used)) void stub_glNormalPointer() {
}

__attribute__((used)) void stub_glEnableClientState() {
}

__attribute__((used)) void stub_glDisableClientState() {
}

__attribute__((used)) void stub_glArrayElement() {
}

__attribute__((used)) void stub_glMatrixMode() {
}

__attribute__((used)) void stub_glLoadIdentity() {
}

__attribute__((used)) void stub_glLoadMatrixf() {
}

__attribute__((used)) void stub_glLoadMatrixd() {
}

__attribute__((used)) void stub_glMultMatrixf() {
}

__attribute__((used)) void stub_glMultMatrixd() {
}

__attribute__((used)) void stub_glPushMatrix() {
}

__attribute__((used)) void stub_glPopMatrix() {
}

__attribute__((used)) void stub_glOrtho() {
}

__attribute__((used)) void stub_glFrustum() {
}

__attribute__((used)) void stub_glTranslatef() {
}

__attribute__((used)) void stub_glTranslated() {
}

__attribute__((used)) void stub_glRotatef() {
}

__attribute__((used)) void stub_glScalef() {
}

__attribute__((used)) void stub_glShadeModel() {
}

__attribute__((used)) void stub_glPushAttrib() {
}

__attribute__((used)) void stub_glPopAttrib() {
}

__attribute__((used)) void stub_glColor3f() {
}

__attribute__((used)) void stub_glColor4f() {
}

__attribute__((used)) void stub_glColor4ub() {
}

__attribute__((used)) void stub_glColor3ub() {
}

__attribute__((used)) void stub_glColor4fv() {
}

__attribute__((used)) void stub_glColor3fv() {
}

__attribute__((used)) void stub_glTexCoord2f() {
}

__attribute__((used)) void stub_glTexCoord2d() {
}

__attribute__((used)) void stub_glTexCoord1f() {
}

__attribute__((used)) void stub_glTexCoord3f() {
}

__attribute__((used)) void stub_glTexCoord4f() {
}

__attribute__((used)) void stub_glTexCoord2fv() {
}

__attribute__((used)) void stub_glTexCoord4fv() {
}

__attribute__((used)) void stub_glVertex2f() {
}

__attribute__((used)) void stub_glVertex3f() {
}

__attribute__((used)) void stub_glVertex4f() {
}

__attribute__((used)) void stub_glVertex2d() {
}

__attribute__((used)) void stub_glVertex3d() {
}

__attribute__((used)) void stub_glVertex2i() {
}

__attribute__((used)) void stub_glVertex3i() {
}

__attribute__((used)) void stub_glVertex3iv() {
}

__attribute__((used)) void stub_glVertex3fv() {
}

__attribute__((used)) void stub_glVertex4fv() {
}

__attribute__((used)) void stub_glVertex2fv() {
}

__attribute__((used)) void stub_glLineStipple() {
}

__attribute__((used)) void stub_glLightfv() {
}

__attribute__((used)) void stub_glLightModelfv() {
}

__attribute__((used)) void stub_glMaterialfv() {
}

__attribute__((used)) void stub_glColorMaterial() {
}

__attribute__((used)) void stub_glFogf() {
}

__attribute__((used)) void stub_glFogfv() {
}

__attribute__((used)) void stub_glFogi() {
}

__attribute__((used)) void stub_glNormal3f() {
}

__attribute__((used)) void stub_glPointSizef() {
}

__attribute__((used)) void stub_glRasterPos2f() {
}

__attribute__((used)) void stub_glRasterPos3f() {
}

__attribute__((used)) void stub_glRectf() {
}

__attribute__((used)) void stub_glDrawPixels() {
}

__attribute__((used)) void stub_glAccum() {
}

__attribute__((used)) void stub_glClearAccum() {
}

__attribute__((used)) void stub_glGetLightfv() {
}

__attribute__((used)) void stub_glGetMaterialfv() {
}

__attribute__((used)) void stub_glColor4d() {
}

__attribute__((used)) void stub_glColor4s() {
}

__attribute__((used)) void stub_glTexEnvi() {
}

__attribute__((used)) void stub_glTexEnvf() {
}

__attribute__((used)) void stub_glTexEnvfv() {
}

__attribute__((used)) void stub_glTexGend() {
}

__attribute__((used)) void stub_glTexGenf() {
}

__attribute__((used)) void stub_glTexGeni() {
}

__attribute__((used)) void stub_glGetTexEnviv() {
}

__attribute__((used)) void stub_glClipPlane() {
}

__attribute__((used)) void stub_glGetClipPlane() {
}
__attribute__((used)) void stub_glAlphaFunc() {
}

__attribute__((used)) void stub_glAreTexturesResident() {
}

__attribute__((used)) void stub_glBitmap() {
}

__attribute__((used)) void stub_glCallList() {
}

__attribute__((used)) void stub_glCallLists() {
}

__attribute__((used)) void stub_glClearIndex() {
}

__attribute__((used)) void stub_glColor3b() {
}

__attribute__((used)) void stub_glColor3bv() {
}

__attribute__((used)) void stub_glColor3d() {
}

__attribute__((used)) void stub_glColor3dv() {
}

__attribute__((used)) void stub_glColor3i() {
}

__attribute__((used)) void stub_glColor3iv() {
}

__attribute__((used)) void stub_glColor3s() {
}

__attribute__((used)) void stub_glColor3sv() {
}

__attribute__((used)) void stub_glColor3ubv() {
}

__attribute__((used)) void stub_glColor3ui() {
}

__attribute__((used)) void stub_glColor3uiv() {
}

__attribute__((used)) void stub_glColor3us() {
}

__attribute__((used)) void stub_glColor3usv() {
}

__attribute__((used)) void stub_glColor4b() {
}

__attribute__((used)) void stub_glColor4bv() {
}

__attribute__((used)) void stub_glColor4dv() {
}

__attribute__((used)) void stub_glColor4i() {
}

__attribute__((used)) void stub_glColor4iv() {
}

__attribute__((used)) void stub_glColor4sv() {
}

__attribute__((used)) void stub_glColor4ubv() {
}

__attribute__((used)) void stub_glColor4ui() {
}

__attribute__((used)) void stub_glColor4uiv() {
}

__attribute__((used)) void stub_glColor4us() {
}

__attribute__((used)) void stub_glColor4usv() {
}

__attribute__((used)) void stub_glCopyPixels() {
}

__attribute__((used)) void stub_glDeleteLists() {
}

__attribute__((used)) void stub_glEdgeFlag() {
}

__attribute__((used)) void stub_glEdgeFlagPointer() {
}

__attribute__((used)) void stub_glEdgeFlagv() {
}

__attribute__((used)) void stub_glEndList() {
}

__attribute__((used)) void stub_glEvalCoord1d() {
}

__attribute__((used)) void stub_glEvalCoord1dv() {
}

__attribute__((used)) void stub_glEvalCoord1f() {
}

__attribute__((used)) void stub_glEvalCoord1fv() {
}

__attribute__((used)) void stub_glEvalCoord2d() {
}

__attribute__((used)) void stub_glEvalCoord2dv() {
}

__attribute__((used)) void stub_glEvalCoord2f() {
}

__attribute__((used)) void stub_glEvalCoord2fv() {
}

__attribute__((used)) void stub_glEvalMesh1() {
}

__attribute__((used)) void stub_glEvalMesh2() {
}

__attribute__((used)) void stub_glEvalPoint1() {
}

__attribute__((used)) void stub_glEvalPoint2() {
}

__attribute__((used)) void stub_glFeedbackBuffer() {
}

__attribute__((used)) void stub_glFogiv() {
}

__attribute__((used)) void stub_glGenLists() {
}

__attribute__((used)) void stub_glGetLightiv() {
}

__attribute__((used)) void stub_glGetMapdv() {
}

__attribute__((used)) void stub_glGetMapfv() {
}

__attribute__((used)) void stub_glGetMapiv() {
}

__attribute__((used)) void stub_glGetMaterialiv() {
}

__attribute__((used)) void stub_glGetPixelMapfv() {
}

__attribute__((used)) void stub_glGetPixelMapuiv() {
}

__attribute__((used)) void stub_glGetPixelMapusv() {
}

__attribute__((used)) void stub_glGetPolygonStipple() {
}

__attribute__((used)) void stub_glGetTexEnvfv() {
}

__attribute__((used)) void stub_glGetTexGendv() {
}

__attribute__((used)) void stub_glGetTexGenfv() {
}

__attribute__((used)) void stub_glGetTexGeniv() {
}

__attribute__((used)) void stub_glIndexd() {
}

__attribute__((used)) void stub_glIndexdv() {
}

__attribute__((used)) void stub_glIndexf() {
}

__attribute__((used)) void stub_glIndexfv() {
}

__attribute__((used)) void stub_glIndexi() {
}

__attribute__((used)) void stub_glIndexiv() {
}

__attribute__((used)) void stub_glIndexMask() {
}

__attribute__((used)) void stub_glIndexPointer() {
}

__attribute__((used)) void stub_glIndexs() {
}

__attribute__((used)) void stub_glIndexsv() {
}

__attribute__((used)) void stub_glIndexub() {
}

__attribute__((used)) void stub_glIndexubv() {
}

__attribute__((used)) void stub_glInitNames() {
}

__attribute__((used)) void stub_glInterleavedArrays() {
}

__attribute__((used)) void stub_glIsList() {
}

__attribute__((used)) void stub_glLightf() {
}

__attribute__((used)) void stub_glLighti() {
}

__attribute__((used)) void stub_glLightiv() {
}

__attribute__((used)) void stub_glLightModelf() {
}

__attribute__((used)) void stub_glLightModeli() {
}

__attribute__((used)) void stub_glLightModeliv() {
}

__attribute__((used)) void stub_glListBase() {
}

__attribute__((used)) void stub_glLoadName() {
}

__attribute__((used)) void stub_glMap1d() {
}

__attribute__((used)) void stub_glMap1f() {
}

__attribute__((used)) void stub_glMap2d() {
}

__attribute__((used)) void stub_glMap2f() {
}

__attribute__((used)) void stub_glMapGrid1d() {
}

__attribute__((used)) void stub_glMapGrid1f() {
}

__attribute__((used)) void stub_glMapGrid2d() {
}

__attribute__((used)) void stub_glMapGrid2f() {
}

__attribute__((used)) void stub_glMaterialf() {
}

__attribute__((used)) void stub_glMateriali() {
}

__attribute__((used)) void stub_glMaterialiv() {
}

__attribute__((used)) void stub_glNewList() {
}

__attribute__((used)) void stub_glNormal3b() {
}

__attribute__((used)) void stub_glNormal3bv() {
}

__attribute__((used)) void stub_glNormal3d() {
}

__attribute__((used)) void stub_glNormal3dv() {
}

__attribute__((used)) void stub_glNormal3fv() {
}

__attribute__((used)) void stub_glNormal3i() {
}

__attribute__((used)) void stub_glNormal3iv() {
}

__attribute__((used)) void stub_glNormal3s() {
}

__attribute__((used)) void stub_glNormal3sv() {
}

__attribute__((used)) void stub_glPassThrough() {
}

__attribute__((used)) void stub_glPixelMapfv() {
}

__attribute__((used)) void stub_glPixelMapuiv() {
}

__attribute__((used)) void stub_glPixelMapusv() {
}

__attribute__((used)) void stub_glPixelTransferf() {
}

__attribute__((used)) void stub_glPixelTransferi() {
}

__attribute__((used)) void stub_glPixelZoom() {
}

__attribute__((used)) void stub_glPolygonStipple() {
}

__attribute__((used)) void stub_glPopClientAttrib() {
}

__attribute__((used)) void stub_glPopName() {
}

__attribute__((used)) void stub_glPrioritizeTextures() {
}

__attribute__((used)) void stub_glPushClientAttrib() {
}

__attribute__((used)) void stub_glPushName() {
}

__attribute__((used)) void stub_glRasterPos2d() {
}

__attribute__((used)) void stub_glRasterPos2dv() {
}

__attribute__((used)) void stub_glRasterPos2fv() {
}

__attribute__((used)) void stub_glRasterPos2i() {
}

__attribute__((used)) void stub_glRasterPos2iv() {
}

__attribute__((used)) void stub_glRasterPos2s() {
}

__attribute__((used)) void stub_glRasterPos2sv() {
}

__attribute__((used)) void stub_glRasterPos3d() {
}

__attribute__((used)) void stub_glRasterPos3dv() {
}

__attribute__((used)) void stub_glRasterPos3fv() {
}

__attribute__((used)) void stub_glRasterPos3i() {
}

__attribute__((used)) void stub_glRasterPos3iv() {
}

__attribute__((used)) void stub_glRasterPos3s() {
}

__attribute__((used)) void stub_glRasterPos3sv() {
}

__attribute__((used)) void stub_glRasterPos4d() {
}

__attribute__((used)) void stub_glRasterPos4dv() {
}

__attribute__((used)) void stub_glRasterPos4f() {
}

__attribute__((used)) void stub_glRasterPos4fv() {
}

__attribute__((used)) void stub_glRasterPos4i() {
}

__attribute__((used)) void stub_glRasterPos4iv() {
}

__attribute__((used)) void stub_glRasterPos4s() {
}

__attribute__((used)) void stub_glRasterPos4sv() {
}

__attribute__((used)) void stub_glRectd() {
}

__attribute__((used)) void stub_glRectdv() {
}

__attribute__((used)) void stub_glRectfv() {
}

__attribute__((used)) void stub_glRecti() {
}

__attribute__((used)) void stub_glRectiv() {
}

__attribute__((used)) void stub_glRects() {
}

__attribute__((used)) void stub_glRectsv() {
}

__attribute__((used)) void stub_glRenderMode() {
}

__attribute__((used)) void stub_glRotated() {
}

__attribute__((used)) void stub_glScaled() {
}

__attribute__((used)) void stub_glSelectBuffer() {
}

__attribute__((used)) void stub_glTexCoord1d() {
}

__attribute__((used)) void stub_glTexCoord1dv() {
}

__attribute__((used)) void stub_glTexCoord1fv() {
}

__attribute__((used)) void stub_glTexCoord1i() {
}

__attribute__((used)) void stub_glTexCoord1iv() {
}

__attribute__((used)) void stub_glTexCoord1s() {
}

__attribute__((used)) void stub_glTexCoord1sv() {
}

__attribute__((used)) void stub_glTexCoord2dv() {
}

__attribute__((used)) void stub_glTexCoord2i() {
}

__attribute__((used)) void stub_glTexCoord2iv() {
}

__attribute__((used)) void stub_glTexCoord2s() {
}

__attribute__((used)) void stub_glTexCoord2sv() {
}

__attribute__((used)) void stub_glTexCoord3d() {
}

__attribute__((used)) void stub_glTexCoord3dv() {
}

__attribute__((used)) void stub_glTexCoord3fv() {
}

__attribute__((used)) void stub_glTexCoord3i() {
}

__attribute__((used)) void stub_glTexCoord3iv() {
}

__attribute__((used)) void stub_glTexCoord3s() {
}

__attribute__((used)) void stub_glTexCoord3sv() {
}

__attribute__((used)) void stub_glTexCoord4d() {
}

__attribute__((used)) void stub_glTexCoord4dv() {
}

__attribute__((used)) void stub_glTexCoord4i() {
}

__attribute__((used)) void stub_glTexCoord4iv() {
}

__attribute__((used)) void stub_glTexCoord4s() {
}

__attribute__((used)) void stub_glTexCoord4sv() {
}

__attribute__((used)) void stub_glTexEnviv() {
}

__attribute__((used)) void stub_glTexGendv() {
}

__attribute__((used)) void stub_glTexGenfv() {
}

__attribute__((used)) void stub_glTexGeniv() {
}

__attribute__((used)) void stub_glVertex2dv() {
}

__attribute__((used)) void stub_glVertex2iv() {
}

__attribute__((used)) void stub_glVertex2s() {
}

__attribute__((used)) void stub_glVertex2sv() {
}

__attribute__((used)) void stub_glVertex3dv() {
}

__attribute__((used)) void stub_glVertex3s() {
}

__attribute__((used)) void stub_glVertex3sv() {
}

__attribute__((used)) void stub_glVertex4d() {
}

__attribute__((used)) void stub_glVertex4dv() {
}

__attribute__((used)) void stub_glVertex4i() {
}

__attribute__((used)) void stub_glVertex4iv() {
}

__attribute__((used)) void stub_glVertex4s() {
}

__attribute__((used)) void stub_glVertex4sv() {
}
