/*!
 * \file nuRingBuffer.h
 * \brief Ring buffer.
 * \author Nus
 * \date 2012/05/02 22:19
 */

#ifndef __NURINGBUFFER_H__
#define __NURINGBUFFER_H__

#include "nuMemory.h"

class nuRingBuffer
{
  void* mpBuffer;
  void* mpCurrent;
  size_t mSize;
  size_t mRemainSize;

  void initialize(size_t size) {
    if(size > 0) {
      if(mpBuffer) {
        nude::Dealloc(mpBuffer);
        mpBuffer = nullptr;
        mSize = 0;
        mRemainSize = 0;
        mpCurrent = nullptr;
      }

      mpBuffer = nude::Alloc(size);

      if(mpBuffer) {
        mpCurrent = mpBuffer;
        mSize = size;
        mRemainSize = mSize;
      }
    }
  }

public:
  nuRingBuffer(size_t size)
      : mSize(0),
        mRemainSize(0),
        mpBuffer(nullptr),
        mpCurrent(nullptr)
  {
    initialize(size);
  }

  ~nuRingBuffer() {
    if(mpBuffer) {
      nude::Dealloc(mpBuffer);
      mpBuffer = nullptr;
    }
  }

  template< class T >
  T* alloc(ui32 num) {
    size_t sz = sizeof(T) * num;
    if(mSize < sz)
      return nullptr;
    if(mRemainSize < sz) {
      mpCurrent = mpBuffer;
      mRemainSize = mSize;
    }
    T* ret = static_cast< T* >(mpCurrent);
    mpCurrent = &ret[num];
    mRemainSize -= sz;
    return ret;
  }

  void* alloc(size_t size) {
    if(mSize < size)
      return nullptr;
    if(mRemainSize < size) {
      mpCurrent = mpBuffer;
      mRemainSize = mSize;
    }
    ui8* ret = static_cast< ui8* >(mpCurrent);
    mpCurrent = &ret[size];
    mRemainSize -= size;
    return ret;
  }

  size_t getSize(void) const {
    return mSize;
  }

};

#endif
