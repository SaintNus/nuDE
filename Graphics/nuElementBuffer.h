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
  friend class nuGSetupContext;
  friend nude::Handle< nuElementBuffer >;

public:
  enum ELEMENT_TYPE {
    UNSIGNED_INT_16,
    UNSIGNED_INT_32,
  };

private:
  enum EXTENSION_FLAG {
    MAPPED = 1 << 0,
  };

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

  GLuint mElementBufferID;

  void update(void);

  void releaseBuffer(void) {
    if(!isInitialized() && mpBuffer) {
      nude::Dealloc(mpBuffer);
      mpBuffer = nullptr;
    }
  }

  void setMapped(bool mapped) {
    ui32 ext = getExtension();
    if(mapped)
      ext |= MAPPED;
    else
      ext &= ~MAPPED;
    setExtension(ext);
  }

  bool isMapped(void) const {
    return (getExtension() & MAPPED) ? true : false;
  }

  nuElementBuffer();
  nuElementBuffer(ELEMENT_TYPE type, ui32 element_num, nuGResource::RESOURCE_USAGE usage);
  ~nuElementBuffer();

public:
  void initialize(void) {
    if(!isInitialized()) {
      setUpdate(true);
    }
  }

  void* beginInitialize(void) {
    if(!isInitialized()) {
      if(!mpBuffer)
        mpBuffer = nude::Alloc(mSize);
      return mpBuffer;
    }
    return nullptr;
  }

  void endInitialize(void) {
    if(!isInitialized() && mpBuffer) {
      setUpdate(true);
    }
  }

  GLuint getHandle(void) const {
    return mElementBufferID;
  }

  void* getBuffer(void) const {
    return mpBuffer;
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

};

namespace nude {
  typedef Handle< nuElementBuffer > ElementBuffer;
}

#endif
