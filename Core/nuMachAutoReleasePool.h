/*!
 * \file nuMachAutoReleasePool.h
 * \brief Auto-release pool for Mach.
 * \author Nus
 * \date 2012/02/26 01:49
 */

#ifndef __NUMACHAUTORELEASEPOOL_H__
#define __NUMACHAUTORELEASEPOOL_H__

class nuMachAutoReleasePool
{
private:
  void* mpHandle;

public:
  nuMachAutoReleasePool();
  ~nuMachAutoReleasePool();

};

#endif
