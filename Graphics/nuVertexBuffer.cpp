/*!
 * \file nuVertexBuffer.cpp
 * \brief Vertex buffer.
 * \author Nus
 * \date 2012/03/23 12:28
 */

#include "nuVertexBuffer.h"

IMPLEMENT_TYPE_INFO_INST(nuVertexBuffer, nuGResource, nullptr);

nuVertexBuffer::nuVertexBuffer(size_t size, nuGResource::RESOURCE_USAGE usage)
    : nuGResource(nuGResource::VERTEX_BUFFER, usage),
      mpBuffer(nullptr),
      mSize(size),
      mUpdateSize(0),
      mVertexBufferID(0)
{
  // None...
}

nuVertexBuffer::~nuVertexBuffer()
{
  releaseBuffer();
  if(mVertexBufferID != 0)
    glDeleteBuffers(1, &mVertexBufferID);
}

void nuVertexBuffer::update(void)
{
  if(isMapped()) {
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    mpBuffer = nullptr;
    setMapped(false);
  }  

  if(!isInitialized()) {
    glGenBuffers(1, &mVertexBufferID);
    NU_ASSERT(mVertexBufferID != 0, "Cannot generate vertex buffer object.\n");

    GLenum usage;
    switch(getUsage()) {
    case nuGResource::STATIC_RESOURCE:
      usage = GL_STATIC_DRAW;
      break;
    case nuGResource::DYNAMIC_RESOURCE:
      usage = GL_DYNAMIC_DRAW;
      break;
    default:
      NU_ASSERT(false, "Logical error.\n");
    }

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, mUpdateSize, mpBuffer, usage);
    releaseBuffer();
    setInitialized(true);
    setUpdate(false);
    mUpdateSize = 0;
  }
}
