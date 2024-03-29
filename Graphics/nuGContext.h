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
#include "nuUniformBuffer.h"
#include "nuFrameBuffer.h"
#include "nuTexture.h"

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

  enum TYPE {
    CLEAR = 0,
    DRAW_ELEMENTS,
  };
    
public:
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
      if(p_ctx->getTagNum() == 0)
        return false;
      mpContext = p_ctx;
      mTagNum = mpContext->getTagNum();
      return true;
    }

    static void createTagList(TagList& tag_list, SortTagContext* ctx, ui32 ctx_num);

  };

  class ArrayDeclaration {
    friend nuGContext;

    nuVertexArray::Array* mpVertexArray;
    ui32 mCount;

  public:
    ArrayDeclaration()
        : mpVertexArray(nullptr),
          mCount(0)
    {
      // None...
    }

    ~ArrayDeclaration() {}
    
    ui32 getCount(void) const {
      return mCount;
    }

    nuVertexArray::Array& getDeclaration(ui32 index) {
      NU_ASSERT_C(mCount > 0 && index < mCount);
      return mpVertexArray[index];
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
    ui32 type: 6;
    ui32 transpose: 1;
    ui32 count: 25;
    void* p_data;
  };

  struct UniformBlock {
    nuUniformBuffer* p_buffer;
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

  UniformValue::TYPE convertToUniformValueType(GLenum type, GLint size) const {
    struct TypeTable {
      GLenum gl;
      UniformValue::TYPE type;
    };
    const TypeTable tbl[] = {
      { GL_FLOAT, UniformValue::FLOAT_1 },
      { GL_FLOAT_VEC2, UniformValue::FLOAT_2 },
      { GL_FLOAT_VEC3, UniformValue::FLOAT_3 },
      { GL_FLOAT_VEC4, UniformValue::FLOAT_4 },
      { GL_INT, UniformValue::INT_1 },
      { GL_INT_VEC2, UniformValue::INT_2 },
      { GL_INT_VEC3, UniformValue::INT_3 },
      { GL_INT_VEC4, UniformValue::INT_4 },
      { GL_UNSIGNED_INT, UniformValue::UINT_1 },
      { GL_UNSIGNED_INT_VEC2, UniformValue::UINT_2 },
      { GL_UNSIGNED_INT_VEC3, UniformValue::UINT_3 },
      { GL_UNSIGNED_INT_VEC4, UniformValue::UINT_4 },
      { GL_BOOL, UniformValue::UINT_1 },
      { GL_BOOL_VEC2, UniformValue::UINT_2 },
      { GL_FLOAT_MAT2, UniformValue::MATRIX_2_V },
      { GL_FLOAT_MAT2x3, UniformValue::MATRIX_2x3_V },
      { GL_FLOAT_MAT2x4, UniformValue::MATRIX_2x4_V },
      { GL_FLOAT_MAT3, UniformValue::MATRIX_3_V },
      { GL_FLOAT_MAT3x2, UniformValue::MATRIX_3x2_V },
      { GL_FLOAT_MAT3x4, UniformValue::MATRIX_3x4_V },
      { GL_FLOAT_MAT4, UniformValue::MATRIX_4_V },
      { GL_FLOAT_MAT4x2, UniformValue::MATRIX_4x2_V },
      { GL_FLOAT_MAT4x3, UniformValue::MATRIX_4x3_V },
      { GL_SAMPLER_1D, UniformValue::INT_1 },
      { GL_SAMPLER_1D_ARRAY, UniformValue::INT_1 },
      { GL_SAMPLER_1D_ARRAY_SHADOW, UniformValue::INT_1 },
      { GL_SAMPLER_1D_SHADOW, UniformValue::INT_1 },
      { GL_SAMPLER_2D, UniformValue::INT_1 },
      { GL_SAMPLER_2D_ARRAY, UniformValue::INT_1 },
      { GL_SAMPLER_2D_ARRAY_SHADOW, UniformValue::INT_1 },
      { GL_SAMPLER_2D_MULTISAMPLE, UniformValue::INT_1 },
      { GL_SAMPLER_2D_MULTISAMPLE_ARRAY, UniformValue::INT_1 },
      { GL_SAMPLER_2D_RECT, UniformValue::INT_1 },
      { GL_SAMPLER_2D_RECT_SHADOW, UniformValue::INT_1 },
      { GL_SAMPLER_2D_SHADOW, UniformValue::INT_1 },
      { GL_SAMPLER_3D, UniformValue::INT_1 },
      { GL_SAMPLER_BUFFER, UniformValue::INT_1 },
      { GL_SAMPLER_CUBE, UniformValue::INT_1 },
      { GL_SAMPLER_CUBE_SHADOW, UniformValue::INT_1 },
      { GL_INT_SAMPLER_1D, UniformValue::INT_1 },
      { GL_INT_SAMPLER_1D_ARRAY, UniformValue::INT_1 },
      { GL_INT_SAMPLER_2D, UniformValue::INT_1 },
      { GL_INT_SAMPLER_2D_ARRAY, UniformValue::INT_1 },
      { GL_INT_SAMPLER_2D_RECT, UniformValue::INT_1 },
      { GL_INT_SAMPLER_2D_MULTISAMPLE, UniformValue::INT_1 },
      { GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, UniformValue::INT_1 },
      { GL_INT_SAMPLER_3D, UniformValue::INT_1 },
      { GL_INT_SAMPLER_BUFFER, UniformValue::INT_1 },
      { GL_INT_SAMPLER_CUBE, UniformValue::INT_1 },
      { GL_UNSIGNED_INT_SAMPLER_1D, UniformValue::INT_1 },
      { GL_UNSIGNED_INT_SAMPLER_1D_ARRAY, UniformValue::INT_1 },
      { GL_UNSIGNED_INT_SAMPLER_2D, UniformValue::INT_1 },
      { GL_UNSIGNED_INT_SAMPLER_2D_ARRAY, UniformValue::INT_1 },
      { GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE, UniformValue::INT_1 },
      { GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, UniformValue::INT_1 },
      { GL_UNSIGNED_INT_SAMPLER_2D_RECT, UniformValue::INT_1 },
      { GL_UNSIGNED_INT_SAMPLER_3D, UniformValue::INT_1 },
      { GL_UNSIGNED_INT_SAMPLER_BUFFER, UniformValue::INT_1 },
      { GL_UNSIGNED_INT_SAMPLER_CUBE, UniformValue::INT_1 },
    };

    const TypeTable tbl_v[] = {
      { GL_FLOAT, UniformValue::FLOAT_1_V },
      { GL_FLOAT_VEC2, UniformValue::FLOAT_2_V },
      { GL_FLOAT_VEC3, UniformValue::FLOAT_3_V },
      { GL_FLOAT_VEC4, UniformValue::FLOAT_4_V },
      { GL_INT, UniformValue::INT_1_V },
      { GL_INT_VEC2, UniformValue::INT_2_V },
      { GL_INT_VEC3, UniformValue::INT_3_V },
      { GL_INT_VEC4, UniformValue::INT_4_V },
      { GL_UNSIGNED_INT, UniformValue::UINT_1_V },
      { GL_UNSIGNED_INT_VEC2, UniformValue::UINT_2_V },
      { GL_UNSIGNED_INT_VEC3, UniformValue::UINT_3_V },
      { GL_UNSIGNED_INT_VEC4, UniformValue::UINT_4_V },
      { GL_BOOL, UniformValue::UINT_1_V },
      { GL_BOOL_VEC2, UniformValue::UINT_2_V },
      { GL_FLOAT_MAT2, UniformValue::MATRIX_2_V },
      { GL_FLOAT_MAT2x3, UniformValue::MATRIX_2x3_V },
      { GL_FLOAT_MAT2x4, UniformValue::MATRIX_2x4_V },
      { GL_FLOAT_MAT3, UniformValue::MATRIX_3_V },
      { GL_FLOAT_MAT3x2, UniformValue::MATRIX_3x2_V },
      { GL_FLOAT_MAT3x4, UniformValue::MATRIX_3x4_V },
      { GL_FLOAT_MAT4, UniformValue::MATRIX_4_V },
      { GL_FLOAT_MAT4x2, UniformValue::MATRIX_4x2_V },
      { GL_FLOAT_MAT4x3, UniformValue::MATRIX_4x3_V },
      { GL_SAMPLER_1D, UniformValue::INT_1_V },
      { GL_SAMPLER_1D_ARRAY, UniformValue::INT_1_V },
      { GL_SAMPLER_1D_ARRAY_SHADOW, UniformValue::INT_1_V },
      { GL_SAMPLER_1D_SHADOW, UniformValue::INT_1_V },
      { GL_SAMPLER_2D, UniformValue::INT_1_V },
      { GL_SAMPLER_2D_ARRAY, UniformValue::INT_1_V },
      { GL_SAMPLER_2D_ARRAY_SHADOW, UniformValue::INT_1_V },
      { GL_SAMPLER_2D_MULTISAMPLE, UniformValue::INT_1_V },
      { GL_SAMPLER_2D_MULTISAMPLE_ARRAY, UniformValue::INT_1_V },
      { GL_SAMPLER_2D_RECT, UniformValue::INT_1_V },
      { GL_SAMPLER_2D_RECT_SHADOW, UniformValue::INT_1_V },
      { GL_SAMPLER_2D_SHADOW, UniformValue::INT_1_V },
      { GL_SAMPLER_3D, UniformValue::INT_1_V },
      { GL_SAMPLER_BUFFER, UniformValue::INT_1_V },
      { GL_SAMPLER_CUBE, UniformValue::INT_1_V },
      { GL_SAMPLER_CUBE_SHADOW, UniformValue::INT_1_V },
      { GL_INT_SAMPLER_1D, UniformValue::INT_1_V },
      { GL_INT_SAMPLER_1D_ARRAY, UniformValue::INT_1_V },
      { GL_INT_SAMPLER_2D, UniformValue::INT_1_V },
      { GL_INT_SAMPLER_2D_ARRAY, UniformValue::INT_1_V },
      { GL_INT_SAMPLER_2D_RECT, UniformValue::INT_1_V },
      { GL_INT_SAMPLER_2D_MULTISAMPLE, UniformValue::INT_1_V },
      { GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, UniformValue::INT_1_V },
      { GL_INT_SAMPLER_3D, UniformValue::INT_1_V },
      { GL_INT_SAMPLER_BUFFER, UniformValue::INT_1_V },
      { GL_INT_SAMPLER_CUBE, UniformValue::INT_1_V },
      { GL_UNSIGNED_INT_SAMPLER_1D, UniformValue::INT_1_V },
      { GL_UNSIGNED_INT_SAMPLER_1D_ARRAY, UniformValue::INT_1_V },
      { GL_UNSIGNED_INT_SAMPLER_2D, UniformValue::INT_1_V },
      { GL_UNSIGNED_INT_SAMPLER_2D_ARRAY, UniformValue::INT_1_V },
      { GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE, UniformValue::INT_1_V },
      { GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, UniformValue::INT_1_V },
      { GL_UNSIGNED_INT_SAMPLER_2D_RECT, UniformValue::INT_1_V },
      { GL_UNSIGNED_INT_SAMPLER_3D, UniformValue::INT_1_V },
      { GL_UNSIGNED_INT_SAMPLER_BUFFER, UniformValue::INT_1_V },
      { GL_UNSIGNED_INT_SAMPLER_CUBE, UniformValue::INT_1_V },
    };

    const ui32 num = sizeof(tbl) / sizeof(TypeTable);
    for(ui32 ui = 0; ui < num; ui++) {
      if(tbl[ui].gl == type) {
        if(size > 1)
          return tbl_v[ui].type;
        else
          return tbl[ui].type;
      }
    }
    NU_ASSERT(false, "Invalid GL type.\n");
    return UniformValue::FLOAT_1;
  };

  struct Viewport {
    GLint origin_x;
    GLint origin_y;
    GLsizei width;
    GLsizei height;

    Viewport()
        : origin_x(0),
          origin_y(0),
          width(0),
          height(0)
    {
      // None...
    }

    Viewport(const nuRect& rect) {
      origin_x = static_cast< GLint >(rect.origin().x());
      origin_y = static_cast< GLint >(rect.origin().y());
      width = static_cast< GLsizei >(rect.size().w());
      height = static_cast< GLsizei >(rect.size().h());
    }

    bool operator == (const Viewport& viewport) const {
      if(origin_x == viewport.origin_x && origin_y == viewport.origin_y &&
         width == viewport.width && height == viewport.height)
        return true;
      return false;
    }

    bool operator != (const Viewport& viewport) const {
      return !(*this == viewport);
    }

  };

  struct Scissor {
    bool enable;
    GLint left;
    GLint bottom;
    GLsizei width;
    GLsizei height;

    Scissor()
        : enable(false),
          left(0),
          bottom(0),
          width(0),
          height(0)
    {
      // None...
    }

    Scissor(bool en, const nuRect& rect) {
      enable = en;
      left = static_cast< GLint >(rect.origin().x());
      bottom = static_cast< GLint >(rect.origin().y());
      width = static_cast< GLsizei >(rect.size().w());
      height = static_cast< GLsizei >(rect.size().h());
    }

    bool operator == (const Scissor& scissor) const {
      if(left == scissor.left && bottom == scissor.bottom &&
         width == scissor.width && height == scissor.height &&
         enable == scissor.enable)
        return true;
      return false;
    }

    bool operator != (const Scissor& scissor) const {
      return !(*this == scissor);
    }

  };

  struct DepthTest {
    bool enable;
    nude::DEPTHSTENCIL_FUNC function;

    DepthTest()
        : enable(false),
          function(nude::DEPTHSTENCIL_ALWAYS)
    {
      // None...
    }

    DepthTest(bool en, nude::DEPTHSTENCIL_FUNC func)
        : enable(en),
          function(func)
    {
      // None...
    }

    bool operator == (const DepthTest& depth_test) const {
      if(enable == depth_test.enable && function == depth_test.function)
        return true;
      return false;
    }

    bool operator != (const DepthTest& depth_test) const {
      return !(*this == depth_test);
    }

  };

  struct StencilTest {
    bool enable;
    nude::DEPTHSTENCIL_FUNC function;
    GLint reference;
    GLuint mask;
    nude::STENCIL_OP stencil_fail;
    nude::STENCIL_OP depth_fail;
    nude::STENCIL_OP depth_pass;

    StencilTest()
        : enable(false),
          function(nude::DEPTHSTENCIL_ALWAYS),
          reference(0),
          mask(0),
          stencil_fail(nude::STENCIL_KEEP),
          depth_fail(nude::STENCIL_KEEP),
          depth_pass(nude::STENCIL_KEEP)
    {
      // None...
    }

    StencilTest(bool en,
                nude::DEPTHSTENCIL_FUNC func,
                GLint ref,
                GLuint smask,
                nude::STENCIL_OP sfail,
                nude::STENCIL_OP zfail,
                nude::STENCIL_OP zpass)
        : enable(en),
          function(func),
          reference(ref),
          mask(smask),
          stencil_fail(sfail),
          depth_fail(zfail),
          depth_pass(zpass)
    {
      // None...
    }

    bool operator == (const StencilTest& stencil_test) const {
      if(enable == stencil_test.enable &&
         function == stencil_test.function &&
         reference == stencil_test.reference &&
         mask == stencil_test.mask &&
         stencil_fail == stencil_test.stencil_fail &&
         depth_fail == stencil_test.depth_fail &&
         depth_pass == stencil_test.depth_pass)
        return true;
      return false;
    }

    bool operator != (const StencilTest& stencil_test) const {
      return !(*this == stencil_test);
    }

  };

  struct Blending {
    bool enable;
    nude::BLEND_EQ_FUNC equation;
    nude::BLEND_FUNC source;
    nude::BLEND_FUNC destination;
    ui32 color;

    Blending()
        : enable(false),
          equation(nude::BLEND_EQ_ADD),
          source(nude::BLEND_SRC_ALPHA),
          destination(nude::BLEND_ONE_MINUS_SRC_ALPHA),
          color(0)
    {
      // None...
    }

    Blending(bool en,
             nude::BLEND_EQ_FUNC eq,
             nude::BLEND_FUNC src,
             nude::BLEND_FUNC dest,
             ui32 clr)
        : enable(en),
          equation(eq),
          source(src),
          destination(dest),
          color(clr)
    {
      // None...
    }

    bool operator == (const Blending& blending) const {
      if(enable == blending.enable &&
         equation == blending.equation &&
         source == blending.source &&
         destination == blending.destination &&
         color == blending.color)
        return true;
      return false;
    }

    bool operator != (const Blending& blending) const {
      return !(*this == blending);
    }

  };

  struct ProgramObject {
    nuShaderProgram* p_shader_program;
    UniformValue* p_value;
    ui32 uniform_block;
    UniformBlock* p_block;
  };

  struct Rasterizer {
    bool culling;
    bool line_smooth;
    f32 line_width;
    nude::CULL_FACE cull_face;
    nude::FRONT_FACE front_face;

    Rasterizer()
        : culling(false),
          line_smooth(false),
          line_width(1.0f),
          cull_face(nude::CULL_BACK),
          front_face(nude::FRONT_COUNTER_CLOCKWISE)
    {
      // None...
    }
  };

  struct FragmentOps {
    const Scissor* p_scissor;
    const DepthTest* p_depth_test;
    const StencilTest* p_stencil_test;
    const Blending* p_blending;
  };

  struct TextureEntity {
    nuTexture* p_texture;
    nuTexture::Parameter* p_parameter;
  };

  template< class T >
  struct DrawCmd {
    TYPE type;
    Viewport* p_viewport;
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
    FragmentOps fragment_ops;
    Rasterizer* p_rasterizer;
    nuVertexArray* p_vertex_array;
    nuVertexBuffer* p_vertex_buffer;
    nuElementBuffer* p_element_buffer;
    nuVertexArray::Array* p_immediate_array;
    TextureEntity* p_texture;

    ui32 primitive_mode: 4;
    ui32 element_num: 28;
    ui32 immediate_num: 8;
    ui32 texture_num: 6;
    ui32 reserved: 18;
  };
  typedef DrawCmd< DrawElements > DrawElementsCmd;

  static const ui32 EXPAND_TEMP_TAG_NUM = 256;
  static const ui32 EXPAND_UNIFORM = 64;
  static const ui32 EXPAND_TEXTURE = 16;

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

  UniformBlock* mpUniformBlock;
  ui32 mUniformBlockNum;
  ui32 mCurrentUniformBlock;

  TextureEntity* mpTextureTable;
  ui32 mTextureTableNum;
  ui32 mCurrentTextureTable;

  union {
    ui32 mAttributes;
    struct {
      ui32 mViewportChanged: 1;
      ui32 mScissorChanged: 1;
      ui32 mDepthTestChanged: 1;
      ui32 mStencilTestChanged: 1;
      ui32 mBlendingChanged: 1;
      ui32 mRasterizerChanged: 1;
      ui32 mReserved: 26;
    };
  };

  Viewport mCurrentViewport;
  Viewport* mpViewport;

  Scissor mCurrentScissor;
  Scissor* mpScissor;

  DepthTest mCurrentDepthTest;
  DepthTest* mpDepthTest;

  StencilTest mCurrentStencilTest;
  StencilTest* mpStencilTest;

  Blending mCurrentBlending;
  Blending* mpBlending;

  Rasterizer mCurrentRasterizer;
  Rasterizer* mpRasterizer;

  nuGContext();

  void sortTag(void);

  Tag* getTags(void) const {
    return mpTag;
  }

  ui32 getTagNum(void) const {
    return mCurrentTag;
  }

  void setUniform(ui32 index, UniformValue::TYPE type, GLint size, bool transpose, const void* p_data);
  void setProgramObject(ProgramObject& po);
  Viewport* getViewport(void);
  void initializeFragmentOps(FragmentOps& fragment_ops);
  Rasterizer* getRasterizer(void);

  ui32 registerTexture(nuTexture* p_texture, nuTexture::Parameter* p_parameter);

public:
  nuGContext(nuGContextBuffer& ctx_buffer);
  ~nuGContext();

  void begin(i64 frame_id, Tag* p_tag, ui32 tag_num);
  void end(void);

  void setPriority(nude::PASS pass, ui32 priority) {
    mCurrentPriority.pass = pass;
    mCurrentPriority.priority = priority;
  }

  void beginDraw(const nude::ShaderProgram& program);
  void endDraw(void);

  void setClearColor(const nuColor& color);
  void setClearDepth(f32 depth);
  void clear(ui32 clear_bit);

  void clear(ui32 clear_bit, const nuColor& color, f32 depth);

  void setVertexArray(nude::VertexArray& array);
  void setVertexBuffer(nude::VertexBuffer& buffer);
  void setElementBuffer(nude::ElementBuffer& buffer);
  void drawElements(nude::PRIMITIVE_MODE primitive_mode, ui32 element_num);

  void setUniform(ui32 index, void* p_data);
  void setUniformMatrix(ui32 index, bool transpose, void* p_data);
  void setUniformBlock(ui32 index, nude::UniformBuffer& buffer);

  void setViewport(const nuRect& rect);

  void setScissor(bool enable, const nuRect& rect);
  void setDepthTest(bool enable, nude::DEPTHSTENCIL_FUNC func);
  void setStencilTest(bool enable, nude::DEPTHSTENCIL_FUNC func);
  void setStencilOp(nude::STENCIL_OP stencil_fail,
                    nude::STENCIL_OP depth_fail,
                    nude::STENCIL_OP depth_pass);
  void setBlending(bool enable,
                   nude::BLEND_EQ_FUNC equation,
                   nude::BLEND_FUNC source,
                   nude::BLEND_FUNC destination);
  void setBlendColor(const nuColor& color);

  ArrayDeclaration declareArray(ui32 array_num);

  void setLineDraw(bool smooth, f32 width);
  void setFrontFace(nude::FRONT_FACE front_face);
  void setCulling(bool enable, nude::CULL_FACE cull_face);

  ui32 setTexture(nude::Texture& texture);
  ui32 setTexture(nude::Texture& texture, const nuTexture::Parameter& parameter);

};

#endif
