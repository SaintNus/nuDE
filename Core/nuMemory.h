/*!
 * \file nuMemory.h
 * \brief Memory.
 * \author Nus
 * \date 2011/09/11 20:10
 */

#ifndef __NUMEMORY_H__
#define __NUMEMORY_H__

namespace nude
{

  //! \brief Allocate memory.
  void* Alloc(size_t size, size_t align = 8);
  //! \brief Deallocate memory.
  void Dealloc(void* p_addr);
  //! \brief Memory size.
  size_t MemSize(void* p_addr);

  class Allocator {
  public:
    Allocator() {}
    ~Allocator() {}

    void* allocate(size_t size) const {
      return Alloc(size);
    }
    void deallocate(void* p_addr) const {
      return Dealloc(p_addr);
    }
    size_t size(void* p_addr) const {
      return MemSize(p_addr);
    }

  };

  template< class Alloc = Allocator >
  class TempBuffer {
    void* mpBuffer;
    size_t mSize;

  public:
    TempBuffer(size_t size)
        : mSize(size)
    {
      Alloc alloc;
      mpBuffer = alloc.allocate(mSize);
    }
    ~TempBuffer() {
      if(mpBuffer) {
        Alloc alloc;
        alloc.deallocate(mpBuffer);
      }
    }

    void* getBuffer(void) const {
      return mpBuffer;
    }
    size_t getSize(void) const {
      return mSize;
    }
  };

}

//! \brief Operator new.
//! \{

void* operator new(std::size_t size) throw(std::bad_alloc);
void* operator new(std::size_t size, const std::nothrow_t&) throw();
void* operator new[](std::size_t size) throw(std::bad_alloc);
void* operator new[](std::size_t size, const std::nothrow_t&) throw();

//! \}

//! \brief Operator delete.
//! \{

void operator delete(void* ptr) throw(); 
void operator delete(void* ptr, const std::nothrow_t&) throw();
void operator delete[](void* ptr) throw();
void operator delete[](void* ptr, const std::nothrow_t&) throw();

//! \}

#endif