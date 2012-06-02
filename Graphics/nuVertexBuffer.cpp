/*!
 * \file nuVertexBuffer.cpp
 * \brief Vertex buffer.
 * \author Nus
 * \date 2012/03/23 12:28
 */

#include "GraphicsInclude.h"
#include "nuVertexBuffer.h"

#define VERTEX_BUFFER_MAP_THRESHOLD (32 * 1024)

IMPLEMENT_TYPE_INFO_INST(nuVertexBuffer, nuGResource, nullptr);

nuVertexBuffer::nuVertexBuffer(size_t size, nuGResource::RESOURCE_USAGE usage)
    : nuGResource(nuGResource::VERTEX_BUFFER, usage),
      mpBuffer(nullptr),
      mSize(size),
      mCommitSize(0),
      mVertexBufferID(0)
{
  // None...
}

nuVertexBuffer::~nuVertexBuffer()
{
  releaseBuffer();
  if(mVertexBufferID != 0)
    CHECK_GL_ERROR(glDeleteBuffers(1, &mVertexBufferID));
}

void nuVertexBuffer::update(void)
{
  if(!isInitialized()) {
    CHECK_GL_ERROR(glGenBuffers(1, &mVertexBufferID));
    NU_ASSERT(mVertexBufferID != 0, "Cannot generate vertex buffer object.\n");

    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID));
    CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, mSize, mpBuffer, getResourceUsage()));
    
    if(getUsage() == nuGResource::STATIC_RESOURCE)
      releaseBuffer();

    mCommitSize = 0;
    setInitialized(true);
  } else {
    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID));

    if(mCommitSize > VERTEX_BUFFER_MAP_THRESHOLD) {
      void* p_buffer;
      CHECK_GL_ERROR(p_buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
      memcpy(p_buffer, mpBuffer, mCommitSize);
      CHECK_GL_ERROR(glUnmapBuffer(GL_ARRAY_BUFFER));
    } else {
      CHECK_GL_ERROR(glBufferSubData(GL_ARRAY_BUFFER, 0, mCommitSize, mpBuffer));
    }

    mCommitSize = 0;
  }
}
