/*!
 * \file nuVertexArray.cpp
 * \brief Vertex array.
 * \author Nus
 * \date 2012/05/05 17:53
 */

#include "GraphicsInclude.h"
#include "nuVertexArray.h"

IMPLEMENT_TYPE_INFO(nuVertexArray, nuGResource);

nuVertexArray::nuVertexArray()
    : nuGResource(nuGResource::VERTEX_ARRAY, nuGResource::DYNAMIC_RESOURCE),
      mHandle(0),
      mVertexBuffer(0),
      mpArray(nullptr),
      mArrayNum(0)
{
  // None...
}

nuVertexArray::~nuVertexArray()
{
  if(mHandle != 0) {
    CHECK_GL_ERROR(glDeleteVertexArrays(1, &mHandle));
    mHandle = 0;
  }

  if(mpArray) {
    delete[] mpArray;
    mpArray = nullptr;
  }
}

void nuVertexArray::update(void)
{
  if(!isInitialized()) {
    CHECK_GL_ERROR(glGenVertexArrays(1, &mHandle));
    NU_ASSERT(mHandle != 0, "Cannot generate vertex array object.\n");
    setInitialized(true);
  }

  CHECK_GL_ERROR(glBindVertexArray(mHandle));
  CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer));

  for(ui32 ui = 0; ui < mArrayNum; ui++)
    CHECK_GL_ERROR(glEnableVertexAttribArray(static_cast< GLuint >(ui)));

  for(ui32 ui = mArrayNum; ui < MAX_VERTEX_ATTRIBUTE; ui++)
    CHECK_GL_ERROR(glDisableVertexAttribArray(static_cast< GLuint >(ui)));
}

void nuVertexArray::begin(ui32 array_num)
{
  if(mpArray) {
    if(mArrayNum < array_num) {
      delete[] mpArray;
      mArrayNum = array_num;
      mpArray = new Array[mArrayNum];
    }
  } else {
    mArrayNum = array_num;
    mpArray = new Array[mArrayNum];
  }
}

void nuVertexArray::declare(ui32 index, const Array& array)
{
  NU_ASSERT(index < mArrayNum, "Array overflow.\n");
  if(index < mArrayNum)
    mpArray[index] = array;
}

void nuVertexArray::end(void)
{
  setUpdate(true);
}

void nuVertexArray::reset(GLuint vertex_id)
{
  mVertexBuffer = vertex_id;

  CHECK_GL_ERROR(glBindVertexArray(mHandle));
  CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer));

  for(ui32 ui = 0; ui < mArrayNum; ui++) {
    Array& va = mpArray[ui];
    GLuint idx = static_cast< GLuint>(ui);
    CHECK_GL_ERROR(glVertexAttribPointer(idx,
                                         static_cast< GLint >(va.size),
                                         getAttributeType(va.type),
                                         va.normalized ? GL_TRUE : GL_FALSE,
                                         static_cast< GLsizei >(va.stride),
                                         reinterpret_cast< GLvoid* >(va.offset)));
  }
}
