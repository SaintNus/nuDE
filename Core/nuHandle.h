/*!
 * \file nuHandle.h
 * \brief Handle template.
 * \author Nus
 * \date 2012/01/30 00:04
 */

#ifndef __NUHANDLE_H__
#define __NUHANDLE_H__

namespace nude {

  template< class T >
  class Handle
  {
    friend T;

  public:
    Handle(const Handle& handle)
        : mpObject(nullptr)
    {
      if(handle.isValid())
        mpObject = handle.mpObject->incRefCount();
    }

    ~Handle() {
      if(mpObject) {
        mpObject->decRefCount();
        mpObject = nullptr;
      }
    }

    const Handle& operator = (const Handle& handle) {
      if(mpObject) {
        mpObject->decRefCount();
        mpObject = nullptr;
      }
      if(handle.isValid())
        mpObject = handle.mpObject->incRefCount();
      return *this;
    }

    bool isValid() const {
      return mpObject != nullptr;
    }

    T* operator * (void) const {
      return mpObject;
    }

    T& operator & (void) const {
      return *mpObject;
    }

    T* operator -> (void) const {
      return mpObject;
    }

  private:
    T* mpObject;

    Handle(T* p_obj)
        : mpObject(p_obj)
    {
      // None...
    }

  };

}

#endif
