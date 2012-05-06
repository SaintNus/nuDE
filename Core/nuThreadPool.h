/*!
 * \file nuThreadPool.h
 * \brief Thread pool.
 * \author Nus
 * \date 2012/02/13 00:28
 */

#ifndef __NUTHREADPOOL_H__
#define __NUTHREADPOOL_H__

#include "nuObject.h"
#include "nuSingleton.h"
#include "nuOSType.h"

class nuTask
{
  friend class nuTaskSet;

  nuObject* mpObject;
  nuFunction mFunction;
  void* mpParameter;

  nuTask()
      : mpObject(nullptr),
        mFunction(nullptr),
        mpParameter(nullptr)
  {
    // None...
  }

public:
  nuTask(nuObject* p_obj, nuFunction p_func, void* param)
      : mpObject(p_obj),
        mFunction(p_func),
        mpParameter(param)
  {
    // None...
  }

  ~nuTask() {
    // None...
  }

  nuTask(const nuTask& task)
      : mpObject(task.mpObject),
        mFunction(task.mFunction),
        mpParameter(task.mpParameter)
  {
    // None...
  }

  const nuTask& operator = (const nuTask& task) {
    mpObject = task.mpObject;
    mFunction = task.mFunction;
    mpParameter = task.mpParameter;
    return *this;
  }

  bool operator == (const nuTask& task) {
    if(mpObject == task.mpObject &&
       mFunction == task.mFunction &&
       mpParameter == task.mpParameter)
      return true;
    return false;
  }

  bool operator != (const nuTask& task) {
    return !(*this == task);
  }

  nuObject* getObject(void) const {
    return mpObject;
  }

  nuFunction getFunction(void) const {
    return mFunction;
  }

  void* getParameter(void) const {
    return mpParameter;
  }

  bool isEmpty(void) const {
    return mpObject == nullptr && mFunction == nullptr && mpParameter == nullptr;
  }

};

class nuTaskSet
{
  friend class nuThreadPool;

private:
  static const ui32 EXPANDABLE_LIST_NUM = 64;

  typedef std::vector< nuTask > TaskList;
  typedef TaskList::iterator TaskListIterator;

  TaskList mTaskList;
  nuTask mEndTask;

public:
  nuTaskSet() {
    mTaskList.reserve(EXPANDABLE_LIST_NUM);
  }
  nuTaskSet(ui32 reserve) {
    mTaskList.reserve(reserve);
  }
  ~nuTaskSet() {}

  void addTask(const nuTask& task) {
#if DEBUG
    for(TaskListIterator it = mTaskList.begin(); it != mTaskList.end(); ++it) {
      NU_ASSERT(*it != task, "Task must be unique.\n");
    }
#endif
    if(mTaskList.size() == mTaskList.capacity())
      mTaskList.reserve(mTaskList.size() + EXPANDABLE_LIST_NUM);
    mTaskList.push_back(task);
  }

  void addEndTask(const nuTask& task) {
    mEndTask = task;
  }

  ui32 getTaskNum(void) const {
    return static_cast< ui32 >(mTaskList.size());
  }

  const nuTask& operator[] (ui32 index) const {
    return mTaskList[index];
  }

};

class nuThreadPool : public nuObject
{
  DECLARE_TYPE_INFO;

public:
  static const ui32 MAX_WORKER = 4;
  class JobTicket;

private:
  class Job;
  class JobArena;

  static void assignThreadID(nuObject& object, ui32 thread_id) {
    object.mThreadID = thread_id;
  }

  struct Task {
    Job* p_job;
    nuObject* p_object;
    nuFunction function;
    void* p_parameter;

    void execute(ui32 thread_id) {
      nuThreadPool::assignThreadID(*p_object, thread_id);
      (p_object->*function)(p_parameter);
      nuThreadPool::assignThreadID(*p_object, 0);
    }

    const Task& operator = (const nuTask& task) {
      p_object = task.getObject();
      function = task.getFunction();
      p_parameter = task.getParameter();
      return *this;
    }

    bool isEmpty(void) const {
      return p_job == nullptr;
    }

  };

  class Job {
    friend class JobArena;
    enum JobState {
      STATE_INITIALIZE = 0,
      STATE_STARTED,
      STATE_FINISHED,
    };
    
    union {
      ui32 mAttribute;
      struct {
        ui32 mRegistered: 1;
        ui32 mApproved: 1;
        ui32 mEndTaskEnable: 1;
        ui32 mReserved: 29;
      };
    };

    volatile i32 mRefCount;
    volatile i32 mFinishedTask;
    volatile i32 mAssignedTask;

    ui32 mNumTask;
    Task* mpTaskList;
    Task mEndTask;
    nuConditionLock mLock;

    Job();

