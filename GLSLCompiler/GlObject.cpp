/*!
 * \file GlObject.cpp
 * \brief GL object.
 * \author Nus
 * \date 2012/01/01 20:55
 */

#include "GlObject.h"

GlObject::GlObject()
    : mContext(nullptr)
{
  const int attr[] = {
    kCGLPFADoubleBuffer,
    kCGLPFAAccelerated,
    kCGLPFAColorSize, 32,
    kCGLPFAAlphaSize, 8,
    kCGLPFADepthSize, 24,
    kCGLPFAStencilSize, 8,
    kCGLPFAOpenGLProfile, kCGLOGLPVersion_3_2_Core,
    0
  };
  CGLPixelFormatObj pix;
  GLint npix;

  CGLChoosePixelFormat(reinterpret_cast< const CGLPixelFormatAttribute* >(attr),
                       &pix,
                       &npix);
  if(pix) {
    CGLCreateContext(pix, nullptr, &mContext);
    CGLReleasePixelFormat(pix);
    if(mContext) {
      CGLError err = CGLSetCurrentContext(mContext);
      if(err != kCGLNoError)
        fprintf(stderr, "Error setting GL context.\n");
      CGLLockContext(mContext);
    }
  }
}

GlObject::~GlObject()
{
  if(mContext) {
    CGLFlushDrawable(mContext);
    CGLUnlockContext(mContext);
    CGLReleaseContext(mContext);
  }
}

GLuint GlObject::compileShader(GLenum shader_type, void* p_buffer, size_t size)
{
  GLint shd_len = static_cast< GLint >(size);
  GLuint shd_id = glCreateShader(shader_type);
  GLint status;

  glShaderSource(shd_id, 1, (const GLchar**) &p_buffer, &shd_len);
  glCompileShader(shd_id);

  glGetShaderiv(shd_id, GL_COMPILE_STATUS, &status);
  if(status == GL_FALSE) {
    GLint log_len;
    glGetShaderiv(shd_id, GL_INFO_LOG_LENGTH, &log_len);
    if(log_len > 0) {
      GLchar* p_log = static_cast< GLchar* >(malloc(log_len));
      GLsizei len;
      glGetShaderInfoLog(shd_id, log_len, &len, p_log);
      fprintf(stderr, "%s\n", p_log);
      free(p_log);
    }
    if(shader_type == GL_VERTEX_SHADER)
      fprintf(stderr, "Error compiling vertex shader.\n");
    else if(shader_type == GL_FRAGMENT_SHADER)
      fprintf(stderr, "Error compiling fragment shader.\n");
    else if(shader_type == GL_GEOMETRY_SHADER)
      fprintf(stderr, "Error compiling geometry shader.\n");
    else
      fprintf(stderr, "Error compiling unknown shader.\n");
    glDeleteShader(shd_id);
    shd_id = 0;
  }

  return shd_id;
}

GLuint GlObject::linkProgram(GLuint vs_id, GLuint fs_id)
{
  GLuint prog_id = glCreateProgram();
  GLint status;

  glAttachShader(prog_id, vs_id);
  glAttachShader(prog_id, fs_id);
  glLinkProgram(prog_id);

  glGetProgramiv(prog_id, GL_LINK_STATUS, &status);
  if(status == GL_FALSE) {
    GLint log_len;
    glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &log_len);
    if(log_len > 0) {
      GLchar* p_log = static_cast< GLchar* >(malloc(log_len));
      GLsizei len;
      glGetProgramInfoLog(prog_id, log_len, &len, p_log);
      fprintf(stderr, "%s\n", p_log);
      free(p_log);
    }
    fprintf(stderr, "Error linking program.\n");
    glDeleteProgram(prog_id);
    return 0;
  }

  glValidateProgram(prog_id);
  glGetProgramiv(prog_id, GL_VALIDATE_STATUS, &status);
  if(status == 0) {
    GLint log_len;
    glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &log_len);
    if(log_len > 0) {
      GLchar* p_log = static_cast< GLchar* >(malloc(log_len));
      GLsizei len;
      glGetProgramInfoLog(prog_id, log_len, &len, p_log);
      fprintf(stderr, "%s\n", p_log);
      free(p_log);
    }
    fprintf(stderr, "Error validating program.\n");
    glDeleteProgram(prog_id);
    return 0;
  }

  return prog_id;
}

void GlObject::deleteShader(GLuint shd_id)
{
  if(shd_id)
    glDeleteShader(shd_id);
}

void GlObject::deleteProgram(GLuint prog_id)
{
  if(prog_id)
    glDeleteProgram(prog_id);
}
