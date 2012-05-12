/*!
 * \file nuMachAutoReleasePool.mm
 * \brief Auto-release pool for Mach.
 * \author Nus
 * \date 2012/02/26 01:49
 */

#import "CoreInclude.h"
#import "nuMachAutoReleasePool.h"

nuMachAutoReleasePool::nuMachAutoReleasePool()
    : mpHandle(nullptr)
{
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  mpHandle = pool;
}

nuMachAutoReleasePool::~nuMachAutoReleasePool()
{
  NSAutoreleasePool* pool = static_cast< NSAutoreleasePool* >(mpHandle);
  [pool drain];
}
