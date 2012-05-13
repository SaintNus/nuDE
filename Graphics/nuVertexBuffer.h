/*!
 * \file nuVertexBuffer.h
 * \brief Vertex buffer.
 * \author Nus
 * \date 2012/03/23 12:28
 */

#ifndef __NUVERTEXBUFFER_H__
#define __NUVERTEXBUFFER_H__

#include "nuGResource.h"

class nuVertexBuffer : public nuGResource
{
  DECLARE_TYPE_INFO;
  friend class nuGResManager;
  friend class nuGSetupContext;
  friend nude::Handle< class nuVertexBuffer >;

  enum EXTENSION_FLAG {
    MAPPED = 1 << 0,
  };

  void* mpBuffer;
  size_t mSize;
  GLuint mVertexBufferID;

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

  nuVertexBuffer();

  nuVertexBuffer(size_t size, nuGResource::RESOURCE_USAGE usage);
  ~nuVertexBuffer();

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
    return mVertexBufferID;
  }

  void* getBuffer(void) const {
    return mpBuffer;
  }

  size_t getSize(void) const {
    return mSize;
  }

};

namespace nude {
  typedef Handle< nuVertexBuffer > VertexBuffer;
}

#endif
