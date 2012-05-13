/*!
 * \file nuGDebug.h
 * \brief GL debug.
 * \author Nus
 * \date 2012/05/13 11:44
 */

#ifndef __NUGDEBUG_H__
#define __NUGDEBUG_H__

#if DEBUG

#define CHECK_GL_ERROR(_func) \
  { \
    _func; \
    GLenum __internal_error__ = glGetError(); \
    if(__internal_error__ != GL_NO_ERROR) { \
      NU_TRACE("GL error!\n"); \
      NU_TRACE("  Function: %s\n", #_func); \
      NU_TRACE("  Result: %s\n", nude::GetErrorString(__internal_error__)); \
      NU_ASSERT_C(__internal_error__ == GL_NO_ERROR); \
    } \
  }

#else
  
#define CHECK_GL_ERROR(_func) _func

#endif

namespace nude
{

  ccstr GetErrorString(GLenum error);

}

#endif
