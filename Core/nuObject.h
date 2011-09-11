/*!
 * \file nuObject.h
 * \brief Base class.
 * \author Nus
 * \date 2011/09/11 23:54
 */

#ifndef __NUOBJECT_H__
#define __NUOBJECT_H__

/*!
 * \class nuObject
 * \brief Base class.
 */
class nuObject
{
public:
  //! \brief Default constructor.
  nuObject() { /* None... */ }
  //! \brief Default destructor.
  virtual ~nuObject() { /* None... */ }

  //! \brief Operator new.
  //! \{
  void* operator new(std::size_t size);
  void* operator new[](std::size_t size);
  //! \}

  //! \brief Operator delete.
  //! \{
  void operator delete(void* ptr);
  void operator delete[](void* ptr);
  //! \}

protected:
  // None...

private:
  // None...
  
};

#endif
