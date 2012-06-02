/*!
 * \file nuDEEntityTest.cpp
 * \brief Entity test.
 * \author Nus
 * \date 2012/05/01 23:08
 */

#include "nuDEInclude.h"
#include "nuDEEntityTest.h"

class ResourceTest : public nuResource
{
  DECLARE_TYPE_INFO;

  ResourceTest() {}
  ~ResourceTest() {}

  ccstr getExt(void) const {
    return "rtst";
  }
  ERROR_CODE setup(const nuStream& stream) {
    size_t sz = stream.getSize();
    void* p_data = nude::Alloc(sz);

    if(!p_data) {
      nude::Dealloc(p_data);
      return ERROR_INSUFFICIENT_MEMORY;
    }

    if(stream.read(p_data, sz) != sz) {
      nude::Dealloc(p_data);
      return ERROR_CORRUPTED;
    }

    nude::Dealloc(p_data);
    return ERROR_NONE;
  }

};

IMPLEMENT_TYPE_INFO(ResourceTest, nuResource);

IMPLEMENT_TYPE_INFO(nuDEEntityTest, nuEntity);

struct Vertex {
  f32 pos[3];
  ui32 abgr;
  f32 uv[2];
};


nuDEEntityTest::nuDEEntityTest()
    : mPosX(-0.2f),
      mDir(1.0f),
      mInit(false),
      mIdx(0),
      mVBIdx(0)
{
  nuApplication::resourceManager().createResource("res://Resources/resource_test.rtst");

  mShaderProgram = nuApplication::renderGL().createShaderProgram(nude::Program_Debug);
  mUniformBuffer = nuApplication::renderGL().createUniformBuffer(nude::Program_Debug,
                                                                 nude::Debug_uniColorXform);
  f32 ubo[4] = { 1.0f, 0.0f, 1.0f, 0.0f };
  mUniformBuffer->beginInitialize();
  {
    void* p_buffer = mUniformBuffer->getBuffer();
    memcpy(p_buffer, ubo, sizeof(ubo));
  }
  mUniformBuffer->endInitialize();

  mVertexBuffer[0] = nuApplication::renderGL().createVertexBuffer(sizeof(Vertex) * 4,
                                                                  nuGResource::DYNAMIC_RESOURCE);
  mVertexBuffer[0]->initialize();
  mVertexBuffer[1] = nuApplication::renderGL().createVertexBuffer(sizeof(Vertex) * 4,
                                                                  nuGResource::DYNAMIC_RESOURCE);
  mVertexBuffer[1]->initialize();

  mVertexArray = nuApplication::renderGL().createVertexArray();
  mVertexArray->begin(nude::DebugAttribute_Num);
  mVertexArray->declare(nude::Debug_inPosition,
                        nuVertexArray::Array(3, nuVertexArray::FLOAT, false, sizeof(Vertex), 0));
  mVertexArray->declare(nude::Debug_inColor,
                        nuVertexArray::Array(4, nuVertexArray::UNSIGNED_INT_8, true, sizeof(Vertex), sizeof(f32) * 3));
  mVertexArray->declare(nude::Debug_inUV,
                        nuVertexArray::Array(2, nuVertexArray::FLOAT, false, sizeof(Vertex), sizeof(f32) * 3 + sizeof(ui32)));
  mVertexArray->end();

  ui16 idx[4] = { 0, 1, 2, 3 };
  mElementBuffer = nuApplication::renderGL().createElementBuffer(nuElementBuffer::UNSIGNED_INT_16,
                                                                 4,
                                                                 nuGResource::nuGResource::STATIC_RESOURCE);
  {
    void* p_buffer = mElementBuffer->beginInitialize();
    memcpy(p_buffer, idx, sizeof(idx));
    mElementBuffer->endInitialize();
  }

  ui32 texture[2][2] = {
    { 0xffff0000, 0xff00ff00 },
    { 0xff0000ff, 0xffffff00 },
  };

  mTexture = nuApplication::renderGL().createTexture(nuGResource::nuGResource::STATIC_RESOURCE);
  {
    nuTexture::Texture2D tex_2d(64, 64, nude::PIXEL_OPTIMAL_HIGH_PRECISION, true, true);
    nuTexture::Parameter param(nude::WRAP_CLAMP_TO_EDGE,
                               nude::WRAP_CLAMP_TO_EDGE,
                               nude::WRAP_CLAMP_TO_EDGE,
                               nude::FILTER_NEAREST_MIPMAP_NEAREST,
                               nude::FILTER_NEAREST);
    ui32* p_buffer = static_cast< ui32* >(mTexture->beginInitialize(tex_2d, param));
    for(ui32 ui = 0; ui < 64; ui++) {
      for(ui32 uj = 0; uj < 64; uj++) {
        ui32 ii = ui / 32;
        ui32 jj = uj / 32;
        *p_buffer = texture[ii][jj];
        p_buffer++;
      }
    }
    mTexture->endInitialize();
  }

  {
    ccstr str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    nuStream test;
    void* mem = test.create(strlen(str));
    memcpy(mem, str, strlen(str));
    test.seekBegin(8);
    test.seekEnd(8);
    test.rewind();
    {
      ui8 buffer[4];
      test.read< ui8 >(buffer, 4);
    }
  }

  mDrawString = nuDrawString::create("ヌス", nuSize(256.0f, 0.0f));
  mDrawString->setMargin(nuSize(0.0f, 0.0f));
}

