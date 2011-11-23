/*!
 * \file nuRenderGL.cpp
 * \brief Renderer.
 * \author Nus
 * \date 2011/10/30 17:40
 */

#include "nuRenderGL.h"

IMPLEMENT_TYPE_INFO(nuRenderGL, nuObject);

nuRenderGL::nuRenderGL()
{

}

nuRenderGL::~nuRenderGL()
{

}

i32 nuRenderGL::render(void)
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  return 0;
}
