/*!
 * \file nuMachThread.h
 * \brief MacOS X thread handler.
 * \author Nus
 * \date 2011/11/06 04:35
 */

#ifndef __NUMACHTHREAD_H__
#define __NUMACHTHREAD_H__

class nuMachThread
{
public:
  nuMachThread();
  ~nuMachThread();

  void dispatch(nuObject* p_object, nuFunction func, void* param);

  bool isCancelled(void);
  bool isFinished(void);
  bool isExecuting(void);

  void setName(ccstr name);
  ccstr getName(void) const;

  void setStackSize(size_t stack_sz);
  size_t getStackSize(void) const;

  void setPriority(f64 priority);
  f64 getPriority(void) const;

private:
  void* mpHandle;

};

#endif
