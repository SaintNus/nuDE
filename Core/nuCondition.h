/*!
 * \file nuCondition.h
 * \brief Condition template.
 * \author Nus
 * \date 2012/02/26 00:55
 */

#ifndef __NUCONDITION_H__
#define __NUCONDITION_H__

namespace nude
{

  template< class T >
  class Condition
  {
    T mCondition;

  public:
    Condition() {}
    ~Condition() {}

    void lock(void) {
      mCondition.lock();
    }

    void unlock(void) {
      mCondition.unlock();
    }

    void wait(void) {
      mCondition.wait();
    }

    void signal(void) {
      mCondition.signal();
    }

    void broadcast(void) {
      mCondition.broadcast();
    }

  };

}

#endif
