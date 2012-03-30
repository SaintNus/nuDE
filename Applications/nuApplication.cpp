/*!
 * \file nuApplication.cpp
 * \brief Application class
 * \author Nus
 * \date 2011/11/03 21:53
 */

#include "nuApplication.h"

IMPLEMENT_TYPE_INFO(nuApplication, nuObject);

nuApplication::nuApplication()
    : mpAppMain(nullptr)
{
  // None...
}

nuApplication::~nuApplication()
{
  terminate();
  if(mpAppMain) {
    delete mpAppMain;
    mpAppMain = nullptr;
  }
}

void nuApplication::initialize(const nuTypeInfo &app_main)
{
  if(!app_main.isDerivedFrom(nuAppMain::TypeInfo())) {
    mpAppMain = nullptr;
    NU_ASSERT(false, "Invalid main class.");
  } else {
    mpAppMain = static_cast< nuAppMain* >(app_main.createInstance());
    mpAppMain->initialize();
  }
}

void nuApplication::terminate(void)
{
  if(mpAppMain)
    mpAppMain->terminate();
}
