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

  enum EXTENSION_FLAG {
    MAPPED = 1 << 0,
  };

  void* mpBuffer;
  GLuint mUniformBufferID;
  size_t mSize;

  void update(void);

  nuUniformBuffer();

  nuUniformBuffer(size_t size);
  ~nuUniformBuffer();

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

  void releaseBuffer(void) {
    if(!isInitialized() && mpBuffer) {
      nude::Dealloc(mpBuffer);
      mpBuffer = nullptr;
    }
  }

public:
  void initialize(void) {
    if(!isInitialized()) {
      setUpdate(true);
    }
  }

  void* beginInitialize(void) {
    if(!isInitialized()) {
      if(!mpBuffer)
        mpBuffer = nude::Alloc(0);
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
    return mUniformBufferID;
  }

  void* getBuffer(void) const {
    return mpBuffer;
  }

};

namespace nude {
  typedef Handle< nuUniformBuffer > UniformBuffer;
}

#endif
