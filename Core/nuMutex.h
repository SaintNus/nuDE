/*!
 * \file nuMutex.h
 * \brief Mutex.
 * \author Nus
 * \date 2011/11/26 22:36
 */

#ifndef __NUMUTEX_H__
#define __NUMUTEX_H__

#include "nuObject.h"

namespace nude
{

  template< class T >
  class Mutex : public nuObject
  {
  public:
    Mutex() {}
    ~Mutex() {}

    void lock(void) {
      mMutexHandler.lock();
    }

    void unlock(void) {
      mMutexHandler.unlock();
    }

    bool trylock(void) {
      return mMutexHandler.trylock();
    }

  private:
    T mMutexHandler;

  };

}

#endif
