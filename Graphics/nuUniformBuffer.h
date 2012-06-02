/*!
 * \file nuUniformBuffer.h
 * \brief Uniform buffer.
 * \author Nus
 * \date 2012/05/16 22:15
 */

#ifndef __NUUNIFORMBUFFER_H__
#define __NUUNIFORMBUFFER_H__

#include "nuGResource.h"

class nuUniformBuffer : public nuGResource
{
  DECLARE_TYPE_INFO;
  friend class nuGResManager;
  friend class nuGSetupContext;
  friend nude::Handle< nuUniformBuffer >;

  void* mpBuffer;
  GLuint mUniformBufferID;
  size_t mSize;
  size_t mCommitSize;

  void update(void);

  nuUniformBuffer();

  nuUniformBuffer(size_t size);
  ~nuUniformBuffer();

  void releaseBuffer(void) {
    if(!isInitialized() && mpBuffer) {
      nude::Dealloc(mpBuffer);
      mpBuffer = nullptr;
    }
  }

public:
  void* updateContent(void) {
    if(!isInitialized()) {
      if(!mpBuffer)
        mpBuffer = nude::Alloc(mSize);
    }
    return mpBuffer;
  }

  bool commitContent(size_t size) {
    if(size > 0 && mpBuffer) {
      mCommitSize = size > mSize ? mSize : size;
      setUpdate(true);
      return true;
    }
    return false;
  }

  GLuint getHandle(void) const {
    return mUniformBufferID;
  }

  size_t getSize(void) const {
    return mSize;
  }

};

namespace nude {
  typedef Handle< nuUniformBuffer > UniformBuffer;
}

#endif