  public:
    Job(const nuTaskSet& task_set)
        : mNumTask(task_set.getTaskNum()),
          mAttribute(0),
          mRefCount(1),
          mFinishedTask(0),
          mAssignedTask(0),
          mLock(STATE_INITIALIZE)
    {
      NU_ASSERT_C(mNumTask > 0);
      mpTaskList = static_cast< Task* >(nude::Alloc(sizeof(Task) * mNumTask));
      NU_ASSERT_C(mpTaskList != nullptr);
      for(ui32 ui = 0; ui < mNumTask; ui++) {
        mpTaskList[ui] = task_set[ui];
        mpTaskList[ui].p_job = this;
      }
      if(!task_set.mEndTask.isEmpty()) {
        mEndTask = task_set.mEndTask;
        mEndTask.p_job = this;
        mEndTaskEnable = 1;
      } else {
        mEndTask.p_job = nullptr;
      }
    }

    ~Job() {
      nude::Dealloc(mpTaskList);
    }

    bool isFinished(void) const {
      i32 finish_cnt = mNumTask;
      if(mEndTaskEnable)
        finish_cnt++;
      return finish_cnt == mFinishedTask;
    }

    bool isTaskAvailable(void) const {
      return static_cast< i32 >(mNumTask) > mAssignedTask;
    }

    Task* getTask(void) {
      i32 curr = mAssignedTask;
      i32 res = curr + 1;
      while(res <= static_cast< i32 >(mNumTask)) {
        bool ret = nuAtomic::cas(curr, res, &mAssignedTask);
        if(!ret) {
          curr = mAssignedTask;
          res = curr + 1;
        } else {
          return &mpTaskList[curr];
        }
      }
      return nullptr;
    }

    Task* nextTask(void) {
      i32 num_task = static_cast< i32 >(mNumTask);
      i32 curr = mAssignedTask;
      i32 res = curr + 1;
      while(res <= num_task) {
        bool ret = nuAtomic::cas(curr, res, &mAssignedTask);
        if(!ret) {
          curr = mAssignedTask;
          res = curr + 1;
        } else {
          return &mpTaskList[curr];
        }
      }
      return nullptr;
    }

    bool commitTask(ui32 thread_id, Task& task) {
      bool end_task = mEndTaskEnable;
      i32 num_task = static_cast< i32 >(mNumTask);
      i32 curr = mFinishedTask;
      i32 res = curr + 1;
      while(1) {
        bool ret = nuAtomic::cas(curr, res, &mFinishedTask);
        if(!ret) {
          curr = mFinishedTask;
          res = curr + 1;
        } else {
          if(res == num_task && end_task) {
            mEndTask.execute(thread_id);
            curr = mFinishedTask;
            res = curr + 1;
            while(1) {
              ret = nuAtomic::cas(curr, res, &mFinishedTask);
              if(!ret) {
                curr = mFinishedTask;
                res = curr + 1;
              } else {
                return true;
              }
            }
          }
          return false;
        }
      }
    }

    Job* incRefCount(void) {
      i32 curr = mRefCount;
      i32 res = curr + 1;
      while(1) {
        bool ret = nuAtomic::cas(curr, res, &mRefCount);
        if(!ret) {
          curr = mRefCount;
          res = curr + 1;
          if(curr < 1)
            return nullptr;
        } else {
          return this;
        }
      }
    }

    void decRefCount(void) {
      i32 curr = mRefCount;
      i32 res = curr - 1;
      while(1) {
        bool ret = nuAtomic::cas(curr, res, &mRefCount);
        if(!ret) {
          curr = mRefCount;
          res = curr - 1;
        } else {
          if(mRefCount == 0) {
            curr = mRefCount;
            res = curr - 1;
            while(res < 0) {
              ret = nuAtomic::cas(curr, res, &mRefCount);
              if(!ret) {
                curr = mRefCount;
                res = curr - 1;
              } else {
                delete this;
                return;
              }
            }
          } else {
            return;
          }
        }
      }
    }

    void wait(void) {
      mLock.lockWhenCondition(STATE_FINISHED);
      mLock.unlock();
    }
  };

  class JobArena : public nuObject {
    typedef std::list< Job* > JobList;
    typedef JobList::iterator JobListIterator;
    typedef JobList::const_iterator JobListConstIterator;

    enum STATE {
      STATE_IDLE = 0,
      STATE_WAIT,
      STATE_RUNNING,
    };

    ui32 mExit;

    JobList mJobList;

    JobList mEntryList;
    nuMutex mEntryMutex;
    nuCondition mCondition;
    volatile i32 mConditionValue;
    STATE mState;
    volatile i32 mUnfinishedJob;

    void queueEntry(Job* p_job) {
      nuMutex::Autolock lock(mEntryMutex);
      p_job->mRegistered = 1;
      nuAtomic::inc(&mUnfinishedJob);
      mEntryList.push_back(p_job->incRefCount());
    }

    Task* getAvailableTask(void) {
      JobListIterator it = mJobList.begin();
      while(it != mJobList.end()) {
        Job* p_job = *it;
        if(p_job->isTaskAvailable()) {
          Task* ret = p_job->getTask();
          if(ret)
            return ret;
        }
        ++it;
      }
      return nullptr;
    }

