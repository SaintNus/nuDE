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
};


nuDEEntityTest::nuDEEntityTest()
    : mPosX(0.0f),
      mDir(1.0f),
      mInit(false)
{
  nuApplication::resourceManager().createResource("res://Resources/resource_test.rtst");

  mShaderProgram = nuApplication::renderGL().createShaderProgram(nude::Program_Debug);
  mVertexBuffer = nuApplication::renderGL().createVertexBuffer(sizeof(Vertex) * 3,
                                                               nuGResource::DYNAMIC_RESOURCE);
  mVertexBuffer->initialize();

  mVertexArray = nuApplication::renderGL().createVertexArray();
  mVertexArray->begin(nude::DebugAttribute_Num);
  mVertexArray->declare(nude::Debug_inPosition,
                        nuVertexArray::Array(3, nuVertexArray::FLOAT, false, sizeof(Vertex), 0));
  mVertexArray->declare(nude::Debug_inColor,
                        nuVertexArray::Array(4, nuVertexArray::UNSIGNED_INT_8, true, sizeof(Vertex), sizeof(f32) * 3));
  mVertexArray->end();

  ui16 idx[3] = { 0, 1, 2 };
  mElementBuffer = nuApplication::renderGL().createElementBuffer(nuElementBuffer::UNSIGNED_INT_16,
                                                                 3,
                                                                 nuGResource::nuGResource::STATIC_RESOURCE);
  void* p_buffer = mElementBuffer->beginInitialize();
  memcpy(p_buffer, idx, sizeof(idx));
  mElementBuffer->endInitialize();

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
}

nuDEEntityTest::~nuDEEntityTest()
{
  mVertexBuffer.release();
  mElementBuffer.release();
  mVertexArray.release();
}

void nuDEEntityTest::setup(nuGSetupContext& setup)
{
  if(mVertexBuffer.isValid())
    setup.map(*mVertexBuffer);
}

void nuDEEntityTest::update(void)
{
  Vertex vtx[3] = {
    {
      { mPosX, 0.6f, 0.0f },
      // 0xff0000ff,
      0xffffffff,
    },
    {
      { -0.2f, -0.3f, 0.0f },
      // 0xff00ff00,
      0xffffffff,
    },
    {
      { 0.2f, -0.3f, 0.0f },
      // 0xffff0000,
      0xffffffff,
    },
  };

  if(mVertexBuffer.isValid()) {
    void* p_buffer = mVertexBuffer->getBuffer();
    if(p_buffer)
      memcpy(p_buffer, vtx, sizeof(vtx));
  }

  mPosX += 0.01f * mDir * nuApplication::instance()->getFrameTime();
  if(fabsf(mPosX) > 0.2f) {
    mPosX = mDir * 0.2f;
    mDir *= -1.0f;
  }
}

void nuDEEntityTest::draw(nuGContext& context)
{
  context.setPriority(nude::PASS_OPAQUE, 0);
  context.clear(nuGContext::CLEAR_COLOR | nuGContext::CLEAR_DEPTH, nuColor(0), 1.0f);

  context.beginDraw(mShaderProgram);
  {
    f32 vv[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

    f32 vvv[3][4] = {
      { 1.0f, 0.0f, 0.0f, 1.0f },
      { 0.0f, 1.0f, 0.0f, 1.0f },
      { 0.0f, 0.0f, 1.0f, 1.0f },
    };

    context.setPriority(nude::PASS_TRANSPARENCY, 0);
    if(!mInit) {
      context.setUniform(nude::Debug_uniTest, vv);
      context.setUniform(nude::Debug_uniColor_0, vvv);
      mInit = true;
    }
    context.setVertexArray(mVertexArray);
    context.setVertexBuffer(mVertexBuffer);
    context.setElementBuffer(mElementBuffer);
    context.drawElements(nude::TRIANGLES, 3);
  }
  context.endDraw();
}
