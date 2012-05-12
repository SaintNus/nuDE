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
  public:
    Handle()
        : mpObject(nullptr)
    {
      // None...
    }

    Handle(const Handle& handle)
        : mpObject(nullptr)
    {
      if(handle.isValid())
        mpObject = static_cast< T* >(handle.mpObject->incRefCount());
    }

    Handle(T* p_obj)
        : mpObject(nullptr)
    {
      NU_ASSERT(p_obj != nullptr, "Invalid pointer.\n");
      mpObject = static_cast< T* >(p_obj->incRefCount());
    }

    ~Handle() {
      release();
    }

    const Handle& operator = (const Handle& handle) {
      if(mpObject) {
        mpObject->decRefCount();
        mpObject = nullptr;
      }
      if(handle.isValid())
        mpObject = static_cast< T* >(handle.mpObject->incRefCount());
      return *this;
    }

    const Handle& operator = (T* p_obj) {
      if(mpObject) {
        mpObject->decRefCount();
        mpObject = nullptr;
      }
      if(p_obj)
        mpObject = p_obj->incRefCount();
      return *this;
    }

    void release(void) {
      if(mpObject) {
        mpObject->decRefCount();
        mpObject = nullptr;
      }
    }

    bool isValid() const {
      return mpObject != nullptr;
    }

    T* operator & (void) const {
      return mpObject;
    }

    T& operator * (void) const {
      return *mpObject;
    }

    T* operator -> (void) const {
      return mpObject;
    }

    template< class C >
    C* cast(void) const {
      return static_cast< C* >(mpObject);
    }

  private:
    T* mpObject;

  };

}

#endif
