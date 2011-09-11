/*!
 * \file nuObject.cpp
 * \brief Base class.
 * \author Nus
 * \date 2011/09/11 23:54
 */

#include "nuMemory.h"
#include "nuObject.h"

/*
 * Operator new.
 */

void* nuObject::operator new(std::size_t size)
{
  return nude::Alloc(size);
}

void* nuObject::operator new[](std::size_t size)
{
  return nude::Alloc(size);
}

/*
 * Operator delete.
 */

void nuObject::operator delete(void* ptr)
{
  nude::Dealloc(ptr);
}

void nuObject::operator delete[](void* ptr)
{
  nude::Dealloc(ptr);
}
