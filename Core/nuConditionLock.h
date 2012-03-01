/*!
 * \file nuConditionLock.h
 * \brief Condition lock.
 * \author Nus
 * \date 2012/02/29 23:38
 */

#ifndef __NUCONDITIONLOCK_H__
#define __NUCONDITIONLOCK_H__

#include "nuType.h"

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

    void lockWhenCondition(i32 condition) {
      mConditionLock.lockWhenCondition(condition);
    }

    void trylockWhenCondition(i32 condition) {
      mConditionLock.trylockWhenCondition(condition);
    }

    void unlockWithCondition(i32 condition) {
      mConditionLock.lockWithCondition(condition);
    }

  };

}

#endif
