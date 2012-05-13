/*!
 * \file nuGDebug.cpp
 * \brief GL debug.
 * \author Nus
 * \date 2012/05/13 11:44
 */

#include "nuGDebug.h"

#define ERROR_STRING(_error, _gl_error) \
  if(_error == _gl_error) return #_gl_error

ccstr nude::GetErrorString(GLenum error)
{
  ERROR_STRING(error, GL_NO_ERROR);
  ERROR_STRING(error, GL_INVALID_ENUM);
  ERROR_STRING(error, GL_INVALID_VALUE);
  ERROR_STRING(error, GL_INVALID_OPERATION);
  ERROR_STRING(error, GL_INVALID_FRAMEBUFFER_OPERATION);
  ERROR_STRING(error, GL_OUT_OF_MEMORY);
  return "Unknown GL error";
}
