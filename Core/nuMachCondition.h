/*!
 * \file nuMachCondition.h
 * \brief Mach condition.
 * \author Nus
 * \date 2012/02/26 01:10
 */

#ifndef __NUMACHCONDITION_H__
#define __NUMACHCONDITION_H__

class nuMachCondition
{
private:
  void* mpHandle;

public:
  nuMachCondition();
  ~nuMachCondition();

  void lock(void);
  void unlock(void);

  void wait(void);
  void signal(void);

  void broadcast(void);

};

#endif
