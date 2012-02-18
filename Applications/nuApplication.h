/*!
 * \file nuApplication.h
 * \brief Application class
 * \author Nus
 * \date 2011/11/03 21:53
 */

#ifndef __NUAPPLICATION_H__
#define __NUAPPLICATION_H__

#include "nuAppMain.h"

/*!
 * \class nuApplication
 * \brief Application class.
 */
class nuApplication : public nuObject
{
  DECLARE_SINGLETON(nuApplication);
  DECLARE_TYPE_INFO;

public:
  static nuAppMain* appMain(void) {
    return INST(nuApplication)->mpAppMain;
  }

  static nuRenderGL& renderGL(void) {
    NU_ASSERT_C(appMain() != NULL);
    return appMain()->getRenderGL();
  }

  static nuThreadPool& threadPool(void) {
    NU_ASSERT_C(appMain() != NULL);
    return appMain()->getThreadPool();
  }

  void initialize(const nuTypeInfo& app_main);
  i32 run(void) {
    if(mpAppMain)
      return mpAppMain->main();
    return -1;
  }

private:
  nuAppMain* mpAppMain;

  nuApplication();
  ~nuApplication();

  void terminate(void);
};

#endif
