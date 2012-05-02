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
  mJobArenaThread.setName("Job Arena");
  mJobArenaThread.dispatchThread(&mJobArena,
                                 static_cast< nuFunction >(&JobArena::schedulerProc),
                                 this);

  for(ui32 ui = 0; ui < MAX_WORKER; ui++)
    mWorker[ui].dispatchWorker(ui, mJobArena);
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

  while(!mExit) {
    if(!mJobList.empty()) {
      JobIterator it = mJobList.begin();
      while(it != mJobList.end()) {
        Job* p_job = *it;
        if(p_job->isFinished()) {
          p_job->mLock.unlockWithCondition(Job::STATE_FINISHED);
          p_job->decRefCount();
          it = mJobList.erase(it);
        } else {
          ++it;
        }
      }
    }

    if(!mEntryList.empty()) {
      nuMutex::Autolock lock(mEntryMutex);
      JobIterator it = mEntryList.begin();
      while(it != mEntryList.end()) {
        Job* p_job = *it;
        p_job->mLock.lockWhenCondition(Job::STATE_INITIALIZE);
        p_job->mApproved = 1;
        mJobList.push_back(p_job);
        ++it;
      }
      mEntryList.clear();
    }

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

    nuThread::usleep(1);
  }
  NU_TRACE("Closing job arena.\n");
}

nuThreadPool::JobTicket nuThreadPool::JobArena::entryJob(const nuTaskSet& task_set)
{
  Job* p_job = new Job(task_set);
  queueEntry(p_job);
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
  while(!mExit) {
    mState = STATE_IDLE;

    mCondition.lockWhenCondition(COND_ASSIGNED);

    if(mExit) {
      mCondition.unlockWithCondition(COND_EMPTY);
      break;
    }

    mState = STATE_EXECUTING;

    while(!mExit && mpTask) {
      mpTask->execute();
      {
        Task* p_task = mpTask;
        Job* p_job = mpTask->p_job;
        mpTask = p_job->nextTask();
        p_job->commitTask(*p_task);
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
