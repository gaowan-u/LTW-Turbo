/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

/**
 * 文件功能：LTW 覆写函数注册表。
 *
 * 每个 GLESOVERRIDE(name) 声明一个 LTW 实现的桌面 GL 入口；
 * proc.c 的 eglGetProcAddress 按此表把游戏请求分发到 LTW 实现。
 */
// 用于设置深度缓冲区的清除值
void glClearDepth(double depth);
void* glMapBuffer(GLenum target, GLenum access);
void glGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params);
void glDebugMessageControl( 	GLenum source,
                               GLenum type,
                               GLenum severity,
                               GLsizei count,
                               const GLuint *ids,
                               GLboolean enabled);
// 用于一次性绘制多个元素的图形，允许指定每个绘制调用的基顶点
void glMultiDrawElementsBaseVertex( 	GLenum mode,
                                       const GLsizei *count,
                                       GLenum type,
                                       const void * const *indices,
                                       GLsizei drawcount,
                                       const GLint *basevertex);
void glBindFragDataLocation(GLuint program,
                            GLuint colorNumber,
                            const char * name);
// 从当前纹理对象中获取像素数据
void glGetTexImage( 	GLenum target,
                       GLint level,
                       GLenum format,
                       GLenum type,
                       void * pixels);

void glGetQueryObjectiv( 	GLuint id,
                            GLenum pname,
                            GLint * params);
// 设置当前视口的深度范围
void glDepthRange(GLdouble nearVal,
                  GLdouble farVal);
// MathCode: 桌面纹理环境（GL_TEXTURE_ENV / GL_COMBINE / GL_INTERPOLATE /
// GL_TEXTURE_ENV_COLOR）：GLES 无对应物，固定管线记录状态供默认 shader
// 模拟（MC 1.12 生物受伤红闪、亮度调整）。
void glTexEnv(GLenum target, GLenum pname, const GLfloat *params);
void glTexEnvf(GLenum target, GLenum pname, GLfloat param);
void glTexEnvi(GLenum target, GLenum pname, GLint param);
void glTexEnvfv(GLenum target, GLenum pname, const GLfloat *params);
void glTexEnviv(GLenum target, GLenum pname, const GLint *params);

// GL_ARB_shader_objects / GL_ARB_vertex_shader / GL_ARB_fragment_shader
// entry points are declared in GL/glext.h (included via proc.c). LWJGL2-era
// games (Minecraft <=1.16) probe for these extensions and then route GLSL
// through the ARB-named APIs, which the GLES driver does not export.
// Implemented in shader_wrapper.c on top of the GL20 wraps (mirrors gl4es).

