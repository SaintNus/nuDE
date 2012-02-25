/*!
 * \file nuThreadPool.cpp
 * \brief Thread pool.
 * \author Nus
 * \date 2012/02/13 00:28
 */

#include "nuThreadPool.h"

IMPLEMENT_TYPE_INFO(nuThreadPool, nuObject);

nuThreadPool::nuThreadPool()
{
  mJobArenaThread.dispatchThread(&mJobArena,
                                 static_cast< nuFunction >(&JobArena::schedulerProc),
                                 nullptr);
}

nuThreadPool::~nuThreadPool()
{
  mJobArena.exit();
  mJobArenaThread.join();
}

void nuThreadPool::JobArena::schedulerProc(void* param)
{
  NU_TRACE("Opening job arena.\n");
  while(!mExit) {
    nuThread::usleep(16666);
  }
  NU_TRACE("Closing job arena.\n");
}

nuThreadPool::JobTicket nuThreadPool::JobArena::entryJob(const nuTaskSet& task_set)
{
  Job* p_job = new Job(task_set);
  queueEntry(p_job);
  return JobTicket(p_job);
}
