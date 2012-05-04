/*!
 * \file nuDEEntityTest.cpp
 * \brief Entity test.
 * \author Nus
 * \date 2012/05/01 23:08
 */

#include "nuDEEntityTest.h"

IMPLEMENT_TYPE_INFO(nuDEEntityTest, nuEntity);

nuDEEntityTest::nuDEEntityTest()
{

}

nuDEEntityTest::~nuDEEntityTest()
{

}

void nuDEEntityTest::setup(nuGSetupContext& setup)
{

}

void nuDEEntityTest::update(void)
{

}

void nuDEEntityTest::draw(nuGContext& context)
{
  context.setPriority(nude::PASS_TRANSPARENCY, 0);
  context.clear(nuGContext::CLEAR_COLOR | nuGContext::CLEAR_DEPTH, nuColor(0), 1.0f);
}
