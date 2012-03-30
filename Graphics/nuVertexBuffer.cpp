/*!
 * \file nuVertexBuffer.cpp
 * \brief Vertex buffer.
 * \author Nus
 * \date 2012/03/23 12:28
 */

#include "nuVertexBuffer.h"

IMPLEMENT_TYPE_INFO(nuVertexBuffer, nuGResource);

nuVertexBuffer::nuVertexBuffer(size_t size, nuGResource::RESOURCE_USAGE usage)
    : nuGResource(nuGResource::VERTEX_BUFFER, usage),
      mpBuffer(nullptr),
      mSize(0)
{
  mpBuffer = nude::Alloc(size);
  if(mpBuffer)
    mSize = size;
}

nuVertexBuffer::~nuVertexBuffer()
{
  releaseBuffer();
}

void nuVertexBuffer::update(void)
{
  if(getUsage() == nuGResource::STATIC_RESOURCE)
    releaseBuffer();
}
