/*!
 * \file nuMachMutex.cpp
 * \brief Mutex for MacOSX.
 * \author Nus
 * \date 2011/11/26 23:15
 */

#include "nuMachMutex.h"

nuMachMutex::nuMachMutex()
    : mpMutex(nullptr)
{
  NSLock* p_lock = [[NSLock alloc] init];
  mpMutex = p_lock;
}

nuMachMutex::~nuMachMutex()
{
  NSLock* p_lock = static_cast< NSLock* >(mpMutex);
  mpMutex = nullptr;
  [p_lock release];
  p_lock = nil;
}

void nuMachMutex::lock(void)
{
  NSLock* p_lock = static_cast< NSLock* >(mpMutex);
  [p_lock lock];
}

void nuMachMutex::unlock(void)
{
  NSLock* p_lock = static_cast< NSLock* >(mpMutex);
  [p_lock unlock];
}

bool nuMachMutex::trylock(void)
{
  NSLock* p_lock = static_cast< NSLock* >(mpMutex);
  return [p_lock tryLock] == YES;
}



nuMachRecursiveMutex::nuMachRecursiveMutex()
    : mpRecursiveMutex(nullptr)
{
  NSRecursiveLock* p_lock = [[NSRecursiveLock alloc] init];
  mpRecursiveMutex = p_lock;
}

nuMachRecursiveMutex::~nuMachRecursiveMutex()
{
  NSRecursiveLock* p_lock = static_cast< NSRecursiveLock* >(mpRecursiveMutex);
  mpRecursiveMutex = nullptr;
  [p_lock release];
  p_lock = nil;
}

void nuMachRecursiveMutex::lock(void)
{
  NSRecursiveLock* p_lock = static_cast< NSRecursiveLock* >(mpRecursiveMutex);
  [p_lock lock];
}

void nuMachRecursiveMutex::unlock(void)
{
  NSRecursiveLock* p_lock = static_cast< NSRecursiveLock* >(mpRecursiveMutex);
  [p_lock unlock];
}

bool nuMachRecursiveMutex::trylock(void)
{
  NSRecursiveLock* p_lock = static_cast< NSRecursiveLock* >(mpRecursiveMutex);
  return [p_lock tryLock] == YES;
}



nuMachSpinLock::nuMachSpinLock()
    : mSpinLock(0)
{
  // None...
}

nuMachSpinLock::~nuMachSpinLock()
{
  // None...
}

void nuMachSpinLock::lock(void)
{
  OSSpinLockLock(&mSpinLock);
}

void nuMachSpinLock::unlock(void)
{
  OSSpinLockUnlock(&mSpinLock);
}

bool nuMachSpinLock::trylock(void)
{
  return OSSpinLockTry(&mSpinLock);
}
