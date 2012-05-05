/*!
 * \file nuApplication.cpp
 * \brief Application class
 * \author Nus
 * \date 2011/11/03 21:53
 */

#include "nuApplication.h"

IMPLEMENT_TYPE_INFO(nuApplication, nuObject);

nuApplication::nuApplication()
    : mpAppMain(nullptr),
      mState(UNINITIALIZED),
      mFrameTime(1.0f)
{
  // None...
}

nuApplication::~nuApplication()
{
  NU_ASSERT(mState != INITIALIZED, "Unterminated application instance.\n");
  if(mpAppMain) {
    delete mpAppMain;
    mpAppMain = nullptr;
  }
}

void nuApplication::initialize(const nuTypeInfo &app_main)
{
  if(mState != UNINITIALIZED)
    return;

  if(!app_main.isDerivedFrom(nuAppMain::TypeInfo())) {
    mpAppMain = nullptr;
    NU_ASSERT(false, "Invalid main class.");
  } else {
    mpAppMain = static_cast< nuAppMain* >(app_main.createInstance());
    mpAppMain->initialize();
    mState = INITIALIZED;
  }
}

void nuApplication::terminate(void)
{
  if(mState != INITIALIZED)
    return;

  if(mpAppMain) {
    mpAppMain->terminate();
    mpAppMain->waitForTermination();
    mState = TERMINATED;
  }
}
