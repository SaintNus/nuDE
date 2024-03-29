/*!
 * \file nuConditionLock.h
 * \brief Condition lock.
 * \author Nus
 * \date 2012/02/29 23:38
 */

#ifndef __NUCONDITIONLOCK_H__
#define __NUCONDITIONLOCK_H__

namespace nude
{

  template< class T >
  class ConditionLock
  {
    T mConditionLock;

    ConditionLock();

  public:
    ConditionLock(i32 condition)
        : mConditionLock(condition)
    {
      // None...
    }

    ~ConditionLock() {}

    i32 getCondition(void) const {
      return mConditionLock.getCondition();
    }

    void lock(void) {
      mConditionLock.lock();
    }

    void unlock(void) {
      mConditionLock.unlock();
    }

    void lockWhenCondition(i32 condition) {
      mConditionLock.lockWhenCondition(condition);
    }

    bool trylockWhenCondition(i32 condition) {
      return mConditionLock.trylockWhenCondition(condition);
    }

    void unlockWithCondition(i32 condition) {
      mConditionLock.unlockWithCondition(condition);
    }

  };

}

#endif
