/*!
 * \file nuVertexBuffer.cpp
 * \brief Vertex buffer.
 * \author Nus
 * \date 2012/03/23 12:28
 */

#include "GraphicsInclude.h"
#include "nuVertexBuffer.h"

IMPLEMENT_TYPE_INFO_INST(nuVertexBuffer, nuGResource, nullptr);

nuVertexBuffer::nuVertexBuffer(size_t size, nuGResource::RESOURCE_USAGE usage)
    : nuGResource(nuGResource::VERTEX_BUFFER, usage),
      mpBuffer(nullptr),
      mSize(size),
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
    releaseBuffer();
    setInitialized(true);
  }

  if(isMapped()) {
    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID));
    CHECK_GL_ERROR(glUnmapBuffer(GL_ARRAY_BUFFER));
    mpBuffer = nullptr;
    setMapped(false);
  }  
}
