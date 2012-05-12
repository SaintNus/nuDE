/*!
 * \file nuDebug.h
 * \brief Debug functions.
 * \author Nus
 * \date 2010/02/26 23:45
 */

#ifndef __NUDEBUG_H__
#define __NUDEBUG_H__

//! \brief Assert.
#if NDEBUG || !DEBUG
#define NU_ASSERT(_cond, ...)
#else
#define NU_ASSERT(_cond, ...) \
    if(!(_cond)) nude::Assert(__FILE__, __LINE__, __VA_ARGS__)
#endif

//! \brief Assert.
#if NDEBUG || !DEBUG
#define NU_ASSERT_C(_cond)
#else
#define NU_ASSERT_C(_cond) \
    if(!(_cond)) nude::Assert(__FILE__, __LINE__, "Assertion failed!\n")
#endif

//! \brief Verify.
#if NDEBUG || !DEBUG
#define NU_VERIFY(_cond)
#else
#define NU_VERIFY(_cond) \
    nude::LogStr("Verify: " #_cond ", %s\n", (_cond) ? "true" : "false")
#endif

//! \brief Trace.
#if NDEBUG
#define NU_TRACE(...)
#else
#define NU_TRACE(...) nude::LogStr(__VA_ARGS__)
#endif

/*!
 * \namespace nude
 * \brief Nude global namespace.
 */
namespace nude
{

  //! \brief Log string.
  void LogStr(ccstr format, ...) __attribute__((format(printf, 1, 2)));
  //! \brief Assert.
  void Assert(ccstr file, ui32 line, ccstr format, ...) __attribute__((format(printf, 3, 4)));

}

#endif
