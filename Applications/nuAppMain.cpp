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
      mRingBufferSize(DEFAULT_RING_BUFFER_SIZE),
      mpTag(nullptr),
      mTagNum(DEFAULT_TAG_NUM),
      mFrameID(0)
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

  if(mpTag) {
    nuGContext::Tag* ptr = mpTag;
    mpTag = nullptr;
    delete[] ptr;
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

  NU_TRACE("Main loop is up and running.\n");
  begin();

  while(mState != TERMINATING) {
    nuAutoReleasePool pool;
    update();
    draw();
  }

  end();
  NU_TRACE("Main loop is terminated.\n");

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
  
  mpTag = new nuGContext::Tag[mTagNum];

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
  mFrameID = mpRenderGL->synchronize();

  if(mpEntityManager->getEntityNum()) {
    nuTaskSet update_set(mpEntityManager->getEntityNum());
    mpEntityManager->createUpdateList(update_set);
    if(update_set.getTaskNum() > 0) {
      nuThreadPool::JobTicket ticket = mpThreadPool->entryJob(update_set);
      mpThreadPool->waitUntilFinished(ticket);
    }
  }
}

void nuAppMain::draw(void)
{
  ui32 tag_num = mTagNum / nuThreadPool::MAX_WORKER;

  for(ui32 ui = 0; ui < nuThreadPool::MAX_WORKER; ui++) {
    mpGraphicContext[ui]->begin(mFrameID, &mpTag[ui * tag_num], tag_num);
  }

  {
    nuTaskSet draw(nuThreadPool::MAX_WORKER);
    for(ui32 ui = 0; ui < nuThreadPool::MAX_WORKER; ui++) {
      draw.addTask(nuTask(this,
                          static_cast< nuFunction >(&nuAppMain::executeDraw),
                          mpGraphicContext[ui]));
    }

    {
      nuThreadPool::JobTicket ticket = mpThreadPool->entryJob(draw);
      mpThreadPool->waitUntilFinished(ticket);
    }
  }
}

void nuAppMain::executeDraw(void* param)
{
  nuGContext* p_ctx = static_cast< nuGContext* >(param);
  p_ctx->end();
}
