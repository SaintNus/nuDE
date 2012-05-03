/*!
 * \file nuGContext.h
 * \brief Graphic context.
 * \author Nus
 * \date 2011/10/30 21:49
 */

#ifndef __NUGCONTEXT_H__
#define __NUGCONTEXT_H__

#include "nuGraphics.h"
#include "nuGContextBuffer.h"

/*!
 * \class nuGContext
 * \brief Graphic context.
 */
class nuGContext
{
  friend class nuRenderGL;

  union Priority {
    ui32 value;
    struct {
      ui32 pass: 5;
      ui32 priority: 27;
    };
  };

public:
  enum TYPE {
    CLEAR = 0,
  };
    
  enum CLEAR_BIT {
    COLOR = GL_COLOR_BUFFER_BIT,
    DEPTH = GL_DEPTH_BUFFER_BIT,
    STENCIL = GL_STENCIL_BUFFER_BIT,
  };

  class Tag {
    friend class nuGContext;
    Priority mPriority;
    void* mpCommand;
  public:
    Tag() {}
    ~Tag() {}
  };

private:
  class Buffer {
    nuGContextBuffer& mContextBuffer;
    size_t mRemain;
    void* mpBuffer;

  public:
    Buffer(nuGContextBuffer& ctx_buffer)
        : mContextBuffer(ctx_buffer),
          mRemain(0),
          mpBuffer(nullptr)
    {
      // None...
    }
    ~Buffer() {
      // None...
    }

    void reset(void) {
      mRemain = 0;
      mpBuffer = nullptr;
    }

    template< class T >
    T* allocBuffer(void) {
      if(mRemain < sizeof(T)) {
        mpBuffer = mContextBuffer.allocRingBuffer(nuGContextBuffer::CONTEXT_BUFFER_SIZE);
        if(mpBuffer)
          mRemain = nuGContextBuffer::CONTEXT_BUFFER_SIZE;
        else
          mRemain = 0;
      }

      if(mpBuffer) {
        T* ret = static_cast< T* >(mpBuffer);
        mpBuffer = &ret[1];
        mRemain -= sizeof(T);
        return ret;
      }

      return nullptr;
    }

  };

  template< class T >
  struct DrawCmd {
    TYPE type;
    T data;
  };

  struct Clear {
    ui32 clear_color;
    f32 depth_value;
    ui32 clear_bit;
  };

  typedef DrawCmd< Clear > ClearCmd;
  static const ui32 EXPAND_TEMP_TAG_NUM = 256;

  i64 mFrameID;
  Buffer mBuffer;
  Tag* mpTag;
  ui32 mTagNum;
  ui32 mCurrentTag;
  Tag* mpTempTag;
  ui32 mTempTagNum;
  Priority mCurrentPriority;

  nuGContext();

  void sortTag(void);

public:
  nuGContext(nuGContextBuffer& ctx_buffer);
  ~nuGContext();

  void begin(i64 frame_id, Tag* p_tag, ui32 tag_num);
  void end(void);

  Tag* getTags(void) const {
    return mpTag;
  }

  ui32 getTagNum(void) const {
    return mCurrentTag;
  }

  void setPriority(nude::PASS pass, ui32 priority) {
    mCurrentPriority.pass = pass;
    mCurrentPriority.priority = priority;
  }

  void clear(ui32 clear_bit, const nuColor& color, f32 depth);  

};

#endif
