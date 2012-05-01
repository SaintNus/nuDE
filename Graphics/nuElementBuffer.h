/*!
 * \file nuElementBuffer.h
 * \brief Element buffer.
 * \author Nus
 * \date 2012/03/22 01:51
 */

#ifndef __NUELEMENTBUFFER_H__
#define __NUELEMENTBUFFER_H__

#include "nuGResource.h"

class nuElementBuffer : public nuGResource
{
  DECLARE_TYPE_INFO;
  friend class nuGResManager;
  friend nude::Handle< nuElementBuffer >;

public:
  enum ELEMENT_TYPE {
    UNSIGNED_INT_16,
    UNSIGNED_INT_32,
  };

private:
  size_t getElementSize(ELEMENT_TYPE type) const {
    const size_t sz_table[] = {
      2,
      4,
    };
    return sz_table[type];
  }

  ELEMENT_TYPE mElementType;
  ui32 mElementNum;

  void* mpBuffer;
  size_t mSize;
  size_t mUpdateSize;

  GLuint mElementBufferID;

  void update(void);

  void releaseBuffer(void) {
    if(mpBuffer) {
      nude::Dealloc(mpBuffer);
      mpBuffer = nullptr;
    }
  }

  nuElementBuffer();
  nuElementBuffer(ELEMENT_TYPE type, ui32 element_num, nuGResource::RESOURCE_USAGE usage);
  ~nuElementBuffer();

public:
  GLuint getHandle(void) const {
    return mElementBufferID;
  }

  void* getBuffer(void) const {
    return mpBuffer;
  }

  void commit(ui32 element_num) {
    mUpdateSize = element_num * getElementSize(mElementType);
    setUpdate(true);
  }

  size_t getSize(void) const {
    return mSize;
  }

  ELEMENT_TYPE getElementType(void) const {
    return mElementType;
  }

  ui32 getElementNum(void) const {
    return mElementNum;
  }

  void bind(void) const {
    static const GLuint restart_idx[] = {
      0xffff,
      0xffffffff,
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBufferID);
    glPrimitiveRestartIndex(restart_idx[mElementType]);
  }

};

namespace nude {
  typedef Handle< nuElementBuffer > ElementBuffer;
}

#endif
