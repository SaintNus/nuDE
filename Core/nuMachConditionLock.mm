/*!
 * \file nuMachConditionLock.mm
 * \brief Conditional lock.
 * \author Nus
 * \date 2012/02/29 23:59
 */

#include "nuMachConditionLock.h"

nuMachConditionLock::nuMachConditionLock(i32 condition)
    : mpHandle(nullptr)
{
  NSConditionLock* cond_lock = [[NSConditionLock alloc] initWithCondition: condition];
  mpHandle = cond_lock;
}

nuMachConditionLock::~nuMachConditionLock()
{
  if(mpHandle) {
    NSConditionLock* cond_lock = static_cast< NSConditionLock* >(mpHandle);
    [cond_lock release];
  }
}

i32 nuMachConditionLock::getCondition(void) const
{
  NSConditionLock* cond_lock = static_cast< NSConditionLock* >(mpHandle);
  return static_cast< i32 >([cond_lock condition]);
}

void nuMachConditionLock::lockWhenCondition(i32 condition)
{
  NSConditionLock* cond_lock = static_cast< NSConditionLock* >(mpHandle);
  [cond_lock lockWhenCondition: condition];
}

void nuMachConditionLock::trylockWhenCondition(i32 condition)
{
  NSConditionLock* cond_lock = static_cast< NSConditionLock* >(mpHandle);
  [cond_lock tryLockWhenCondition: condition];
}

void nuMachConditionLock::unlockWithCondition(i32 condition)
{
  NSConditionLock* cond_lock = static_cast< NSConditionLock* >(mpHandle);
  [cond_lock unlockWithCondition: condition];
}