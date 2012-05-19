/*!
 * \file nuGSetupContext.cpp
 * \brief Graphic setup context.
 * \author Nus
 * \date 2012/05/01 20:32
 */

#include "GraphicsInclude.h"
#include "nuVertexBuffer.h"
#include "nuElementBuffer.h"
#include "nuShaderProgram.h"
#include "nuUniformBuffer.h"
#include "nuGSetupContext.h"

nuGSetupContext::nuGSetupContext()
    : mFrameID(0)
{
  // None...
}

nuGSetupContext::~nuGSetupContext()
{

}

void nuGSetupContext::beginSetup(i64 frame_id)
{
  mFrameID = frame_id;
}

void nuGSetupContext::endSetup(void)
{
  // None...
}

void nuGSetupContext::map(nuVertexBuffer& vertex_buffer)
{
  if(vertex_buffer.isMapped())
    return;

  if(vertex_buffer.isInitialized() && vertex_buffer.getHandle()) {
    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer.getHandle()));
    CHECK_GL_ERROR(vertex_buffer.mpBuffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    if(vertex_buffer.mpBuffer) {
      vertex_buffer.setMapped(true);
      vertex_buffer.setUpdate(true);
    }
  }
}

void nuGSetupContext::map(nuElementBuffer& element_buffer)
{
  if(element_buffer.isMapped())
    return;

  if(element_buffer.isInitialized() && element_buffer.getHandle()) {
    CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer.getHandle()));
    CHECK_GL_ERROR(element_buffer.mpBuffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
    if(element_buffer.mpBuffer) {
      element_buffer.setMapped(true);
      element_buffer.setUpdate(true);
    }
  }
}

void nuGSetupContext::map(class nuUniformBuffer& uniform_buffer)
{
  if(uniform_buffer.isMapped())
    return;

  if(uniform_buffer.isInitialized() && uniform_buffer.getHandle()) {
    CHECK_GL_ERROR(glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer.getHandle()));
    CHECK_GL_ERROR(uniform_buffer.mpBuffer = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY));
    if(uniform_buffer.mpBuffer) {
      uniform_buffer.setMapped(true);
      uniform_buffer.setUpdate(true);
    }
  }
}
