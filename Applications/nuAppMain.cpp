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
      mpContextBuffer(nullptr),
      mState(UNINITIALIZED),
      mRingBufferSize(DEFAULT_RING_BUFFER_SIZE)
{
  for(ui32 ui = 0; ui < nuThreadPool::MAX_WORKER; ui++) {
    mpGraphicContext[ui] = nullptr;
  }
}

nuAppMain::~nuAppMain()
{
  waitForTermination();

  if(mpEntityManager) {
    nuEntityManager* ptr = mpEntityManager;
    mpEntityManager = nullptr;
    delete ptr;
  }

  for(ui32 ui = 0; ui < nuThreadPool::MAX_WORKER; ui++) {
    if(mpGraphicContext[ui]) {
      delete mpGraphicContext[ui];
      mpGraphicContext[ui] = nullptr;
    }
  }

  if(mpContextBuffer) {
    nuGContextBuffer* ptr = mpContextBuffer;
    mpContextBuffer = nullptr;
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
  mpRenderGL->acquire();

  while(mState != TERMINATING) {
    nuAutoReleasePool pool;
    update();
  }

  NU_TRACE("Main loop is terminated...\n");
  end();

  mpRenderGL->release();
  mState = READY;

  return 0;
}

void nuAppMain::initialize(void)
{
  NU_ASSERT_C(mState == UNINITIALIZED);

  mpRenderGL = new nuRenderGL;
  mpThreadPool = new nuThreadPool;
  mpEntityManager = new nuEntityManager;
  mpContextBuffer = new nuGContextBuffer(mRingBufferSize);

  for(ui32 ui = 0; ui < nuThreadPool::MAX_WORKER; ui++) {
    mpGraphicContext[ui] = new nuGContext(*mpContextBuffer);
  }
  
  mState = READY;
}

void nuAppMain::terminate(void)
{
  NU_ASSERT_C(mState == RUNNING);

  if(mState == RUNNING) {
    mState = TERMINATING;
  }
}

void nuAppMain::update(void)
{
  mpRenderGL->synchronize();
}
