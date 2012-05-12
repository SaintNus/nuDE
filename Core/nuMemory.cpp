/*!
 * \file nuMemory.h
 * \brief Memory.
 * \author Nus
 * \date 2011/09/11 20:10
 */

#include "CoreInclude.h"
#include "nuMemory.h"

struct MemHeader
{
  size_t size;
  void* ptr;
};

// Allocate memory.
void* nude::Alloc(size_t size, size_t align)
{
  size_t na = align - 1;
  size_t sz = size + sizeof(MemHeader) + na;
  void* ptr = malloc(sz);
  MemHeader* p_header = static_cast< MemHeader* >(ptr);
  p_header->size = size;
  ui64 align_addr = reinterpret_cast< ui64 >(&p_header[1]);
  align_addr += na; 
  align_addr &= ~na;
  void** ret = reinterpret_cast< void** >(align_addr);
  ret[-1] = ptr;
  return reinterpret_cast< void* >(align_addr);
}

// Deallocate memory.
void nude::Dealloc(void* p_addr)
{
  if(!p_addr)
    return;

  void** ptr = static_cast< void** >(p_addr);
  ptr--;
  free(*ptr);
}

size_t nude::MemSize(void* p_addr)
{
  if(!p_addr)
    return 0;

  void** ptr = static_cast< void** >(p_addr);
  ptr--;
  MemHeader* p_header = static_cast< MemHeader* >(*ptr);
  return p_header->size;
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
