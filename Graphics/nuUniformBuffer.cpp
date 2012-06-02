/*!
 * \file nuUniformBuffer.cpp
 * \brief Uniform buffer.
 * \author Nus
 * \date 2012/05/16 22:15
 */

#include "GraphicsInclude.h"
#include "nuUniformBuffer.h"

#define UNIFORM_BUFFER_MAP_THRESHOLD (32 * 1024)

IMPLEMENT_TYPE_INFO_INST(nuUniformBuffer, nuGResource, nullptr);

nuUniformBuffer::nuUniformBuffer(size_t size)
    : nuGResource(nuGResource::UNIFORM_BUFFER, nuGResource::DYNAMIC_RESOURCE),
      mpBuffer(nullptr),
      mSize(size),
      mCommitSize(0),
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

    mCommitSize = 0;
    setInitialized(true);
  } else {
    CHECK_GL_ERROR(glBindBuffer(GL_UNIFORM_BUFFER, mUniformBufferID));

    if(mCommitSize > UNIFORM_BUFFER_MAP_THRESHOLD) {
      void* p_buffer;
      CHECK_GL_ERROR(p_buffer = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY));
      memcpy(p_buffer, mpBuffer, mCommitSize);
      CHECK_GL_ERROR(glUnmapBuffer(GL_UNIFORM_BUFFER));
    } else {
      CHECK_GL_ERROR(glBufferSubData(GL_UNIFORM_BUFFER, 0, mCommitSize, mpBuffer));
    }

    mCommitSize = 0;
  }
}
