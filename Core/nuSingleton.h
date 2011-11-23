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
public:
  //! \brief Get singleton instance.
  static T* instance(void) {
    return mpInstance;
  }

  //! \brief Create singleton instance.
  static T* createInstance(void) {
    if(!mpInstance)
      mpInstance = new T;
    return mpInstance;
  }

  //! \brief Delete singleton instance.
  static void deleteInstance(void) {
    if(mpInstance) {
      T* ptr = mpInstance;
      mpInstance = nullptr;
      delete ptr;
    }
  }

  static const nuTypeInfo& getTypeInfo(void) {
    NU_ASSERT_C(mpInstance != nullptr);
    return mpInstance->getTypeInfo();
  }

private:
  static T* mpInstance;           //!< Singleton instance.
  //! \brief Default constructor.
  nuSingleton() {}
  //! \brief Default destructor.
  ~nuSingleton() {}

};

#define DECLARE_SINGLETON(_class) \
  private: \
    friend class nuSingleton< _class >

#define IMPLEMENT_SINGLETON(_class) \
  template<> _class* nuSingleton< _class >::mpInstance = nullptr

#define INST(_class) nuSingleton< _class >::instance()

#endif
