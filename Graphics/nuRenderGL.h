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
#include "nuShaderProgram.h"
#include "nuUniformBuffer.h"
#include "nuOpenGLCaps.h"

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
    nuShaderProgram* p_shader_program;
    nuVertexArray* p_vertex_array;
    nuVertexBuffer* p_vertex_buffer;
    nuElementBuffer* p_element_buffer;
    const nuGContext::Viewport* p_viewport;
    nuGContext::FragmentOps fragment_ops;
  };

  nuGResManager mResourceManager;
  i64 mFrameID;
  nuConditionLock mLock;
  nuGContext::TagList* mpNextTagList;
  nuGContext::TagList* mpCurrentTagList;
  RenderContext mRenderContext;

  GLint mVertexLoc;
  GLint mColorLoc;

  nuOpenGLCaps mCapabilities;
  nuRect mViewport;

  void executeClear(RenderContext& context, void* clear_cmd);
  void executeDrawElements(RenderContext& context, void* draw_cmd);

  void setShaderProgram(RenderContext& context, nuGContext::ProgramObject& program);
  void setUniformValue(RenderContext& context, nuGContext::ProgramObject& program);
  void setUniformBlock(RenderContext& context, nuGContext::ProgramObject& program);
  void setViewport(RenderContext& context, const nuGContext::Viewport* p_viewport);
  void setFragmentOps(RenderContext& context, nuGContext::FragmentOps& fragment_ops);

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

  void initialize(nude::ShaderList& shader_list);
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

  nude::ShaderProgram createShaderProgram(nude::ProgramList program) {
    return nude::ShaderProgram(mResourceManager.createShaderProgram(program));
  }

  nude::UniformBuffer createUniformBuffer(nude::ProgramList program, ui32 ubo_id) {
    return nude::UniformBuffer(mResourceManager.createUniformBuffer(program, ubo_id));
  }

  const nuOpenGLCaps& getCapabilities(void) const {
    return mCapabilities;
  }

  const nuRect& getViewport(void) const {
    return mViewport;
  }

  void setViewport(const nuRect& viewport) {
    mViewport = viewport;
  }

};

#endif
