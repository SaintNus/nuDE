/*!
 * \file nuMachAtomic.cpp
 * \brief Atomic function for MacOSX.
 * \author Nus
 * \date 2011/11/27 00:17
 */

#include "CoreInclude.h"
#include "nuAtomic.h"
#include "nuMachAtomic.h"

using namespace nude;

template<>
i32 Atomic< nuMachAtomic >::add(i32 amount, volatile i32* value)
{
  return OSAtomicAdd32(amount, value);
}

template<>
i32 Atomic< nuMachAtomic >::addBarrier(i32 amount, volatile i32* value)
{
  return OSAtomicAdd32Barrier(amount, value);
}

template<>
i64 Atomic< nuMachAtomic >::add(i64 amount, volatile i64* value)
{
  return OSAtomicAdd64(amount, value);
}

template<>
i64 Atomic< nuMachAtomic >::addBarrier(i64 amount, volatile i64* value)
{
  return OSAtomicAdd64Barrier(amount, value);
}

template<>
i32 Atomic< nuMachAtomic >::inc(volatile i32* value)
{
  return OSAtomicIncrement32(value);
}

template<>
i32 Atomic< nuMachAtomic >::incBarrier(volatile i32* value)
{
  return OSAtomicIncrement32Barrier(value);
}

template<>
i64 Atomic< nuMachAtomic >::inc(volatile i64* value)
{
  return OSAtomicIncrement64(value);
}

template<>
i64 Atomic< nuMachAtomic >::incBarrier(volatile i64* value)
{
  return OSAtomicIncrement64Barrier(value);
}

template<>
i32 Atomic< nuMachAtomic >::dec(volatile i32* value)
{
  return OSAtomicDecrement32(value);
}

template<>
i32 Atomic< nuMachAtomic >::decBarrier(volatile i32* value)
{
  return OSAtomicDecrement32Barrier(value);
}

template<>
i64 Atomic< nuMachAtomic >::dec(volatile i64* value)
{
  return OSAtomicDecrement64(value);
}

template<>
i64 Atomic< nuMachAtomic >::decBarrier(volatile i64* value)
{
  return OSAtomicDecrement64Barrier(value);
}

template<>
i32 Atomic< nuMachAtomic >::logicOr(ui32 mask, volatile ui32* value)
{
  return OSAtomicOr32(mask, value);
}

template<>
i32 Atomic< nuMachAtomic >::logicOrBarrier(ui32 mask, volatile ui32* value)
{
  return OSAtomicOr32Barrier(mask, value);
}

template<>
i32 Atomic< nuMachAtomic >::logicOrOrg(ui32 mask, volatile ui32* value)
{
  return OSAtomicOr32Orig(mask, value);
}

template<>
i32 Atomic< nuMachAtomic >::logicOrBarrierOrg(ui32 mask, volatile ui32* value)
{
  return OSAtomicOr32OrigBarrier(mask, value);
}

template<>
i32 Atomic< nuMachAtomic >::logicAnd(ui32 mask, volatile ui32* value)
{
  return OSAtomicAnd32(mask, value);
}

template<>
i32 Atomic< nuMachAtomic >::logicAndBarrier(ui32 mask, volatile ui32* value)
{
  return OSAtomicAnd32Barrier(mask, value);
}

template<>
i32 Atomic< nuMachAtomic >::logicAndOrg(ui32 mask, volatile ui32* value)
{
  return OSAtomicAnd32Orig(mask, value);
}

template<>
i32 Atomic< nuMachAtomic >::logicAndBarrierOrg(ui32 mask, volatile ui32* value)
{
  return OSAtomicAnd32OrigBarrier(mask, value);
}

template<>
i32 Atomic< nuMachAtomic >::logicXor(ui32 mask, volatile ui32* value)
{
  return OSAtomicXor32(mask, value);
}

template<>
i32 Atomic< nuMachAtomic >::logicXorBarrier(ui32 mask, volatile ui32* value)
{
  return OSAtomicXor32Barrier(mask, value);
}

template<>
i32 Atomic< nuMachAtomic >::logicXorOrg(ui32 mask, volatile ui32* value)
{
  return OSAtomicXor32Orig(mask, value);
}

template<>
i32 Atomic< nuMachAtomic >::logicXorBarrierOrg(ui32 mask, volatile ui32* value)
{
  return OSAtomicXor32OrigBarrier(mask, value);
}

template<>
bool Atomic< nuMachAtomic >::cas(i32 old_value, i32 new_value, volatile i32* value)
{
  return OSAtomicCompareAndSwap32(old_value, new_value, value);
}

template<>
bool Atomic< nuMachAtomic >::casBarrier(i32 old_value, i32 new_value, volatile i32* value)
{
  return OSAtomicCompareAndSwap32Barrier(old_value, new_value, value);
}

template<>
bool Atomic< nuMachAtomic >::cas(i64 old_value, i64 new_value, volatile i64* value)
{
  return OSAtomicCompareAndSwap64(old_value, new_value, value);
}

template<>
bool Atomic< nuMachAtomic >::casBarrier(i64 old_value, i64 new_value, volatile i64* value)
{
  return OSAtomicCompareAndSwap64(old_value, new_value, value);
}

template<>
bool Atomic< nuMachAtomic >::cas(void* old_value, void* new_value, void* volatile *value)
{
  return OSAtomicCompareAndSwapPtr(old_value, new_value, value);
}

template<>
bool Atomic< nuMachAtomic >::casBarrier(void* old_value, void* new_value, void* volatile *value)
{
  return OSAtomicCompareAndSwapPtrBarrier(old_value, new_value, value);
}

template<>
bool Atomic< nuMachAtomic >::tas(ui32 n, volatile void* address)
{
  return OSAtomicTestAndSet(n, address);
}

template<>
bool Atomic< nuMachAtomic >::tasBarrier(ui32 n, volatile void* address)
{
  return OSAtomicTestAndSetBarrier(n, address);
}

template<>
bool Atomic< nuMachAtomic >::tac(ui32 n, volatile void* address)
{
  return OSAtomicTestAndClear(n, address);
}

template<>
bool Atomic< nuMachAtomic >::tacBarrier(ui32 n, volatile void* address)
{
  return OSAtomicTestAndClearBarrier(n, address);
}
