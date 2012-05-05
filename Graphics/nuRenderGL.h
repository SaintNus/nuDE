/*!
 * \file nuRenderGL.h
 * \brief Renderer.
 * \author Nus
 * \date 2011/10/30 17:40
 */

#ifndef __NURENDERGL_H__
#define __NURENDERGL_H__

#include "nuGContext.h"
#include "nuGResManager.h"
#include "nuVertexArray.h"
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

  struct RenderContext {
    nuColor clear_color;
    f32 depth_value;
    nuVertexArray* p_vertex_array;
    nuVertexBuffer* p_vertex_buffer;
    nuElementBuffer* p_element_buffer;
  };

  nuGResManager mResourceManager;
  i64 mFrameID;
  nuConditionLock mLock;
  nuGContext::TagList* mpNextTagList;
  nuGContext::TagList* mpCurrentTagList;
  RenderContext mRenderContext;

  GLint mVertexLoc;
  GLint mColorLoc;

  void executeClear(RenderContext& context, void* clear_cmd);
  void executeDrawElements(RenderContext& context, void* draw_cmd);

public:
  nuRenderGL();
  ~nuRenderGL();

  void setNextTagList(nuGContext::TagList& next) {
    mpNextTagList = &next;
  }

  i64 updateGraphicResources(void);
  bool render(void);

  void acquire(void) {
    mLock.lockWhenCondition(INIT_PHASE);
  }

  void release(void) {
    mLock.unlock();
  }

  i64 synchronize(void);
  bool isCommandSubmitted(void);

  void initialize(void);
  void terminate(void);

  nude::VertexArray createVertexArray() {
    return nude::VertexArray(mResourceManager.createVertexArray());
  }

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
