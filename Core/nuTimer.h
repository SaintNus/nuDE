/*!
 * \file nuTimer.h
 * \brief Timer implementation.
 * \author Nus
 * \date 2012/06/03 12:18
 */

#ifndef __NUTIMER_H__
#define __NUTIMER_H__

namespace nude
{

  template< class T >
  class timer
  {
    timeval mTimeStart;

  public:
    timer() {
      reset();
    }

    ~timer() {}

    void reset(void) {
      gettimeofday(&mTimeStart, nullptr);
    }

    f32 getElapsedTime(void) const {
      T et;
      timeval t_end;
      gettimeofday(&t_end, nullptr);
      return et.getTime(mTimeStart, t_end);
    }

  };

  class Sec
  {
    friend class timer< Sec >;

    Sec() {}
    ~Sec() {}
    f32 getTime(const timeval& t1, const timeval& t2) {
      f64 elapsed_time = static_cast< f64 >(t2.tv_sec - t1.tv_sec);
      elapsed_time += static_cast< f64 >(t2.tv_usec - t1.tv_usec) / 1.0e-6;
      return static_cast< f32 >(elapsed_time);
    }
  };

  class mSec
  {
    friend class timer< mSec >;

    mSec() {}
    ~mSec() {}
    f32 getTime(const timeval& t1, const timeval& t2) {
      f64 elapsed_time = static_cast< f64 >(t2.tv_sec - t1.tv_sec) * 1000.0;
      elapsed_time += static_cast< f64 >(t2.tv_usec - t1.tv_usec) / 1000.0;
      return static_cast< f32 >(elapsed_time);
    }
  };

  class uSec
  {
    friend class timer< uSec >;

    uSec() {}
    ~uSec() {}
    f32 getTime(const timeval& t1, const timeval& t2) {
      f64 elapsed_time = static_cast< f64 >(t2.tv_sec - t1.tv_sec) * 1.0e6;
      elapsed_time += static_cast< f64 >(t2.tv_usec - t1.tv_usec);
      return static_cast< f32 >(elapsed_time);
    }
  };

}

typedef nude::timer< nude::Sec > nuTimerSec;
typedef nude::timer< nude::mSec > nuTimermSec;
typedef nude::timer< nude::uSec > nuTimeruSec;

#endif
