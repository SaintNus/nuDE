/*!
 * \file nuElementBuffer.cpp
 * \brief Element buffer.
 * \author Nus
 * \date 2012/03/22 01:51
 */

#include "nuElementBuffer.h"

IMPLEMENT_TYPE_INFO(nuElementBuffer, nuGResource);

nuElementBuffer::nuElementBuffer(size_t size, nuGResource::RESOURCE_USAGE usage)
    : nuGResource(nuGResource::ELEMENT_BUFFER, usage),
      mpBuffer(nullptr),
      mSize(0)
{
  mpBuffer = nude::Alloc(size);
  if(mpBuffer)
    mSize = size;
}

nuElementBuffer::~nuElementBuffer()
{
  releaseBuffer();
}

void nuElementBuffer::update(void)
{
  if(getUsage() == nuGResource::STATIC_RESOURCE)
    releaseBuffer();
}
