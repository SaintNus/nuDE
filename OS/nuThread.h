/*!
 * \file nuThread.h
 * \brief Thread class.
 * \author Nus
 * \date 2011/11/06 03:40
 */

#ifndef __NUTHREAD_H__
#define __NUTHREAD_H__

#include "nuMachThread.h"

namespace nude
{
  template< class ThreadHandler >
  class Thread : public nuObject
  {
  public:
    Thread();
    ~Thread();
    void dispatchThread(nuObject* p_object, nuFunction func);

  private:
    ThreadHandler mThreadHandler;

  };
}

typedef nude::Thread< nuMachThread > nuThread;

#endif
