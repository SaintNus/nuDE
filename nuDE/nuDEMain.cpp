/*!
 * \file nuDEMain.cpp
 * \brief Main loop.
 * \author Nus
 * \date 2012/03/11 00:28
 */

#include "nuDEInclude.h"
#include "nuDEMain.h"

IMPLEMENT_TYPE_INFO(nuDEMain, nuAppMain);

nuDEMain::nuDEMain()
    : mpTest(nullptr)
{
  // None...
}

nuDEMain::~nuDEMain()
{
  if(mpTest) {
    delete mpTest;
    mpTest = nullptr;
  }
}

void nuDEMain::update(void)
{
  nuAppMain::update();
}

void nuDEMain::begin(void)
{
  nuAppMain::begin();
  NU_ASSERT_C(mpTest == nullptr);
  mpTest = new nuDEEntityTest;
}

void nuDEMain::end(void)
{
  nuAppMain::end();
  if(mpTest) {
    delete mpTest;
    mpTest = nullptr;
  }
}
