/*!
 * \file nuThreadPool.cpp
 * \brief Thread pool.
 * \author Nus
 * \date 2012/02/13 00:28
 */

#include "CoreInclude.h"
#include "nuThreadPool.h"

IMPLEMENT_TYPE_INFO(nuThreadPool, nuObject);

nuThreadPool::nuThreadPool()
{
  mJobArenaThread.setName("Job Arena");
  mJobArenaThread.dispatchThread(&mJobArena,
                                 static_cast< nuFunction >(&JobArena::schedulerProc),
                                 this);

  for(ui32 ui = 0; ui < MAX_WORKER; ui++)
    mWorker[ui].dispatchWorker(ui + 1, mJobArena);
}

nuThreadPool::~nuThreadPool()
{
  for(ui32 ui = 0; ui < MAX_WORKER; ui++)
    mWorker[ui].exit();

  mJobArena.exit();
  mJobArenaThread.join();
}

void nuThreadPool::JobArena::schedulerProc(void* param)
{
  nuThreadPool* p_pool = static_cast< nuThreadPool* >(param);

  NU_TRACE("Opening job arena.\n");
  mCondition.lock();

  while(!mExit) {
    if(mUnfinishedJob > 0)
      nuThread::usleep(500);

    while(mConditionValue == EMPTY && mUnfinishedJob == 0) {
      mState = STATE_WAIT;
      mCondition.wait();
      if(mExit)
        break;
    }

    if(mExit)
      break;

    mState = STATE_RUNNING;

    if(mUnfinishedJob == 0)
      setCondition(EMPTY);

    if(!mJobList.empty())
      processFinishedJob();

    if(!mEntryList.empty())
      processJobEntry();

    if(!mJobList.empty()) {
      for(ui32 ui = 0; ui < MAX_WORKER; ui++) {
        Worker& worker = p_pool->mWorker[ui];
        if(worker.isIdling()) {
          Task* p_task = getAvailableTask();
          if(p_task)
            worker.assignTask(p_task);
          else
            break;
        }
      }
    }
  }

  mCondition.unlock();
  mState = STATE_IDLE;
  NU_TRACE("Closing job arena.\n");
}

nuThreadPool::JobTicket nuThreadPool::JobArena::entryJob(const nuTaskSet& task_set)
{
  Job* p_job = new Job(task_set);
  queueEntry(p_job);
  setCondition(HAS_DATA);
  return JobTicket(p_job);
}

nuThreadPool::Worker::~Worker()
{
  exit();
}

void nuThreadPool::Worker::dispatchWorker(ui32 worker_id, JobArena& job_arena)
{
  i8 worker_name[32];

  snprintf(worker_name, 32, "Worker-%d", worker_id);
  worker_name[31] = 0x00;
  mThread.setName(worker_name);
  mID = worker_id;

  mThread.dispatchThread(this, 
                         static_cast< nuFunction >(&Worker::workerProcedure),
                         &job_arena);

  while(mState != STATE_IDLE)
    nuThread::usleep(1000);
}

void nuThreadPool::Worker::workerProcedure(void* param)
{
  nuThreadPool::JobArena& job_arena = *static_cast< nuThreadPool::JobArena* >(param);

  while(!mExit) {
    mState = STATE_IDLE;

    mCondition.lockWhenCondition(COND_ASSIGNED);

    if(mExit) {
      mCondition.unlockWithCondition(COND_EMPTY);
      break;
    }

    mState = STATE_EXECUTING;

    while(!mExit && mpTask) {
      mpTask->execute(mID);
      {
        Task* p_task = mpTask;
        Job* p_job = mpTask->p_job;
        mpTask = p_job->nextTask();
        if(p_job->commitTask(mID, *p_task)) {
          job_arena.setCondition(nuThreadPool::JobArena::HAS_DATA);
          break;
        }
      }
    }

    mCondition.unlockWithCondition(COND_EMPTY);
  }

  mState = STATE_TERMINATED;
}

void nuThreadPool::Worker::assignTask(Task* p_task)
{
  mCondition.lockWhenCondition(COND_EMPTY);
  NU_ASSERT(mpTask == nullptr, "Task exist!\n");
  mpTask = p_task;
  mCondition.unlockWithCondition(COND_ASSIGNED);
}

void nuThreadPool::waitUntilFinished(JobTicket& ticket)
{
  if(ticket.isValid())
    ticket.mpJob->wait();
}
