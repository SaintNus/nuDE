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
  DECLARE_TYPE_INFO;

public:
  static nuApplication* createApplication(const nuTypeInfo& main) {
    if(!mpInstance)
      mpInstance = new nuApplication(main);
    return mpInstance;
  }

  static void deleteApplication(void) {
    if(mpInstance) {
      delete mpInstance;
      mpInstance = nullptr;
    }
  }

  static nuApplication* getCurrent(void) {
    return mpInstance;
  }

  nuAppMain* getAppMain(void) {
    return mpAppMain;
  }

  i32 run(void) {
    if(mpAppMain)
      return mpAppMain->main();
    return -1;
  }

private:
  static nuApplication* mpInstance;

  nuAppMain* mpAppMain;

  nuApplication();
  nuApplication(const nuTypeInfo& type);
  ~nuApplication();

};

#endif
