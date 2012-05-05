/*!
 * \file nuDEEntityTest.cpp
 * \brief Entity test.
 * \author Nus
 * \date 2012/05/01 23:08
 */

#include "nuDEEntityTest.h"

IMPLEMENT_TYPE_INFO(nuDEEntityTest, nuEntity);

struct Vertex {
  f32 pos[3];
  ui32 abgr;
};


nuDEEntityTest::nuDEEntityTest()
    : mPosX(0.0f),
      mDir(1.0f)
{
  mVertexBuffer = nuApplication::renderGL().createVertexBuffer(sizeof(Vertex) * 3,
                                                               nuGResource::DYNAMIC_RESOURCE);
  mVertexBuffer->initialize();

  mVertexArray = nuApplication::renderGL().createVertexArray();
  mVertexArray->beginDeclaration(2);
  mVertexArray->declare(0, nuVertexArray::Array(3, nuVertexArray::FLOAT, false, sizeof(Vertex), 0));
  mVertexArray->declare(1, nuVertexArray::Array(4, nuVertexArray::UNSIGNED_INT_8, true, sizeof(Vertex), sizeof(f32) * 3));
  mVertexArray->endDeclaration();

  ui16 idx[3] = { 0, 1, 2 };
  mElementBuffer = nuApplication::renderGL().createElementBuffer(nuElementBuffer::UNSIGNED_INT_16,
                                                                 3,
                                                                 nuGResource::nuGResource::STATIC_RESOURCE);
  void* p_buffer = mElementBuffer->beginInitialize();
  memcpy(p_buffer, idx, sizeof(idx));
  mElementBuffer->endInitialize();
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
      0xff0000ff,
    },
    {
      { -0.2f, -0.3f, 0.0f },
      0xff00ff00,
    },
    {
      { 0.2f, -0.3f, 0.0f },
      0xffff0000,
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
  context.setPriority(nude::PASS_TRANSPARENCY, 0);

  context.clear(nuGContext::CLEAR_COLOR | nuGContext::CLEAR_DEPTH, nuColor(0), 1.0f);

  context.setVertexArray(mVertexArray);
  context.setVertexBuffer(mVertexBuffer);
  context.setElementBuffer(mElementBuffer);
  context.drawElements(nude::TRIANGLES, 3);
}
