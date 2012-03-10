/*!
 * \file nuDEMain.cpp
 * \brief Main loop.
 * \author Nus
 * \date 2012/03/11 00:28
 */

#include "nuDEMain.h"

IMPLEMENT_TYPE_INFO(nuDEMain, nuAppMain);

nuDEMain::nuDEMain()
{

}

nuDEMain::~nuDEMain()
{

}

void nuDEMain::initialize(void)
{
  nuAppMain::initialize();

  // Test file!
  {
    ccstr path = "home://Temp/テスト.cpp";
    nuFile file(nude::FATTR_READ, path);
    NU_TRACE("File size: %lu\n", file.getSize());
  }
}

void nuDEMain::update(void)
{
  nuAppMain::update();
  nuThread::usleep(1);
}

void nuDEMain::terminate(void)
{
  nuAppMain::terminate();
}
