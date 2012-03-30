/*!
 * \file nuRenderGL.h
 * \brief Renderer.
 * \author Nus
 * \date 2011/10/30 17:40
 */

#ifndef __NURENDERGL_H__
#define __NURENDERGL_H__

#include "nuGResManager.h"

/*!
 * \class nuRenderGL
 * \brief OpenGL renderer.
 */
class nuRenderGL : public nuObject
{
  DECLARE_TYPE_INFO;

public:
  nuRenderGL();
  ~nuRenderGL();

  void updateResources(void);
  i32 render(void);

  void initTest(void);
  void termTest(void);

  nuGResHandle createVertexBuffer(size_t size, nuGResource::RESOURCE_USAGE usage) {
    return nuGResHandle(mResourceManager.createVertexBuffer(size, usage));
  }

  nuGResHandle createIndexBuffer(size_t size, nuGResource::RESOURCE_USAGE usage) {
    return nuGResHandle(mResourceManager.createElementBuffer(size, usage));
  }

private:
  nuGResManager mResourceManager;

};

#endif
