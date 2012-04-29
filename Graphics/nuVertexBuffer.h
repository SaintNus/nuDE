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

  void* mpBuffer;
  size_t mSize;
  GLuint mVertexBufferID;
  size_t mUpdateSize;

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
  void* getBuffer(void) const {
    return mpBuffer;
  }

  size_t getSize(void) const {
    return mSize;
  }

  void commit(size_t size) {
    mUpdateSize = size;
    setUpdate(true);
  }

};

typedef nude::Handle< nuVertexBuffer > nuVertexHandle;

#endif
