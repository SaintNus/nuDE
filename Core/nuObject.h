/*!
 * \file nuObject.h
 * \brief Base class.
 * \author Nus
 * \date 2011/09/11 23:54
 */

#ifndef __NUOBJECT_H__
#define __NUOBJECT_H__

#include "nuDebug.h"
#include "nuMemory.h"
#include "nuTypeInfo.h"

/*!
 * \class nuObject
 * \brief Base class.
 */
class nuObject
{
public:
  DECLARE_TYPE_INFO;

  //! \brief Default constructor.
  nuObject() { /* None... */ }
  //! \brief Default destructor.
  virtual ~nuObject() { /* None... */ }

  //! \brief Operator new.
  //! \{
  void* operator new(std::size_t size) {
    return nude::Alloc(size);
  }
  void* operator new[](std::size_t size) {
    return nude::Alloc(size);
  }
  //! \}

  //! \brief Operator delete.
  //! \{
  void operator delete(void* ptr) {
    nude::Dealloc(ptr);
  }
  void operator delete[](void* ptr) {
    nude::Dealloc(ptr);
  }
  //! \}

protected:
  // None...

private:
  // None...
  
};

#endif