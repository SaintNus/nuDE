/*!
 * \file nuAppMain.cpp
 * \brief Application main class.
 * \author Nus
 * \date 2011/11/03 22:13
 */

#include "nuEntityManager.h"
#include "nuAppMain.h"

IMPLEMENT_TYPE_INFO(nuAppMain, nuObject);

nuAppMain::nuAppMain()
    : mpRenderGL(nullptr),
      mpThreadPool(nullptr),
      mpEntityManager(nullptr),
      mState(UNINITIALIZED)
{
  // None...
}

nuAppMain::~nuAppMain()
{
  if(mState == RUNNING || mState == TERMINATING) {
    while(mState != READY)
      nuThread::usleep(16000);
  }

  if(mpEntityManager) {
    nuEntityManager* ptr = mpEntityManager;
    mpEntityManager = nullptr;
    delete ptr;
  }

  if(mpRenderGL) {
    nuRenderGL* ptr = mpRenderGL;
    mpRenderGL = nullptr;
    delete ptr;
  }

  if(mpThreadPool) {
    nuThreadPool* ptr = mpThreadPool;
    mpThreadPool = nullptr;
    delete ptr;
  }
}

i32 nuAppMain::main(void)
{
  mState = RUNNING;
  while(mState != TERMINATING) {
    nuAutoReleasePool pool;
    update();
  }
  NU_TRACE("Main loop is terminated...\n");
  mState = READY;
  return 0;
}

void nuAppMain::initialize(void)
{
  NU_ASSERT_C(mpRenderGL == nullptr);
  NU_ASSERT_C(mpThreadPool == nullptr);

  mpRenderGL = new nuRenderGL;
  mpThreadPool = new nuThreadPool;
  mpEntityManager = new nuEntityManager;
  
  mState = READY;
}

void nuAppMain::terminate(void)
{
  NU_ASSERT_C(mState == RUNNING);
  if(mState == RUNNING) {
    mState = TERMINATING;
  }
}
