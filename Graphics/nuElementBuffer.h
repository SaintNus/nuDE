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

  void* mpBuffer;
  size_t mSize;

  void update(void);

  void releaseBuffer(void) {
    if(mpBuffer) {
      nude::Dealloc(mpBuffer);
      mpBuffer = nullptr;
    }
  }

  nuElementBuffer() {}

public:
  nuElementBuffer(size_t size, nuGResource::RESOURCE_USAGE usage);
  ~nuElementBuffer();

  void* getBuffer(void) const {
    return mpBuffer;
  }

  size_t getSize(void) const {
    return mSize;
  }

};

#endif
