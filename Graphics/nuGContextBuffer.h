/*!
 * \file nuGContextBuffer.h
 * \brief Buffer for graphic context.
 * \author Nus
 * \date 2012/05/02 19:35
 */

#ifndef __NUGCONTEXTBUFFER_H__
#define __NUGCONTEXTBUFFER_H__

class nuGContextBuffer
{
  nuRingBuffer mRingBuffer;

public:
  static const size_t CONTEXT_BUFFER_SIZE = 64 * 1024;

  nuGContextBuffer(size_t rb_size);
  ~nuGContextBuffer();

  void* allocRingBuffer(size_t size) {
    return mRingBuffer.alloc(size);
  }

};

#endif
