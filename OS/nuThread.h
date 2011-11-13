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
    Thread() {}
    ~Thread() {}

    void dispatchThread(nuObject* p_object, nuFunction func, void* param) {
      mThreadHandler.dispatch(p_object, func, param);
    }

    bool isCancelled(void) const {
      return mThreadHandler.isCancelled();
    }
    bool isFinished(void) const {
      return mThreadHandler.isFinished();
    }
    bool isExecuting(void) const {
      return mThreadHandler.isExecuting();
    }

    void setName(ccstr name) {
      mThreadHandler.setName(name);
    }
    ccstr getName(void) const {
      return mThreadHandler.getName();
    }

    void setStackSize(size_t stack_sz) {
      mThreadHandler.setStackSize(stack_sz);
    }
    size_t getStackSize(void) const {
      return mThreadHandler.getStackSize();
    }

    void setPriority(f64 priority) {
      mThreadHandler.setPriority(priority);
    }
    f64 getPriority(void) const {
      return mThreadHandler.getPriority();
    }

    static void sleep(ui32 int_seconds);
    static void usleep(ui32 int_useconds);

  private:
    ThreadHandler mThreadHandler;
  };
}

typedef nude::Thread< nuMachThread > nuThread;

#endif
