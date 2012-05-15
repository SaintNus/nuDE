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
#include "nuShaderProgram.h"

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

  class SortTagContext {
    friend class nuGContext;
    nuGContext* mpContext;
    ui32 mTagNum;
  public:
    SortTagContext() {}
    ~SortTagContext() {}

    bool initialize(nuGContext* p_ctx) {
      if(mpContext->getTagNum() == 0)
        return false;
      mpContext = p_ctx;
      mTagNum = mpContext->getTagNum();
      return true;
    }

    static void createTagList(TagList& tag_list, SortTagContext* ctx, ui32 ctx_num);

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

    void* allocBuffer(size_t size) {
      if(mRemain < size) {
        mpBuffer = mContextBuffer.allocRingBuffer(nuGContextBuffer::CONTEXT_BUFFER_SIZE);
        if(mpBuffer)
          mRemain = nuGContextBuffer::CONTEXT_BUFFER_SIZE;
        else
          mRemain = 0;
      }

      if(mpBuffer) {
        ui8* ret = static_cast< ui8* >(mpBuffer);
        mpBuffer = &ret[size];
        mRemain -= size;
        return ret;
      }

      return nullptr;
    }

  };

  struct UniformValue {
    enum TYPE {
      FLOAT_1 = 0,
      FLOAT_2,
      FLOAT_3,
      FLOAT_4,
      INT_1,
      INT_2,
      INT_3,
      INT_4,
      UINT_1,
      UINT_2,
      UINT_3,
      UINT_4,
      FLOAT_1_V,
      FLOAT_2_V,
      FLOAT_3_V,
      FLOAT_4_V,
      INT_1_V,
      INT_2_V,
      INT_3_V,
      INT_4_V,
      UINT_1_V,
      UINT_2_V,
      UINT_3_V,
      UINT_4_V,
      MATRIX_2_V,
      MATRIX_3_V,
      MATRIX_4_V,
      MATRIX_2x3_V,
      MATRIX_3x2_V,
      MATRIX_2x4_V,
      MATRIX_4x2_V,
      MATRIX_3x4_V,
      MATRIX_4x3_V,
    };
    GLuint location;
    ui32 type: 6;
    ui32 transpose: 1;
    ui32 count: 25;
    void* p_data;
  };

  size_t getUniformSize(UniformValue::TYPE type) const {
    const size_t tbl[] = {
      sizeof(f32) * 1,
      sizeof(f32) * 2,
      sizeof(f32) * 3,
      sizeof(f32) * 4,

      sizeof(i32) * 1,
      sizeof(i32) * 2,
      sizeof(i32) * 3,
      sizeof(i32) * 4,

      sizeof(ui32) * 1,
      sizeof(ui32) * 2,
      sizeof(ui32) * 3,
      sizeof(ui32) * 4,

      sizeof(f32) * 1,
      sizeof(f32) * 2,
      sizeof(f32) * 3,
      sizeof(f32) * 4,

      sizeof(i32) * 1,
      sizeof(i32) * 2,
      sizeof(i32) * 3,
      sizeof(i32) * 4,

      sizeof(ui32) * 1,
      sizeof(ui32) * 2,
      sizeof(ui32) * 3,
      sizeof(ui32) * 4,

      sizeof(f32) * 2 * 2,
      sizeof(f32) * 3 * 3,
      sizeof(f32) * 4 * 4,

      sizeof(f32) * 2 * 3,
      sizeof(f32) * 3 * 2,
      sizeof(f32) * 2 * 4,
      sizeof(f32) * 4 * 2,
      sizeof(f32) * 3 * 4,
      sizeof(f32) * 4 * 3,
    };
    return tbl[type];
  }

  struct ProgramObject {
    nuShaderProgram* p_shader_program;
    ui32 uniform_num;
    UniformValue* p_value;
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
    ProgramObject program_object;
    nuVertexArray* p_vertex_array;
    nuVertexBuffer* p_vertex_buffer;
    nuElementBuffer* p_element_buffer;
    ui32 primitive_mode: 4;
    ui32 element_num: 28;
  };
  typedef DrawCmd< DrawElements > DrawElementsCmd;

  static const ui32 EXPAND_TEMP_TAG_NUM = 256;
  static const ui32 EXPAND_UNIFORM_VALUE = 64;

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
  UniformValue* mpUniformValue;
  ui32 mUniformValueNum;
  ui32 mCurrentUniformValue;

  nuGContext();

  void sortTag(void);

  Tag* getTags(void) const {
    return mpTag;
  }

  ui32 getTagNum(void) const {
    return mCurrentTag;
  }

  void setUniform(ui32 index, UniformValue::TYPE type, ui32 size, bool transpose, const void* p_data);