GLESOVERRIDE(glClearDepth)
GLESOVERRIDE(glMapBuffer)
GLESOVERRIDE(glGetTexLevelParameteriv)
GLESOVERRIDE(glGetTexLevelParameterfv)
GLESOVERRIDE(glCreateShader)
GLESOVERRIDE(glDeleteShader)
GLESOVERRIDE(glCreateProgram)
GLESOVERRIDE(glDeleteProgram)
GLESOVERRIDE(glLinkProgram)
GLESOVERRIDE(glAttachShader)
GLESOVERRIDE(glGetShaderiv)
GLESOVERRIDE(glShaderSource)
GLESOVERRIDE(glTexImage2D)
GLESOVERRIDE(glDebugMessageControl)
GLESOVERRIDE(glGetString)
GLESOVERRIDE(glEnable)
GLESOVERRIDE(glDisable)
GLESOVERRIDE(glBindTexture)
GLESOVERRIDE(glActiveTexture)
GLESOVERRIDE(glActiveTextureARB)
GLESOVERRIDE(glClientActiveTexture)
GLESOVERRIDE(glClientActiveTextureARB)
GLESOVERRIDE(glPixelStorei)
GLESOVERRIDE(glGenerateMipmap)
GLESOVERRIDE(glViewport)
GLESOVERRIDE(glBlendFunc)
GLESOVERRIDE(glBlendFuncSeparate)
GLESOVERRIDE(glDepthFunc)
GLESOVERRIDE(glDepthMask)
GLESOVERRIDE(glColorMask)
GLESOVERRIDE(glCullFace)
GLESOVERRIDE(glStencilFunc)
GLESOVERRIDE(glStencilMask)
GLESOVERRIDE(glLineWidth)
GLESOVERRIDE(glHint)
GLESOVERRIDE(glBufferData)
GLESOVERRIDE(glBufferSubData)
GLESOVERRIDE(glDeleteBuffers)
GLESOVERRIDE(glCompileShader)
GLESOVERRIDE(glUniform1i)
GLESOVERRIDE(glUniform4f)
GLESOVERRIDE(glUniformMatrix4fv)
GLESOVERRIDE(glStencilOp)
GLESOVERRIDE(glStencilFuncSeparate)
GLESOVERRIDE(glStencilOpSeparate)
GLESOVERRIDE(glStencilMaskSeparate)
GLESOVERRIDE(glPolygonOffset)
GLESOVERRIDE(glScissor)
GLESOVERRIDE(glClearDepthf)
GLESOVERRIDE(glClearStencil)
GLESOVERRIDE(glDrawArraysInstanced)
GLESOVERRIDE(glDrawElementsInstanced)
GLESOVERRIDE(glVertexAttribDivisor)
GLESOVERRIDE(glEnableVertexAttribArray)
GLESOVERRIDE(glDisableVertexAttribArray)
GLESOVERRIDE(glUniform2f)
GLESOVERRIDE(glUniform3f)
GLESOVERRIDE(glUniform2fv)
GLESOVERRIDE(glUniform3fv)
GLESOVERRIDE(glGetUniformLocation)
GLESOVERRIDE(glTexImage3D)
GLESOVERRIDE(glTexSubImage3D)
GLESOVERRIDE(glCopyTexImage2D)
GLESOVERRIDE(glBlitFramebuffer)
GLESOVERRIDE(glDrawRangeElements)
GLESOVERRIDE(glDrawArrays)
GLESOVERRIDE(glDrawElements)
// MathCode: [DBG-mctx] 诊断构建临时覆写，拦截应用侧 VAO 生命周期，
// 定位 fp_vao=1 在渲染上下文无效的根因；定位后随探针一起移除。
GLESOVERRIDE(glGenVertexArrays)
GLESOVERRIDE(glDeleteVertexArrays)
GLESOVERRIDE(glBindVertexArray)
GLESOVERRIDE(glSampleCoverage)
GLESOVERRIDE(glFlush)
GLESOVERRIDE(glFinish)
GLESOVERRIDE(glGetTexParameteriv)
GLESOVERRIDE(glGetTexParameterfv)
GLESOVERRIDE(glBegin)
GLESOVERRIDE(glEnd)
GLESOVERRIDE(glMatrixMode)
GLESOVERRIDE(glLoadIdentity)
GLESOVERRIDE(glLoadMatrixf)
GLESOVERRIDE(glLoadMatrixd)
GLESOVERRIDE(glMultMatrixf)
GLESOVERRIDE(glMultMatrixd)
GLESOVERRIDE(glPushMatrix)
GLESOVERRIDE(glPopMatrix)
GLESOVERRIDE(glOrtho)
GLESOVERRIDE(glFrustum)
GLESOVERRIDE(glTranslatef)
GLESOVERRIDE(glTranslated)
GLESOVERRIDE(glScalef)
GLESOVERRIDE(glScaled)
GLESOVERRIDE(glRotatef)
GLESOVERRIDE(glRotated)
GLESOVERRIDE(glVertex3fv)
GLESOVERRIDE(glVertex3f)
GLESOVERRIDE(glVertex3d)
GLESOVERRIDE(glVertex2f)
GLESOVERRIDE(glVertex2d)
GLESOVERRIDE(glVertex4f)
GLESOVERRIDE(glVertex3iv)
GLESOVERRIDE(glVertex3sv)
GLESOVERRIDE(glVertex4fv)
GLESOVERRIDE(glVertex2fv)
GLESOVERRIDE(glColor4f)
GLESOVERRIDE(glColor3f)
GLESOVERRIDE(glColor4ub)
GLESOVERRIDE(glColor3ub)
GLESOVERRIDE(glColor4fv)
GLESOVERRIDE(glColor3fv)
GLESOVERRIDE(glColor4ubv)
GLESOVERRIDE(glTexCoord2f)
GLESOVERRIDE(glTexCoord2fv)
GLESOVERRIDE(glTexCoord1f)
GLESOVERRIDE(glTexCoord3f)
GLESOVERRIDE(glTexCoord4f)
GLESOVERRIDE(glTexCoord2d)
GLESOVERRIDE(glMultiTexCoord2f)
GLESOVERRIDE(glMultiTexCoord2fv)
GLESOVERRIDE(glMultiTexCoord3f)
GLESOVERRIDE(glMultiTexCoord4f)
GLESOVERRIDE(glMultiTexCoord2fARB)
GLESOVERRIDE(glMultiTexCoord2fvARB)
GLESOVERRIDE(glMultiTexCoord3fARB)
GLESOVERRIDE(glMultiTexCoord4fARB)
GLESOVERRIDE(glNormal3f)
GLESOVERRIDE(glNormal3fv)
GLESOVERRIDE(glVertexPointer)
GLESOVERRIDE(glTexCoordPointer)
GLESOVERRIDE(glColorPointer)
GLESOVERRIDE(glNormalPointer)
GLESOVERRIDE(glEnableClientState)
GLESOVERRIDE(glDisableClientState)
GLESOVERRIDE(glArrayElement)
GLESOVERRIDE(glShadeModel)
GLESOVERRIDE(glPushAttrib)
GLESOVERRIDE(glPopAttrib)
GLESOVERRIDE(glGetFloatv)
GLESOVERRIDE(glGetDoublev)
GLESOVERRIDE(glGetBooleanv)
GLESOVERRIDE(glAlphaFunc)
GLESOVERRIDE(glGenLists)
GLESOVERRIDE(glNewList)
GLESOVERRIDE(glEndList)
GLESOVERRIDE(glCallList)
GLESOVERRIDE(glCallLists)
GLESOVERRIDE(glDeleteLists)
GLESOVERRIDE(glIsList)
GLESOVERRIDE(glListBase)
GLESOVERRIDE(glMultiDrawArrays)
GLESOVERRIDE(glMultiDrawElements)
GLESOVERRIDE(glMultiDrawElementsBaseVertex)
GLESOVERRIDE(glDrawElementsBaseVertex)
GLESOVERRIDE(glBindBufferBase)
GLESOVERRIDE(glBindBufferRange)
GLESOVERRIDE(glBindBuffer)
GLESOVERRIDE(glUseProgram)
GLESOVERRIDE(glGetIntegerv)
GLESOVERRIDE(glBindFramebuffer)
GLESOVERRIDE(glGenFramebuffers)
GLESOVERRIDE(glDeleteFramebuffers)
GLESOVERRIDE(glFramebufferTexture2D)
GLESOVERRIDE(glFramebufferTextureLayer)
GLESOVERRIDE(glFramebufferRenderbuffer)
GLESOVERRIDE(glGetFramebufferAttachmentParameteriv)
GLESOVERRIDE(glDrawBuffers)
GLESOVERRIDE(glDrawBuffer)
GLESOVERRIDE(glClearBufferiv)
GLESOVERRIDE(glClearBufferuiv)
GLESOVERRIDE(glClearBufferfv)
GLESOVERRIDE(glCheckFramebufferStatus)
GLESOVERRIDE(glReadPixels)
GLESOVERRIDE(glTexSubImage2D)
GLESOVERRIDE(glCopyTexSubImage2D)
GLESOVERRIDE(glTexParameteri)
GLESOVERRIDE(glBindFragDataLocation)
GLESOVERRIDE(glGetTexImage)
GLESOVERRIDE(glGetQueryObjectiv)
GLESOVERRIDE(glDepthRange)
// MathCode: 纹理环境入口（生物受伤红闪）
GLESOVERRIDE(glTexEnv)
GLESOVERRIDE(glTexEnvf)
GLESOVERRIDE(glTexEnvi)
GLESOVERRIDE(glTexEnvfv)
GLESOVERRIDE(glTexEnviv)
GLESOVERRIDE(glVertexAttrib1d)
GLESOVERRIDE(glVertexAttrib1dv)
GLESOVERRIDE(glVertexAttrib1s)
GLESOVERRIDE(glVertexAttrib1sv)
GLESOVERRIDE(glVertexAttrib2d)
GLESOVERRIDE(glVertexAttrib2dv)
GLESOVERRIDE(glVertexAttrib2s)
GLESOVERRIDE(glVertexAttrib2sv)
GLESOVERRIDE(glVertexAttrib3d)
GLESOVERRIDE(glVertexAttrib3dv)
GLESOVERRIDE(glVertexAttrib3s)
GLESOVERRIDE(glVertexAttrib3sv)
GLESOVERRIDE(glVertexAttrib4d)
GLESOVERRIDE(glVertexAttrib4dv)
GLESOVERRIDE(glVertexAttrib4s)
GLESOVERRIDE(glVertexAttrib4sv)
GLESOVERRIDE(glVertexAttrib4Nbv)
GLESOVERRIDE(glVertexAttrib4Niv)
GLESOVERRIDE(glVertexAttrib4Nsv)
GLESOVERRIDE(glVertexAttrib4Nub)
GLESOVERRIDE(glVertexAttrib4Nubv)
GLESOVERRIDE(glVertexAttrib4Nuiv)
GLESOVERRIDE(glVertexAttrib4Nusv)
GLESOVERRIDE(glVertexAttribI1i)
GLESOVERRIDE(glVertexAttribI1iv)
GLESOVERRIDE(glVertexAttribI1ui)
GLESOVERRIDE(glVertexAttribI1uiv)
GLESOVERRIDE(glVertexAttribI2i)
GLESOVERRIDE(glVertexAttribI2iv)
GLESOVERRIDE(glVertexAttribI2ui)
GLESOVERRIDE(glVertexAttribI2uiv)
GLESOVERRIDE(glVertexAttribI3i)
GLESOVERRIDE(glVertexAttribI3iv)
GLESOVERRIDE(glVertexAttribI3ui)
GLESOVERRIDE(glVertexAttribI3uiv)
GLESOVERRIDE(glVertexAttribI4bv)
GLESOVERRIDE(glVertexAttribI4ubv)
GLESOVERRIDE(glVertexAttribI4sv)
GLESOVERRIDE(glVertexAttribI4usv)
GLESOVERRIDE(glVertexAttribPointer)
GLESOVERRIDE(glBufferStorage)
GLESOVERRIDE(glGetStringi)
GLESOVERRIDE(glTexParameterf)
GLESOVERRIDE(glTexParameteri)
GLESOVERRIDE(glTexParameterfv)
GLESOVERRIDE(glTexParameteriv)
GLESOVERRIDE(glTexParameterIiv)
GLESOVERRIDE(glTexParameterIuiv)
GLESOVERRIDE(glRenderbufferStorage)
GLESOVERRIDE(glGetError)
GLESOVERRIDE(glTexBuffer)
GLESOVERRIDE(glTexBufferRange)
GLESOVERRIDE(glMapBufferRange)
GLESOVERRIDE(glFlushMappedBufferRange)
// GL_ARB_shader_objects / vertex_shader / fragment_shader entry points.
// LWJGL2-era games route GLSL through these when the ARB shader extensions
// are advertised; implemented in shader_wrapper.c on top of the GL20 wraps.
GLESOVERRIDE(glCreateShaderObjectARB)
GLESOVERRIDE(glCreateProgramObjectARB)
GLESOVERRIDE(glShaderSourceARB)
GLESOVERRIDE(glCompileShaderARB)
GLESOVERRIDE(glAttachObjectARB)
GLESOVERRIDE(glDetachObjectARB)
GLESOVERRIDE(glLinkProgramARB)
GLESOVERRIDE(glUseProgramObjectARB)
GLESOVERRIDE(glValidateProgramARB)
GLESOVERRIDE(glDeleteObjectARB)
GLESOVERRIDE(glGetObjectParameterivARB)
GLESOVERRIDE(glGetObjectParameterfvARB)
GLESOVERRIDE(glGetInfoLogARB)
GLESOVERRIDE(glGetAttachedObjectsARB)
GLESOVERRIDE(glGetUniformLocationARB)
GLESOVERRIDE(glGetAttribLocationARB)
GLESOVERRIDE(glBindAttribLocationARB)
