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
  friend nude::Handle< nuVertexBuffer >;

  void* mpBuffer;
  size_t mSize;
  size_t mCommitSize;
  GLuint mVertexBufferID;

  void update(void);

  void releaseBuffer(void) {
    if(mpBuffer) {
      nude::Dealloc(mpBuffer);
      mpBuffer = nullptr;
    }
  }

  nuVertexBuffer();

  nuVertexBuffer(size_t size, nuGResource::RESOURCE_USAGE usage);
  ~nuVertexBuffer();

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
    return mVertexBufferID;
  }

  size_t getSize(void) const {
    return mSize;
  }

};

namespace nude {
  typedef Handle< nuVertexBuffer > VertexBuffer;
}

#endif
