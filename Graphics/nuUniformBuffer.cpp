/*!
 * \file nuUniformBuffer.cpp
 * \brief Uniform buffer.
 * \author Nus
 * \date 2012/05/16 22:15
 */

#include "GraphicsInclude.h"
#include "nuUniformBuffer.h"

IMPLEMENT_TYPE_INFO_INST(nuUniformBuffer, nuGResource, nullptr);

nuUniformBuffer::nuUniformBuffer(size_t size)
    : nuGResource(nuGResource::UNIFORM_BUFFER, nuGResource::DYNAMIC_RESOURCE),
      mpBuffer(nullptr),
      mSize(size),
      mUniformBufferID(0)
{
  // None...
}

nuUniformBuffer::~nuUniformBuffer()
{
  releaseBuffer();
  if(mUniformBufferID != 0)
    CHECK_GL_ERROR(glDeleteBuffers(1, &mUniformBufferID));
}

void nuUniformBuffer::update(void)
{
  if(!isInitialized()) {
    CHECK_GL_ERROR(glGenBuffers(1, &mUniformBufferID));
    NU_ASSERT(mUniformBufferID != 0, "Cannot generate uniform buffer object.\n");

    CHECK_GL_ERROR(glBindBuffer(GL_UNIFORM_BUFFER, mUniformBufferID));
    CHECK_GL_ERROR(glBufferData(GL_UNIFORM_BUFFER, mSize, mpBuffer, getResourceUsage()));
    releaseBuffer();
    setInitialized(true);
  }

  if(isMapped()) {
    CHECK_GL_ERROR(glBindBuffer(GL_UNIFORM_BUFFER, mUniformBufferID));
    CHECK_GL_ERROR(glUnmapBuffer(GL_UNIFORM_BUFFER));
    mpBuffer = nullptr;
    setMapped(false);
  }  
}
