/*!
 * \file nuThread.h
 * \brief Thread class.
 * \author Nus
 * \date 2011/11/06 03:40
 */

#ifndef __NUTHREAD_H__
#define __NUTHREAD_H__

namespace nude
{
  /*!
   * \class Thread.
   * \brief Template for thread handler class.
   */
  template< class ThreadHandler >
  class Thread : public nuObject
  {
  public:
    //! \brief Default constructor.
    Thread() {}
    //! \brief Default destructor.
    ~Thread() {}

    //! \brief Dispatch thread.
    void dispatchThread(nuObject* p_object, nuFunction func, void* param) {
      mThreadHandler.dispatch(p_object, func, param);
    }

    //! \brief Is thread cancelled.
    bool isCancelled(void) const {
      return mThreadHandler.isCancelled();
    }
    //! \brief Is thread finished.
    bool isFinished(void) const {
      return mThreadHandler.isFinished();
    }
    //! \brief Is thread executing.
    bool isExecuting(void) const {
      return mThreadHandler.isExecuting();
    }

    //! \brief Set thread name.
    void setName(ccstr name) {
      mThreadHandler.setName(name);
    }
    //! \brief Get thread name.
    ccstr getName(void) const {
      return mThreadHandler.getName();
    }

    //! \brief Set stack size.
    void setStackSize(size_t stack_sz) {
      mThreadHandler.setStackSize(stack_sz);
    }
    //! \brief Get stack size.
    size_t getStackSize(void) const {
      return mThreadHandler.getStackSize();
    }

    //! \brief Set thread priority.
    void setPriority(f64 priority) {
      mThreadHandler.setPriority(priority);
    }
    //! \brief Get thread priority.
    f64 getPriority(void) const {
      return mThreadHandler.getPriority();
    }

    //! \brief Sleep (in seconds).
    static void sleep(ui32 int_seconds);
    //! \brief Sleep (in μseconds).
    static void usleep(ui32 int_useconds);

  private:
    ThreadHandler mThreadHandler; //!< Thread handler.
  };
}

#endif
