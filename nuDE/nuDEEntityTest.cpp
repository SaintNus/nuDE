/*!
 * \file nuDEEntityTest.cpp
 * \brief Entity test.
 * \author Nus
 * \date 2012/05/01 23:08
 */

#include "nuDEEntityTest.h"

IMPLEMENT_TYPE_INFO(nuDEEntityTest, nuEntity);

nuDEEntityTest::nuDEEntityTest()
    : mPosX(0.0f),
      mDir(1.0f)
{

}

nuDEEntityTest::~nuDEEntityTest()
{

}

void nuDEEntityTest::setup(nuGSetupContext& setup)
{
  if(nuApplication::instance()->renderGL().getTestVB().isValid())
    setup.map(*nuApplication::instance()->renderGL().getTestVB());
}

void nuDEEntityTest::update(void)
{
  struct Vertex {
    f32 pos[3];
    f32 color[4];
  };

  Vertex vtx[3] = {
    {
      { mPosX, 0.6f, 0.0f },
      { 1.0f, 0.85f, 0.35f, 1.0f }
    },
    {
      { -0.2f, -0.3f, 0.0f },
      { 1.0f, 0.85f, 0.35f, 1.0f }
    },
    {
      { 0.2f, -0.3f, 0.0f },
      { 1.0f, 0.85f, 0.35f, 1.0f }
    },
  };

  if(nuApplication::instance()->renderGL().getTestVB().isValid()) {
    void* p_buffer = nuApplication::instance()->renderGL().getTestVB()->getBuffer();
    if(p_buffer)
      memcpy(p_buffer, vtx, sizeof(Vertex));
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
}
