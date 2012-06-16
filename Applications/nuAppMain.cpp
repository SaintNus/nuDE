/*!
 * \file nuAppMain.cpp
 * \brief Application main class.
 * \author Nus
 * \date 2011/11/03 22:13
 */

#include "ApplicationsInclude.h"

#include "nuEntityManager.h"
#include "nuAppMain.h"

IMPLEMENT_TYPE_INFO(nuAppMain, nuObject);

nuAppMain::nuAppMain()
    : mpResourceManager(nullptr),
      mpRenderGL(nullptr),
      mpThreadPool(nullptr),
      mpEntityManager(nullptr),
      mpContextBuffer(nullptr),
      mState(UNINITIALIZED),
      mRingBufferSize(DEFAULT_RING_BUFFER_SIZE),
      mpTag(nullptr),
      mTagNum(DEFAULT_TAG_NUM),
      mFrameID(0),
      mCurrentTagList(0)
{
  for(ui32 ui = 0; ui < nuThreadPool::MAX_WORKER; ui++) {
    mpGraphicContext[ui] = nullptr;
  }

#if !NDEBUG
  mRenderPerformaceDispPos = nuPoint(10.0f, 10.0f);
  mRenderTimeAverage = 16.666f;
  mRenderTimeChanged = false;

  mUpdatePerformaceDispPos = nuPoint(10.0f, 25.0f);
  const f32 ftime = 1.0f / 60.0f;
  for(ui32 ui = 0; ui < 60; ui++)
    mUpdateTime[ui] = ftime;
  mUpdateTimeAverage = 0.0f;
  mUpdateTimeIdx = 0;
#endif
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

  if(mpResourceManager) {
    nuResourceManager* ptr = mpResourceManager;
    mpResourceManager = nullptr;
    delete ptr;
  }
}

void nuAppMain::begin(void)
{
#if !NDEBUG
  mRenderPerformance = nuDrawString::create("Render thread: 8888.888 ms");
  mUpdatePerformance = nuDrawString::create("Update thread: 8888.888 ms");
#endif
}

void nuAppMain::end(void)
{
#if !NDEBUG
  mRenderPerformance.release();
  mUpdatePerformance.release();
#endif
}

i32 nuAppMain::main(void)
{
  mState = RUNNING;
  mpRenderGL->acquire();

  NU_TRACE("Main loop is up and running.\n");

  begin();

  while(mState != TERMINATING) {
#if !NDEBUG
    if(mUpdateTimeIdx == 0) {
      f32 sum = 0.0f;
      for(ui32 ui = 0; ui < 60; ui++)
        sum += mUpdateTime[ui];
      mUpdateTimeAverage = sum / 60.0f;
      c8 buffer[128];
      snprintf(buffer, 128, "Update thread: %.3f ms", mUpdateTimeAverage);
      buffer[127] = 0x00;
      mUpdatePerformance->setString(buffer);
    }
#endif

    nuAutoReleasePool pool;
    update();
    draw();

#if !NDEBUG
    mUpdatePerformance->drawAt(mUpdatePerformaceDispPos);
    mUpdateTime[mUpdateTimeIdx] = mTimer.getElapsedTime();
    if(mUpdateTime[mUpdateTimeIdx] > 1000.0f)
      mUpdateTime[mUpdateTimeIdx] = 1000.0f;
    mUpdateTimeIdx++;
    mUpdateTimeIdx %= 60;
#endif
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
  mpResourceManager = new nuResourceManager;
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
  mCurrentTagList ^= 1;

  mpRenderGL->setNextTagList(mTagList[mCurrentTagList]);
  mFrameID = mpRenderGL->synchronize();

#if !NDEBUG
  mTimer.reset();

  if(mRenderTimeChanged) {
    c8 buffer[128];
    snprintf(buffer, 128, "Render thread: %.3f ms", mRenderTimeAverage);
    buffer[127] = 0x00;
    mRenderPerformance->setString(buffer);
    mRenderTimeChanged = false;
  }
#endif

  if(mpEntityManager->getEntityNum()) {
    nuTaskSet update_set(mpEntityManager->getEntityNum());
    mpEntityManager->createUpdateList(update_set, mEntityTable);
    if(update_set.getTaskNum() > 0) {
      nuThreadPool::JobTicket ticket = mpThreadPool->entryJob(update_set);
      mpThreadPool->waitUntilFinished(ticket);
    }
  }
}

void nuAppMain::draw(void)
{
  if(mEntityTableIterator.initialize(mEntityTable) == 0)
    return;

#if !NDEBUG
    mRenderPerformance->drawAt(mRenderPerformaceDispPos);
#endif  

  ui32 tag_num = mTagNum / nuThreadPool::MAX_WORKER;

  nuRect view_rect = mpRenderGL->getViewport();
  for(ui32 ui = 0; ui < nuThreadPool::MAX_WORKER; ui++) {
    mpGraphicContext[ui]->begin(mFrameID, &mpTag[ui * tag_num], tag_num);
    mpGraphicContext[ui]->setViewport(view_rect);
    mpGraphicContext[ui]->setScissor(true, view_rect);
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

  {
    nuGContext::SortTagContext sort_ctx[nuThreadPool::MAX_WORKER];
    ui32 ctx_num = 0;
    for(ui32 ui = 0; ui < nuThreadPool::MAX_WORKER; ui++) {
      if(sort_ctx[ctx_num].initialize(mpGraphicContext[ui]))
        ctx_num++;
    }

    nuGContext::SortTagContext::createTagList(mTagList[mCurrentTagList ^ 1], sort_ctx, ctx_num);
  }
}

void nuAppMain::executeDraw(void* param)
{
  const ui32 reserve_num = 8;
  nuEntity** table;
  nuGContext* p_ctx = static_cast< nuGContext* >(param);
  ui32 num = mEntityTableIterator.get(&table, reserve_num);
  while(num > 0) {
    for(ui32 ui = 0; ui < num; ui++)
      mpEntityManager->drawEntity(*p_ctx, *table[ui]);
    num = mEntityTableIterator.get(&table, reserve_num);
  }
  p_ctx->end();
}