public:
  nuGContext(nuGContextBuffer& ctx_buffer);
  ~nuGContext();

  void begin(i64 frame_id, Tag* p_tag, ui32 tag_num);
  void end(void);

  void setPriority(nude::PASS pass, ui32 priority) {
    mCurrentPriority.pass = pass;
    mCurrentPriority.priority = priority;
  }

  void beginDraw(nude::PASS pass, ui32 priority, const nude::ShaderProgram& program) {
    setPriority(pass, priority);
    program->protect(mFrameID);
    mCurrentDrawElements.program_object.p_shader_program = &program;
    mCurrentUniformValue = 0;
  }

  void endDraw(void) {
    mCurrentDrawElements.program_object.p_shader_program = nullptr;
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

  void setUniform1(ui32 index, f32 data) {
    f32 val = data;
    setUniform(index, UniformValue::FLOAT_1, 1, false, &val);
  }
  void setUniform2(ui32 index, const f32* p_data) {
    setUniform(index, UniformValue::FLOAT_2, 1, false, p_data);
  }
  void setUniform3(ui32 index, const f32* p_data) {
    setUniform(index, UniformValue::FLOAT_3, 1, false, p_data);
  }
  void setUniform4(ui32 index, const f32* p_data) {
    setUniform(index, UniformValue::FLOAT_4, 1, false, p_data);
  }

  void setUniform1(ui32 index, i32 data) {
    i32 val = data;
    setUniform(index, UniformValue::INT_1, 1, false, &val);
  }
  void setUniform2(ui32 index, const i32* p_data) {
    setUniform(index, UniformValue::INT_2, 1, false, p_data);
  }
  void setUniform3(ui32 index, const i32* p_data) {
    setUniform(index, UniformValue::INT_3, 1, false, p_data);
  }
  void setUniform4(ui32 index, const i32* p_data) {
    setUniform(index, UniformValue::INT_4, 1, false, p_data);
  }

  void setUniform1(ui32 index, ui32 data) {
    ui32 val = data;
    setUniform(index, UniformValue::UINT_1, 1, false, &val);
  }
  void setUniform2(ui32 index, const ui32* p_data) {
    setUniform(index, UniformValue::UINT_2, 1, false, p_data);
  }
  void setUniform3(ui32 index, const ui32* p_data) {
    setUniform(index, UniformValue::UINT_3, 1, false, p_data);
  }
  void setUniform4(ui32 index, const ui32* p_data) {
    setUniform(index, UniformValue::UINT_4, 1, false, p_data);
  }

  void setUniform1(ui32 index, ui32 count, const f32* p_data) {
    setUniform(index, UniformValue::FLOAT_1_V, count, false, p_data);
  }
  void setUniform2(ui32 index, ui32 count, const f32* p_data) {
    setUniform(index, UniformValue::FLOAT_2_V, count, false, p_data);
  }
  void setUniform3(ui32 index, ui32 count, const f32* p_data) {
    setUniform(index, UniformValue::FLOAT_3_V, count, false, p_data);
  }
  void setUniform4(ui32 index, ui32 count, const f32* p_data) {
    setUniform(index, UniformValue::FLOAT_4_V, count, false, p_data);
  }

  void setUniform1(ui32 index, ui32 count, const i32* p_data) {
    setUniform(index, UniformValue::INT_1_V, count, false, p_data);
  }
  void setUniform2(ui32 index, ui32 count, const i32* p_data) {
    setUniform(index, UniformValue::INT_2_V, count, false, p_data);
  }
  void setUniform3(ui32 index, ui32 count, const i32* p_data) {
    setUniform(index, UniformValue::INT_3_V, count, false, p_data);
  }
  void setUniform4(ui32 index, ui32 count, const i32* p_data) {
    setUniform(index, UniformValue::INT_4_V, count, false, p_data);
  }

  void setUniform1(ui32 index, ui32 count, const ui32* p_data) {
    setUniform(index, UniformValue::UINT_1_V, count, false, p_data);
  }
  void setUniform2(ui32 index, ui32 count, const ui32* p_data) {
    setUniform(index, UniformValue::UINT_2_V, count, false, p_data);
  }
  void setUniform3(ui32 index, ui32 count, const ui32* p_data) {
    setUniform(index, UniformValue::UINT_3_V, count, false, p_data);
  }
  void setUniform4(ui32 index, ui32 count, const ui32* p_data) {
    setUniform(index, UniformValue::UINT_4_V, count, false, p_data);
  }

  void setUniformMatrix2(ui32 index, ui32 count, bool transpose, const f32* p_data) {
    setUniform(index, UniformValue::MATRIX_2_V, count, transpose, p_data);
  }
  void setUniformMatrix3(ui32 index, ui32 count, bool transpose, const f32* p_data) {
    setUniform(index, UniformValue::MATRIX_3_V, count, transpose, p_data);
  }
  void setUniformMatrix4(ui32 index, ui32 count, bool transpose, const f32* p_data) {
    setUniform(index, UniformValue::MATRIX_4_V, count, transpose, p_data);
  }

  void setUniformMatrix2x3(ui32 index, ui32 count, bool transpose, const f32* p_data) {
    setUniform(index, UniformValue::MATRIX_2x3_V, count, transpose, p_data);
  }
  void setUniformMatrix3x2(ui32 index, ui32 count, bool transpose, const f32* p_data) {
    setUniform(index, UniformValue::MATRIX_3x2_V, count, transpose, p_data);
  }

  void setUniformMatrix2x4(ui32 index, ui32 count, bool transpose, const f32* p_data) {
    setUniform(index, UniformValue::MATRIX_2x4_V, count, transpose, p_data);
  }
  void setUniformMatrix4x2(ui32 index, ui32 count, bool transpose, const f32* p_data) {
    setUniform(index, UniformValue::MATRIX_4x2_V, count, transpose, p_data);
  }

  void setUniformMatrix3x4(ui32 index, ui32 count, bool transpose, const f32* p_data) {
    setUniform(index, UniformValue::MATRIX_3x4_V, count, transpose, p_data);
  }
  void setUniformMatrix4x3(ui32 index, ui32 count, bool transpose, const f32* p_data) {
    setUniform(index, UniformValue::MATRIX_4x3_V, count, transpose, p_data);
  }

};

#endif
