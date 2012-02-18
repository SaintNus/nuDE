/*!
 * \file nuRenderBuffer.h
 * \brief Render buffer.
 * \author Nus
 * \date 2012/02/09 00:54
 */

#ifndef __NURENDERBUFFER_H__
#define __NURENDERBUFFER_H__

class nuRenderBuffer
{
private:
  class Scratch
  {
  private:
    void* mpBuffer;
    size_t mSize;
    size_t mRemaining;
    ui8* mpCurrent;

    Scratch();

  public:
    Scratch(size_t size)
        : mpBuffer(nullptr),
          mSize(size),
          mRemaining(size),
          mpCurrent(nullptr)
    {
      if(mSize > 0) {
        mpBuffer = nude::Alloc(mSize);
        NU_ASSERT(mpBuffer != nullptr, "Insufficient memory.\n");
        mpCurrent = static_cast< ui8* >(mpBuffer);
      }
    }

    ~Scratch() {
      if(mpBuffer) {
        nude::Dealloc(mpBuffer);
        mpBuffer = nullptr;
      }
    }

    void* getBuffer(void) const {
      return mpBuffer;
    }

    void* allocate(size_t size) {
      if(size > mSize)
        return nullptr;
      if(mRemaining >= size) {
        void* ret = mpCurrent;
        mpCurrent += size;
        return ret;
      }
      return nullptr;
    }

    void reset(void) {
      mRemaining = mSize;
      mpCurrent = static_cast< ui8* >(mpBuffer);
    }

  };

  Scratch mScratchBuffer;

public:
  nuRenderBuffer(size_t scratch_sz);
  ~nuRenderBuffer();

  void* allocScratch(size_t size) {
    return mScratchBuffer.allocate(size);
  }

  void reset(void) {
    mScratchBuffer.reset();
  }

};

#endif
