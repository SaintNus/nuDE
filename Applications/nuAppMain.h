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
  nuRenderGL& getRenderGL(void) const {
    return *mpRenderGL;
  }
  STATE getState(void) const {
    return mState;
  }
  nuThreadPool& getThreadPool(void) const {
    return *mpThreadPool;
  }

  nuEntityManager& getEntityManager(void) const {
    return *mpEntityManager;
  }

protected:
  static const size_t DEFAULT_RING_BUFFER_SIZE = 2 * 1024 * 1024;

  STATE mState;
  nuRenderGL* mpRenderGL;
  nuThreadPool* mpThreadPool;

  nuEntityManager* mpEntityManager;
  nuGContextBuffer* mpContextBuffer;
  size_t mRingBufferSize;
  nuGContext* mpGraphicContext[nuThreadPool::MAX_WORKER];

  virtual void update(void);
  virtual void end(void) {}

};

#endif
