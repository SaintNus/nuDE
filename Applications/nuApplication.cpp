/*!
 * \file nuApplication.cpp
 * \brief Application class
 * \author Nus
 * \date 2011/11/03 21:53
 */

#include "nuApplication.h"

IMPLEMENT_TYPE_INFO(nuApplication, nuObject);

nuApplication* nuApplication::mpInstance = nullptr;

nuApplication::nuApplication(const nuTypeInfo& type)
{
  if(!type.isDerivedFrom(nuAppMain::TypeInfo())) {
    mpAppMain = nullptr;
    NU_ASSERT(false, "Invalid main class.");
  } else {
    mpAppMain = static_cast< nuAppMain* >(type.createInstance());
  }
}

nuApplication::~nuApplication()
{
  if(mpAppMain) {
    delete mpAppMain;
    mpAppMain = nullptr;
  }
}
