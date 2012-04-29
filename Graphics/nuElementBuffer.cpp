/*!
 * \file nuElementBuffer.cpp
 * \brief Element buffer.
 * \author Nus
 * \date 2012/03/22 01:51
 */

#include "nuElementBuffer.h"

IMPLEMENT_TYPE_INFO_INST(nuElementBuffer, nuGResource, nullptr);

nuElementBuffer::nuElementBuffer(ELEMENT_TYPE type, ui32 element_num, nuGResource::RESOURCE_USAGE usage)
    : nuGResource(nuGResource::ELEMENT_BUFFER, usage),
      mpBuffer(nullptr),
      mSize(0),
      mUpdateSize(0),
      mElementBufferID(0),
      mElementNum(element_num),
      mElementType(type)
{
  size_t size = getElementSize(mElementType) * mElementNum;
  mpBuffer = nude::Alloc(size);
  if(mpBuffer)
    mSize = size;
}

nuElementBuffer::~nuElementBuffer()
{
  releaseBuffer();
  if(mElementBufferID > 0)
    glDeleteBuffers(1, &mElementBufferID);
}

void nuElementBuffer::update(void)
{
  if(!isUpdated())
    return;

  if(!isInitialized()) {
    glGenBuffers(1, &mElementBufferID);
    NU_ASSERT(mElementBufferID != 0, "Cannot generate vertex buffer object.\n");
  }

  switch(getUsage()) {
  case nuGResource::STATIC_RESOURCE:
    if(!isInitialized()) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBufferID);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, mUpdateSize, mpBuffer, GL_STATIC_DRAW);
      releaseBuffer();
      setInitialized(true);
      setUpdate(false);
      mUpdateSize = 0;
    }
    break;
  case nuGResource::DYNAMIC_RESOURCE:
    if(!isInitialized()) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBufferID);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, mSize, mpBuffer, GL_STATIC_DRAW);
      setInitialized(true);
      setUpdate(false);
      mUpdateSize = 0;
    } else {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBufferID);
      glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mUpdateSize, mpBuffer);
      setUpdate(false);
      mUpdateSize = 0;
    }
    break;
  default:
    NU_ASSERT(false, "Logical error.\n");
  }
}
