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
    return nuApplication::instance()->mpAppMain;
  }

  static nuRenderGL& renderGL(void) {
    NU_ASSERT_C(appMain() != nullptr);
    return appMain()->getRenderGL();
  }

  static nuThreadPool& threadPool(void) {
    NU_ASSERT_C(appMain() != nullptr);
    return appMain()->getThreadPool();
  }

  static nuEntityManager& entityManager(void) {
    NU_ASSERT_C(appMain() != nullptr);
    return appMain()->getEntityManager();
  }

  static nuResourceManager& resourceManager(void) {
    NU_ASSERT_C(appMain() != nullptr);
    return appMain()->getResourceManager();
  }

  void initialize(const nuTypeInfo& app_main);

  i32 run(void) {
    if(mpAppMain)
      return mpAppMain->main();
    return -1;
  }

  void terminate(void);

  void setFrameTime(f32 frame_time) {
    mFrameTime = frame_time;
  }

  f32 getFrameTime(void) const {
    return mFrameTime;
  }

private:
  enum STATE {
    UNINITIALIZED = 0,
    INITIALIZED,
    TERMINATED,
  };

  nuAppMain* mpAppMain;
  STATE mState;
  f32 mFrameTime;

  nuApplication();
  ~nuApplication();

};

#endif
