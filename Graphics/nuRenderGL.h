/*!
 * \file nuRenderGL.h
 * \brief Renderer.
 * \author Nus
 * \date 2011/10/30 17:40
 */

#ifndef __NURENDERGL_H__
#define __NURENDERGL_H__

#include "nuGResManager.h"
#include "nuVertexBuffer.h"
#include "nuElementBuffer.h"

/*!
 * \class nuRenderGL
 * \brief OpenGL renderer.
 */
class nuRenderGL : public nuObject
{
  DECLARE_TYPE_INFO;

  enum RENDERER_PHASE {
    INIT_PHASE = 0,
    SETUP_PHASE,
    EXECUTE_PHASE,
  };

  nuGResManager mResourceManager;
  i64 mFrameID;
  nuConditionLock mLock;

public:
  nuRenderGL();
  ~nuRenderGL();

  i64 updateGraphicResources(void);
  i32 render(void);

  void acquire(void) {
    mLock.lockWhenCondition(INIT_PHASE);
  }

  void release(void) {
    mLock.unlock();
  }

  void synchronize(void);
  bool isCommandSubmitted(void);

  void initialize(void);
  void terminate(void);

  nude::VertexBuffer createVertexBuffer(size_t size, nuGResource::RESOURCE_USAGE usage) {
    return nude::VertexBuffer(mResourceManager.createVertexBuffer(size, usage));
  }

  nude::ElementBuffer createElementBuffer(nuElementBuffer::ELEMENT_TYPE type,
                                          ui32 size,
                                          nuGResource::RESOURCE_USAGE usage) {
    return nude::ElementBuffer(mResourceManager.createElementBuffer(type, size, usage));
  }

};

#endif
