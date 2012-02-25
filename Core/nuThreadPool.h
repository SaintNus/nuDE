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

private:
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
  class JobTicket;

private:
  class Job;
  class JobArena;

  struct Task {
    Job* p_job;
    nuObject* p_object;
    nuFunction function;
    void* p_parameter;

    void execute(void) {
      (p_object->*function)(p_parameter);
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
    
    union {
      ui32 mAttribute;
      struct {
        ui32 mRegistered: 1;
        ui32 mFinished: 1;
        ui32 mApproved: 1;
        ui32 mReserved: 29;
      };
    };

    volatile i32 mRefCount;

    ui32 mNumTask;
    Task* mpTaskList;
    Task mEndTask;

    Job();

  public:
    Job(const nuTaskSet& task_set)
        : mNumTask(task_set.getTaskNum()),
          mAttribute(0),
          mRefCount(1)
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
      } else {
        mEndTask.p_job = nullptr;
      }
    }

    ~Job() {
      nude::Dealloc(mpTaskList);
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
          }
        }
      }
    }

  };

  class JobArena : public nuObject {
    typedef std::list< Job* > JobList;
    typedef JobList::iterator JobIterator;
    typedef JobList::const_iterator JobConstIterator;

    bool mExit;

    JobList mJobList;

    JobList mEntryList;
    nuMutex mEntryMutex;

    void queueEntry(Job* p_job) {
      nuMutex::Autolock lock(mEntryMutex);
      p_job->mRegistered = 1;
      mEntryList.push_back(p_job->incRefCount());
    }

  public:
    JobArena()
        : mExit(false)
    {
      // None...
    }

    ~JobArena() {
      for(JobIterator it = mEntryList.begin(); it != mEntryList.end(); ++it) {
        delete *it;
        *it = nullptr;
      }
      mEntryList.clear();
      for(JobIterator it = mJobList.begin(); it != mJobList.end(); ++it) {
        delete *it;
        *it = nullptr;
      }
      mJobList.clear();
    }

    JobTicket entryJob(const nuTaskSet& task_set);

    void schedulerProc(void* param);

    void exit(void) {
      mExit = true;
    }

  };

  JobArena mJobArena;
  nuThread mJobArenaThread;

public:
  class JobTicket {
    friend class nuThreadPool;
  
    Job* mpJobTicket;

    JobTicket(Job* p_job)
        : mpJobTicket(p_job)
    {
      // None...
    }
  public:
    JobTicket()
        : mpJobTicket(nullptr)
    {
      // None...
    }

    ~JobTicket() {
      if(mpJobTicket) {
        mpJobTicket->decRefCount();
        mpJobTicket = nullptr;
      }
    }

    JobTicket(const JobTicket& job_ticket) {
      if(job_ticket.mpJobTicket)
        mpJobTicket = job_ticket.mpJobTicket->incRefCount();
      else
        mpJobTicket = nullptr;
    }

    const JobTicket& operator = (const JobTicket& job_ticket) {
      if(mpJobTicket) {
        mpJobTicket->decRefCount();
        mpJobTicket = nullptr;
      }
      if(job_ticket.mpJobTicket)
        mpJobTicket = job_ticket.mpJobTicket->incRefCount();
      return *this;
    }

    bool isValid(void) const {
      return mpJobTicket != nullptr;
    }

  };

  nuThreadPool();
  ~nuThreadPool();

  JobTicket entryJob(const nuTaskSet& task_set) {
    return mJobArena.entryJob(task_set);
  }

};

#endif