    void processJobEntry(void) {
      nuMutex::Autolock lock(mEntryMutex);
      JobListIterator it = mEntryList.begin();
      while(it != mEntryList.end()) {
        Job* p_job = *it;
        p_job->mLock.lockWhenCondition(Job::STATE_INITIALIZE);
        p_job->mApproved = 1;
        mJobList.push_back(p_job);
        ++it;
      }
      mEntryList.clear();
    }

    void processFinishedJob(void) {
      JobListIterator it = mJobList.begin();
      while(it != mJobList.end()) {
        Job* p_job = *it;
        if(p_job->isFinished()) {
          nuAtomic::dec(&mUnfinishedJob);
          p_job->mLock.unlockWithCondition(Job::STATE_FINISHED);
          p_job->decRefCount();
          it = mJobList.erase(it);
        } else {
          ++it;
        }
      }
    }

  public:
    enum CONDITION {
      EMPTY = 0,
      HAS_DATA,
    };
  
    JobArena()
        : mExit(0),
          mConditionValue(EMPTY),
          mState(STATE_IDLE),
          mUnfinishedJob(0)
    {
      // None...
    }

    ~JobArena() {
      for(JobListIterator it = mEntryList.begin(); it != mEntryList.end(); ++it) {
        delete *it;
        *it = nullptr;
      }
      mEntryList.clear();
      for(JobListIterator it = mJobList.begin(); it != mJobList.end(); ++it) {
        delete *it;
        *it = nullptr;
      }
      mJobList.clear();
    }

    JobTicket entryJob(const nuTaskSet& task_set);

    void schedulerProc(void* param);

    void exit(void) {
      mExit = 1;

      while(mState != STATE_IDLE) {
        mCondition.signal();
        nuThread::usleep(1000);
      }
    }

    void setCondition(CONDITION condition) {
      i32 curr = mConditionValue;
      i32 res = condition;
      while(res != curr) {
        bool ret = nuAtomic::cas(curr, res, &mConditionValue);
        if(ret) {
          if(res == HAS_DATA && mState == STATE_WAIT) {
            mCondition.lock();
            mState = STATE_RUNNING;
            mCondition.signal();
            mCondition.unlock();
          }
        } else {
          curr = mConditionValue;
        }
      }
    }

  };

  class Worker : public nuObject {
    Task* mpTask;
    nuThread mThread;
    nuConditionLock mCondition;

    union {
      ui32 mAttribute;
      struct {
        ui32 mExit: 1;
        ui32 mState: 3;
        ui32 mID: 8;
        ui32 mReserved: 20;
      };
    };

  public:
    enum State {
      STATE_UNINITIALIZED = 0,
      STATE_IDLE,
      STATE_EXECUTING,
      STATE_TERMINATING,
      STATE_TERMINATED,
    };

    enum Condition {
      COND_EMPTY = 0,
      COND_ASSIGNED,
    };

    Worker()
        : mAttribute(0),
          mpTask(nullptr),
          mCondition(COND_EMPTY)
    {
      // None...
    }

    ~Worker();

    void dispatchWorker(ui32 worker_id, JobArena& job_arena);
    void workerProcedure(void* param);
    void assignTask(Task* p_task);

    void exit(void) {
      if(mExit == 0) {
        mCondition.lockWhenCondition(COND_EMPTY);
        mExit = 1;
        mCondition.unlockWithCondition(COND_ASSIGNED);
        while(mState != STATE_TERMINATED)
          nuThread::usleep(1000);
      }
    }

    State getState(void) const {
      return static_cast< State >(mState);
    }

    bool isIdling(void) const {
      return mState == STATE_IDLE && mpTask == nullptr;
    }

  };

  JobArena mJobArena;
  nuThread mJobArenaThread;
  Worker mWorker[MAX_WORKER];

public:
  class JobTicket {
    friend class nuThreadPool;
  
    Job* mpJob;

    JobTicket(Job* p_job)
        : mpJob(p_job)
    {
      // None...
    }
  public:
    JobTicket()
        : mpJob(nullptr)
    {
      // None...
    }

    ~JobTicket() {
      release();
    }

    JobTicket(const JobTicket& job_ticket) {
      if(job_ticket.mpJob)
        mpJob = job_ticket.mpJob->incRefCount();
      else
        mpJob = nullptr;
    }

    const JobTicket& operator = (const JobTicket& job_ticket) {
      if(mpJob) {
        mpJob->decRefCount();
        mpJob = nullptr;
      }
      if(job_ticket.mpJob)
        mpJob = job_ticket.mpJob->incRefCount();
      return *this;
    }

    bool isValid(void) const {
      return mpJob != nullptr;
    }

    void release(void) {
      if(mpJob) {
        Job* p_job = mpJob;
        mpJob = nullptr;
        p_job->decRefCount();
      }
    }

  };

  nuThreadPool();
  ~nuThreadPool();

  JobTicket entryJob(const nuTaskSet& task_set) {
    return JobTicket(mJobArena.entryJob(task_set));
  }

  void waitUntilFinished(JobTicket& ticket);

};

#endif
