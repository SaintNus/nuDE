/*!
 * \file nuSingleton.h
 * \brief Singleton template class.
 * \author Nus
 * \date 2011/11/11 22:36
 */

#ifndef __NUSINGLETON_H__
#define __NUSINGLETON_H__

#include "nuTypeInfo.h"

/*!
 * \class nuSingleton
 * \brief Singleton template class.
 */
template< class T >
class nuSingleton
{
  static T** staticInstance(void) {
    static T* mpInstance = nullptr;
    return &mpInstance;
  }

  //! \brief Default constructor.
  nuSingleton();
  //! \brief Default destructor.
  ~nuSingleton();

public:
  //! \brief Get singleton instance.
  static T* instance(void) {
    return *(staticInstance());
  }

  //! \brief Create singleton instance.
  static T* createInstance(void) {
    T** inst = staticInstance();
    if(!(*inst))
      *inst = new T;
    return *inst;
  }

  //! \brief Delete singleton instance.
  static void deleteInstance(void) {
    T** inst = staticInstance();
    if(*inst) {
      T* ptr = *inst;
      *inst = nullptr;
      delete ptr;
    }
  }

  static const nuTypeInfo& getTypeInfo(void) {
    NU_ASSERT_C(instance() != nullptr);
    return instance()->getTypeInfo();
  }

};

#define DECLARE_SINGLETON(_class) \
  private: \
    friend class nuSingleton< _class >; \
  public: \
    static _class* instance(void) { \
      return nuSingleton< _class >::instance(); \
    }

#endif
