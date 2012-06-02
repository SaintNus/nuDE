/*!
 * \file nuElementBuffer.cpp
 * \brief Element buffer.
 * \author Nus
 * \date 2012/03/22 01:51
 */

#include "GraphicsInclude.h"
#include "nuElementBuffer.h"

#define ELEMENT_BUFFER_MAP_THRESHOLD (32 * 1024)

IMPLEMENT_TYPE_INFO_INST(nuElementBuffer, nuGResource, nullptr);

nuElementBuffer::nuElementBuffer(ELEMENT_TYPE type, ui32 element_num, nuGResource::RESOURCE_USAGE usage)
    : nuGResource(nuGResource::ELEMENT_BUFFER, usage),
      mpBuffer(nullptr),
      mSize(0),
      mCommitSize(0),
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
    CHECK_GL_ERROR(glDeleteBuffers(1, &mElementBufferID));
}

void nuElementBuffer::update(void)
{
  if(!isInitialized()) {
    CHECK_GL_ERROR(glGenBuffers(1, &mElementBufferID));
    NU_ASSERT(mElementBufferID != 0, "Cannot generate vertex buffer object.\n");

    CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBufferID));
    CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, mSize, mpBuffer, getResourceUsage()));

    if(getUsage() == nuGResource::STATIC_RESOURCE)
      releaseBuffer();

    mCommitSize = 0;
    setInitialized(true);
  } else {
    CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBufferID));

    if(mCommitSize > ELEMENT_BUFFER_MAP_THRESHOLD) {
      void* p_buffer;
      CHECK_GL_ERROR(p_buffer = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
      memcpy(p_buffer, mpBuffer, mCommitSize);
      CHECK_GL_ERROR(glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER));
    } else {
      CHECK_GL_ERROR(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mCommitSize, mpBuffer));
    }

    mCommitSize = 0;
  }  
}
