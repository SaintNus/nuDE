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

class nuTask
{
  friend class nuTaskSet;

private:
  nuObject* mpObject;
  nuFunction mFunction;
  void* mParameter;

  nuTask()
      : mpObject(nullptr),
        mFunction(nullptr),
        mParameter(nullptr)
  {
    // None...
  }

public:
  nuTask(nuObject* p_obj, nuFunction p_func, void* param)
      : mpObject(p_obj),
        mFunction(p_func),
        mParameter(param)
  {
    // None...
  }

  ~nuTask() {
    // None...
  }

  nuTask(const nuTask& task)
      : mpObject(task.mpObject),
        mFunction(task.mFunction),
        mParameter(task.mParameter)
  {
    // None...
  }

  void execute(void) {
    (mpObject->*mFunction)(mParameter);
  }

  const nuTask& operator = (const nuTask& task) {
    mpObject = task.mpObject;
    mFunction = task.mFunction;
    mParameter = task.mParameter;
    return *this;
  }

  bool operator == (const nuTask& task) {
    if(mpObject == task.mpObject &&
       mFunction == task.mFunction &&
       mParameter == task.mParameter)
      return true;
    return false;
  }

  bool operator != (const nuTask& task) {
    return !(*this == task);
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
    return mTaskList.size();
  }

};

class nuThreadPool : public nuObject
{
  DECLARE_TYPE_INFO;

public:
  nuThreadPool();
  ~nuThreadPool();

};

#endif
