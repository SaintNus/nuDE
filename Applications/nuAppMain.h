/*!
 * \file nuAppMain.h
 * \brief Application main class.
 * \author Nus
 * \date 2011/11/03 22:13
 */

#ifndef __NUAPPMAIN_H__
#define __NUAPPMAIN_H__

#include "nuEntityManager.h"

/*!
 * \class nuAppMain
 * \brief Application main class.
 */
class nuAppMain : public nuObject
{
  DECLARE_TYPE_INFO;

public:
  enum STATE {
    UNINITIALIZED = 0,
    READY,
    RUNNING,
    TERMINATING,
  };

  nuAppMain();
  virtual ~nuAppMain();

  virtual void initialize(void);
  virtual i32 main(void);
  virtual void terminate(void);

  void waitForTermination(void) {
    if(mState == RUNNING || mState == TERMINATING) {
      while(mState != READY)
        nuThread::usleep(16000);
    }
  }

  nuRenderGL& getRenderGL(void) {
    return *mpRenderGL;
  }

  STATE getState(void) const {
    return mState;
  }

  nuThreadPool& getThreadPool(void) {
    return *mpThreadPool;
  }

  nuEntityManager& getEntityManager(void) {
    return *mpEntityManager;
  }

  nuResourceManager& getResourceManager(void) {
    return *mpResourceManager;
  }

#if !NDEBUG
  void updateRenderTimeAverage(f32 avg) {
    mRenderTimeAverage = avg;
    mRenderTimeChanged = true;
  }
#endif

protected:
  static const size_t DEFAULT_RING_BUFFER_SIZE = 2 * 1024 * 1024;
  static const ui32 DEFAULT_TAG_NUM = 8 * 1024;

  STATE mState;
  nuResourceManager* mpResourceManager;
  nuRenderGL* mpRenderGL;
  nuThreadPool* mpThreadPool;

  nuEntityManager* mpEntityManager;
  nuGContextBuffer* mpContextBuffer;
  size_t mRingBufferSize;

  nuGContext* mpGraphicContext[nuThreadPool::MAX_WORKER];
  nuGContext::Tag* mpTag;

  ui32 mTagNum;
  i64 mFrameID;

  nuEntityManager::EntityTable mEntityTable;
  nuEntityManager::EntityTable::Iterator mEntityTableIterator;

  nuGContext::TagList mTagList[2];
  ui32 mCurrentTagList;

#if !NDEBUG
  nude::DrawString mRenderPerformance;
  nuPoint mRenderPerformaceDispPos;
  f32 mRenderTimeAverage;
  bool mRenderTimeChanged;

  nude::DrawString mUpdatePerformance;
  nuPoint mUpdatePerformaceDispPos;
  f32 mUpdateTime[60];
  ui32 mUpdateTimeIdx;
  f32 mUpdateTimeAverage;

  nuTimermSec mTimer;
#endif

  virtual void update(void);
  virtual void draw(void);
  virtual void begin(void);
  virtual void end(void);

  void executeDraw(void* param);

};

#endif
