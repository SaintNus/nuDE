/*!
 * \file nuMachCondition.mm
 * \brief Mach condition.
 * \author Nus
 * \date 2012/02/26 01:10
 */

#include "nuMachCondition.h"

nuMachCondition::nuMachCondition()
    : mpHandle(nullptr)
{
  NSCondition* condition = [[NSCondition alloc] init];
  mpHandle = condition;
}

nuMachCondition::~nuMachCondition()
{
  NSCondition* condition = static_cast< NSCondition* >(mpHandle);
  [condition release];
}

void nuMachCondition::lock(void)
{
  NSCondition* condition = static_cast< NSCondition* >(mpHandle);
  [condition lock];
}

void nuMachCondition::unlock(void)
{
  NSCondition* condition = static_cast< NSCondition* >(mpHandle);
  [condition unlock];
}

void nuMachCondition::wait(void)
{
  NSCondition* condition = static_cast< NSCondition* >(mpHandle);
  [condition wait];
}

void nuMachCondition::signal(void)
{
  NSCondition* condition = static_cast< NSCondition* >(mpHandle);
  [condition signal];
}

void nuMachCondition::broadcast(void)
{
  NSCondition* condition = static_cast< NSCondition* >(mpHandle);
  [condition broadcast];
}
