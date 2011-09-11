/*!
 * \file nuMemory.h
 * \brief Memory.
 * \author Nus
 * \date 2011/09/11 20:10
 */

#include "nuMemory.h"

// Allocate memory.
void* nude::Alloc(size_t size)
{
  //! \todo Add memory header info.
  return malloc(size);
}

// Deallocate memory.
void nude::Dealloc(void* p_addr)
{
  //! \todo Leak check.
  free(p_addr);
}

/*
 * Operator new.
 */

void* operator new(std::size_t size) throw(std::bad_alloc)
{
  return nude::Alloc(size);
}

void* operator new(std::size_t size, const std::nothrow_t&) throw()
{
  return nude::Alloc(size);
}

void* operator new[](std::size_t size) throw(std::bad_alloc)
{
  return nude::Alloc(size);
}

void* operator new[](std::size_t size, const std::nothrow_t&) throw()
{
  return nude::Alloc(size);
}

/*
 * Operator delete.
 */

void operator delete(void* ptr) throw()
{
  nude::Dealloc(ptr);
}

void operator delete(void* ptr, const std::nothrow_t&) throw()
{
  nude::Dealloc(ptr);
}

void operator delete[](void* ptr) throw()
{
  nude::Dealloc(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t&) throw()
{
  nude::Dealloc(ptr);
}
