/*!
 * \file nuAtomic.h
 * \brief Atomic functions.
 * \author Nus
 * \date 2011/11/26 23:52
 */

#ifndef __NUATOMIC_H__
#define __NUATOMIC_H__

namespace nude
{
  template< class T >
  class Atomic : public nuObject
  {
  public:
    static i32 add(i32 amount, volatile i32* value);
    static i32 addBarrier(i32 amount, volatile i32* value);
    static i64 add(i64 amount, volatile i64* value);
    static i64 addBarrier(i64 amount, volatile i64* value);

    static i32 inc(volatile i32* value);
    static i32 incBarrier(volatile i32* value);
    static i64 inc(volatile i64* value);
    static i64 incBarrier(volatile i64* value);

    static i32 dec(volatile i32* value);
    static i32 decBarrier(volatile i32* value);
    static i64 dec(volatile i64* value);
    static i64 decBarrier(volatile i64* value);

    static i32 logicOr(ui32 mask, volatile ui32* value);
    static i32 logicOrBarrier(ui32 mask, volatile ui32* value);
    static i32 logicOrOrg(ui32 mask, volatile ui32* value);
    static i32 logicOrBarrierOrg(ui32 mask, volatile ui32* value);

    static i32 logicAnd(ui32 mask, volatile ui32* value);
    static i32 logicAndBarrier(ui32 mask, volatile ui32* value);
    static i32 logicAndOrg(ui32 mask, volatile ui32* value);
    static i32 logicAndBarrierOrg(ui32 mask, volatile ui32* value);

    static i32 logicXor(ui32 mask, volatile ui32* value);
    static i32 logicXorBarrier(ui32 mask, volatile ui32* value);
    static i32 logicXorOrg(ui32 mask, volatile ui32* value);
    static i32 logicXorBarrierOrg(ui32 mask, volatile ui32* value);

    static bool cas(i32 old_value, i32 new_value, volatile i32* value);
    static bool casBarrier(i32 old_value, i32 new_value, volatile i32* value);
    static bool cas(i64 old_value, i64 new_value, volatile i64* value);
    static bool casBarrier(i64 old_value, i64 new_value, volatile i64* value);
    static bool cas(void* old_value, void* new_value, void* volatile *value);
    static bool casBarrier(void* old_value, void* new_value, void* volatile *value);

    static bool tas(ui32 n, volatile void* address);
    static bool tasBarrier(ui32 n, volatile void* address);

    static bool tac(ui32 n, volatile void* address);
    static bool tacBarrier(ui32 n, volatile void* address);

  private:
    Atomic();
    ~Atomic();
  };
}

#endif
