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
  mSize = getElementSize(mElementType) * mElementNum;
}

nuElementBuffer::~nuElementBuffer()
{
  releaseBuffer();
  if(mElementBufferID > 0)
    glDeleteBuffers(1, &mElementBufferID);
}

void nuElementBuffer::update(void)
{
  if(isMapped()) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBufferID);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    mpBuffer = nullptr;
    setMapped(false);
  }  

  if(!isUpdated())
    return;

  if(!isInitialized()) {
    glGenBuffers(1, &mElementBufferID);
    NU_ASSERT(mElementBufferID != 0, "Cannot generate vertex buffer object.\n");

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mUpdateSize, mpBuffer, usage);
    releaseBuffer();
    setInitialized(true);
    setUpdate(false);
    mUpdateSize = 0;
  }
}