nuDEEntityTest::~nuDEEntityTest()
{
  // None...
}

void nuDEEntityTest::setup(nuGSetupContext& setup)
{
  mVBIdx ^= 1;
  if(mVertexBuffer[mVBIdx].isValid())
    setup.map(*mVertexBuffer[mVBIdx]);
  if(mUniformBuffer.isValid())
    setup.map(*mUniformBuffer);
}

void nuDEEntityTest::update(void)
{
  Vertex vtx[4] = {
    {
      { mPosX, 0.3f, 0.0f },
      // 0xff0000ff,
      0xffffffff,
      { 0.0f, 1.0f },
    },
    {
      { -0.2f, -0.3f, 0.0f },
      // 0xff00ff00,
      0xffffffff,
      { 0.0f, 0.0f },
    },
    {
      { mPosX + 0.4f, 0.3f, 0.0f },
      // 0xff0000ff,
      0xffffffff,
      { 1.0f, 1.0f },
    },
    {
      { 0.2f, -0.3f, 0.0f },
      // 0xffff0000,
      0xffffffff,
      { 1.0f, 0.0f },
    },
  };

  if(mVertexBuffer[mVBIdx].isValid()) {
    void* p_buffer = mVertexBuffer[mVBIdx]->getBuffer();
    if(p_buffer)
      memcpy(p_buffer, vtx, sizeof(vtx));
  }

  if(mUniformBuffer.isValid()) {
    f32 adder[4][4] = {
      { 1.0f, 0.0f, 0.0f, 0.0f },
      { 0.0f, 1.0f, 0.0f, 0.0f },
      { 0.0f, 0.0f, 1.0f, 0.0f },
      { 1.0f, 1.0f, 1.0f, 1.0f },
    };

    ui32 test = 3;
    void* p_buffer = mUniformBuffer->getBuffer();
    if(p_buffer)
      memcpy(p_buffer, adder[test], sizeof(f32) * 4);

    mIdx++;
    mIdx %= 400;
  }

  mPosX += 0.01f * mDir * nuApplication::instance()->getFrameTime();
  if(fabsf(mPosX + 0.2f) > 0.2f) {
    mPosX = -0.2f + mDir * 0.2f;
    mDir *= -1.0f;
    if(mDir > 0.0f)
      mDrawString->setString("竹達彩奈");
    else
      mDrawString->setString("ヌス");
  }
}

void nuDEEntityTest::draw(nuGContext& context)
{
  context.setPriority(nude::PASS_OPAQUE, 0);
  context.clear(nude::CLEAR_COLOR | nude::CLEAR_DEPTH, nuColor::Black, 1.0f);

  context.beginDraw(mShaderProgram);
  {
    f32 vv[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    f32 vvv[3][4] = {
      { 0.1f, 0.0f, 0.0f, 1.0f },
      { 0.0f, 0.1f, 0.1f, 1.0f },
      { 0.0f, 0.0f, 0.0f, 1.0f },
    };

    context.setPriority(nude::PASS_TRANSPARENCY, 0);
    ui32 tex = context.setTexture(mTexture);

    if(!mInit) {
      context.setUniform(nude::Debug_uniTest, vv);
      context.setUniform(nude::Debug_uniColor_0, vvv);
      context.setUniform(nude::Debug_uniTexture, &tex);
      context.setUniformBlock(nude::Debug_uniColorXform, mUniformBuffer);
      mInit = true;
    }
    context.setDepthTest(true, nude::DEPTHSTENCIL_LEQUAL);
    context.setBlending(true, nude::BLEND_EQ_ADD, nude::BLEND_SRC_ALPHA, nude::BLEND_ONE_MINUS_SRC_ALPHA);
    context.setVertexArray(mVertexArray);
    context.setVertexBuffer(mVertexBuffer[mVBIdx]);
    context.setElementBuffer(mElementBuffer);
    context.setFrontFace(nude::FRONT_COUNTER_CLOCKWISE);
    context.setCulling(true, nude::CULL_BACK);

    /*
    nuGContext::ArrayDeclaration decl = context.declareArray(nude::DebugAttribute_Num);
    decl.getDeclaration(nude::Debug_inPosition) =
        nuVertexArray::Array(3, nuVertexArray::FLOAT, false, sizeof(Vertex), 0);
    decl.getDeclaration(nude::Debug_inColor) =
        nuVertexArray::Array(4, nuVertexArray::UNSIGNED_INT_8, true, sizeof(Vertex), sizeof(f32) * 3);
     */

    context.drawElements(nude::TRIANGLE_STRIP, 4);
  }
  context.endDraw();

  mDrawString->drawAt(nuPoint(0, 0));
}
