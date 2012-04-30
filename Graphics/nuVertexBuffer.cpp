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
      mVertexBufferID(0),
      mVertexArrayID(0),
      mVertexArrayNum(0),
      mStride(0)
{
  mpBuffer = nude::Alloc(size);
  if(mpBuffer)
    mSize = size;
  memset(mVertexArray, 0x00, sizeof(mVertexArray));
}

nuVertexBuffer::~nuVertexBuffer()
{
  releaseBuffer();
  if(mVertexBufferID != 0)
    glDeleteBuffers(1, &mVertexBufferID);
  if(mVertexArrayID != 0)
    glDeleteVertexArrays(1, &mVertexArrayID);
}

void nuVertexBuffer::update(void)
{
  if(isUpdated()) {
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

  if(isUpdateVertexArray() && mVertexBufferID != 0) {
    if(mVertexArrayID == 0) {
      glGenVertexArrays(1, &mVertexArrayID);
    }

    glBindVertexArray(mVertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);

    {
      ui32 flag = 0;

      for(ui32 ui = 0; ui < mVertexArrayNum; ui++) {
        VertexArray& va = mVertexArray[ui];
        GLuint idx = static_cast< GLuint >(va.index);

        flag |= 1 << idx;

        glEnableVertexAttribArray(idx);
        glVertexAttribPointer(idx,
                              static_cast< GLint >(va.size),
                              getAttributeType(va.type),
                              va.normalized ? GL_TRUE : GL_FALSE,
                              static_cast< GLsizei >(mStride),
                              reinterpret_cast< GLvoid* >(va.offset));
      }

      for(ui32 ui = 0; ui < MAX_VERTEX_ATTRIBUTE; ui++) {
        ui32 mask = 1 << ui;
        if((flag & mask) == 0)
          glDisableVertexAttribArray(static_cast< GLuint >(ui));
      }
    }

    setUpdateVertexArray(false);
  }
}
