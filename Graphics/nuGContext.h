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
#include "nuVertexArray.h"
#include "nuVertexBuffer.h"
#include "nuElementBuffer.h"

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
      ui32 priority: 28;
      ui32 pass: 4;
    };
  };

public:
  enum TYPE {
    CLEAR = 0,
    DRAW_ELEMENTS,
  };
    
  enum CLEAR_BIT {
    CLEAR_COLOR = GL_COLOR_BUFFER_BIT,
    CLEAR_DEPTH = GL_DEPTH_BUFFER_BIT,
    CLEAR_STENCIL = GL_STENCIL_BUFFER_BIT,
  };

  class Tag {
    friend class nuGContext;
    friend class nuRenderGL;

    Priority mPriority;
    void* mpCommand;
  public:
    Tag() {}
    ~Tag() {}
  };

  class TagList {
    friend class nuGContext;
    friend class nuRenderGL;

    static const ui32 EXPAND_TAG_LIST = 1024;

    Tag* mpTagList;
    ui32 mTagNum;
    ui32 mCapacity;

    void reserve(ui32 tag_num) {
      ui32 num = tag_num / EXPAND_TAG_LIST;
      num = (num + 1) * EXPAND_TAG_LIST;
      if(num > mCapacity) {
        if(mpTagList) {
          delete[] mpTagList;
          mTagNum = 0;
          mCapacity = 0;
        }

        mpTagList = new Tag[num];

        if(mpTagList) {
          mCapacity = num;
          mTagNum = 0;
        }
      }
    }

  public:
    TagList()
        : mpTagList(nullptr),
          mTagNum(0),
          mCapacity(0)
    {
      // None...
    }
          
    ~TagList() {
      if(mpTagList) {
        delete[] mpTagList;
        mpTagList = nullptr;
      }
    }
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

  struct DrawElements {
    nuVertexArray* p_vertex_array;
    nuVertexBuffer* p_vertex_buffer;
    nuElementBuffer* p_element_buffer;
    ui32 primitive_mode: 4;
    ui32 element_num: 28;
  };
  typedef DrawCmd< DrawElements > DrawElementsCmd;

  static const ui32 EXPAND_TEMP_TAG_NUM = 256;

  i64 mFrameID;
  Buffer mBuffer;
  Tag* mpTag;
  ui32 mTagNum;
  ui32 mCurrentTag;
  Tag* mpTempTag;
  ui32 mTempTagNum;
  Priority mCurrentPriority;

  Clear mCurrentClear;
  DrawElements mCurrentDrawElements;

  nuGContext();

  void sortTag(void);

public:
  class SortTagContext {
    friend class nuGContext;
    nuGContext* mpContext;
    ui32 mTagNum;
  public:
    SortTagContext() {}
    ~SortTagContext() {}

    void initialize(nuGContext* p_ctx) {
      mpContext = p_ctx;
      mTagNum = mpContext->getTagNum();
    }
  };

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


  void setClearColor(const nuColor& color);
  void setClearDepth(f32 depth);
  void clear(ui32 clear_bit);

  void clear(ui32 clear_bit, const nuColor& color, f32 depth) {
    setClearColor(color);
    setClearDepth(depth);
    clear(clear_bit);
  }

  void setVertexArray(nude::VertexArray& array);
  void setVertexBuffer(nude::VertexBuffer& buffer);
  void setElementBuffer(nude::ElementBuffer& buffer);
  void drawElements(nude::PRIMITIVE_MODE primitive_mode, ui32 element_num);

  static void createTagList(TagList& tag_list, SortTagContext* ctx, ui32 ctx_num);

};

#endif
