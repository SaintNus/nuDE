/*!
 * \file nuMachThread.h
 * \brief MacOS X thread handler.
 * \author Nus
 * \date 2011/11/06 04:35
 */

#ifndef __NUMACHTHREAD_H__
#define __NUMACHTHREAD_H__

/*!
 * \class nuMachThread
 * \brief Thread handler for MacOSX.
 */
class nuMachThread
{
public:
  //! \brief Default constructor.
  nuMachThread();
  //! \brief Default destructor.
  ~nuMachThread();

  //! \brief Dispatch thread.
  void dispatch(nuObject* p_object, nuFunction func, void* param);

  //! \brief Is thread cancelled.
  bool isCancelled(void) const;
  //! \brief Is thread finished.
  bool isFinished(void) const;
  //! \brief Is thread executing.
  bool isExecuting(void) const;

  //! \brief Set thread name.
  void setName(ccstr name);
  //! \brief Get thread name.
  ccstr getName(void) const;

  //! \brief Set stack size.
  void setStackSize(size_t stack_sz);
  //! \brief Get stack size.
  size_t getStackSize(void) const;

  //! \brief Set thread priority.
  void setPriority(f64 priority);
  //! \brief Get thread priority.
  f64 getPriority(void) const;

private:
  void* mpHandle;                 //!< Thread handle.

};

#endif
