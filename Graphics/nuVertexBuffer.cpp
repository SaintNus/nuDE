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
      mSize(0),
      mUpdateSize(0),
      mVertexBufferID(0)
{
  mpBuffer = nude::Alloc(size);
  if(mpBuffer)
    mSize = size;
}

nuVertexBuffer::~nuVertexBuffer()
{
  releaseBuffer();
  if(mVertexBufferID != 0)
    glDeleteBuffers(1, &mVertexBufferID);
}

void nuVertexBuffer::update(void)
{
  if(!isUpdated())
    return;

  if(!isInitialized()) {
    glGenBuffers(1, &mVertexBufferID);
    NU_ASSERT(mVertexBufferID != 0, "Cannot generate vertex buffer object.\n");
  }

  switch(getUsage()) {
  case nuGResource::STATIC_RESOURCE:
    if(!isInitialized()) {
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
      glBufferData(GL_ARRAY_BUFFER, mUpdateSize, mpBuffer, GL_STATIC_DRAW);
      releaseBuffer();
      setInitialized(true);
      setUpdate(false);
      mUpdateSize = 0;
    }
    break;
  case nuGResource::DYNAMIC_RESOURCE:
    if(!isInitialized()) {
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
      glBufferData(GL_ARRAY_BUFFER, mSize, mpBuffer, GL_STATIC_DRAW);
      setInitialized(true);
      setUpdate(false);
      mUpdateSize = 0;
    } else {
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
      glBufferSubData(GL_ARRAY_BUFFER, 0, mUpdateSize, mpBuffer);
      setUpdate(false);
      mUpdateSize = 0;
    }
    break;
  default:
    NU_ASSERT(false, "Logical error.\n");
  }
}
