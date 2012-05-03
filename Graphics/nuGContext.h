/*!
 * \file nuGContext.h
 * \brief Graphic context.
 * \author Nus
 * \date 2011/10/30 21:49
 */

#ifndef __NUGCONTEXT_H__
#define __NUGCONTEXT_H__

#include "nuGContextBuffer.h"

/*!
 * \class nuGContext
 * \brief Graphic context.
 */
class nuGContext
{
  friend class nuRenderGL;

public:
  enum TYPE {
    CLEAR = 0,
  };
    
  enum CLEAR_BIT {
    COLOR = GL_COLOR_BUFFER_BIT,
    DEPTH = GL_DEPTH_BUFFER_BIT,
    STENCIL = GL_STENCIL_BUFFER_BIT,
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

  struct Tag {
    union {
      ui32 value;
      struct {
        ui32 pass: 5;
        ui32 priority: 27;
      };
    };
    void* command;
  };

  template< class T >
  struct DrawCmd {
    TYPE type;
    T data;
  };

  struct Clear {
    nuColor clear_color;
    f32 depth_value;
    ui32 clear_bit;
  };

  typedef DrawCmd< Clear > ClearCmd;
  i64 mFrameID;
  Buffer mBuffer;
  Tag* mpTag;

  nuGContext();
  nuGContext(nuGContextBuffer& ctx_buffer);
  ~nuGContext();

public:
  void clear(ui32 clear_bit, const nuColor& color, f32 depth);

};

#endif
