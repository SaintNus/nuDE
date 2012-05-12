/*!
 * \file nuMachConditionLock.h
 * \brief Conditional lock.
 * \author Nus
 * \date 2012/02/29 23:59
 */

#ifndef __NUMACHCONDITIONLOCK_H__
#define __NUMACHCONDITIONLOCK_H__

class nuMachConditionLock
{
  void* mpHandle;

public:
  nuMachConditionLock(i32 condition);
  ~nuMachConditionLock();

  i32 getCondition(void) const;

  void lock(void);
  void unlock(void);

  void lockWhenCondition(i32 condition);
  bool trylockWhenCondition(i32 condition);
  void unlockWithCondition(i32 condition);

};

#endif
