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
      mVertexBufferID(0),
      mVertexArrayID(0),
      mVertexArrayNum(0),
      mStride(0)
{
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
  if(isMapped()) {
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    mpBuffer = nullptr;
    setMapped(false);
  }  

  if(isUpdated()) {
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

    glBindVertexArray(0);
    setUpdateVertexArray(false);
  }
}
