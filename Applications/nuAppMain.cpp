/*!
 * \file nuAppMain.cpp
 * \brief Application main class.
 * \author Nus
 * \date 2011/11/03 22:13
 */

#include "nuAppMain.h"

IMPLEMENT_TYPE_INFO(nuAppMain, nuObject);

nuAppMain::nuAppMain()
    : mpRenderGL(nullptr),
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
  if(mpRenderGL) {
    nuRenderGL* ptr = mpRenderGL;
    mpRenderGL = nullptr;
    delete ptr;
  }
}

i32 nuAppMain::main(void)
{
  mState = RUNNING;
  while(mState != TERMINATING) {
    NU_TRACE("Main loop is running...\n");
    nuThread::sleep(1);
  }
  NU_TRACE("Main loop is terminated...\n");
  mState = READY;
  return 0;
}

nuRenderGL* nuAppMain::createRenderGL(void) const
{
  return new nuRenderGL;
}

void nuAppMain::initialize(void)
{
  NU_ASSERT_C(mpRenderGL == NULL);
  mpRenderGL = createRenderGL();
  mState = READY;
}

void nuAppMain::terminate(void)
{
  NU_ASSERT_C(mState == RUNNING);
  if(mState == RUNNING) {
    mState = TERMINATING;
  }
}
