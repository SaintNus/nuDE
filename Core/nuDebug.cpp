/*!
 * \file nuDebug.cpp
 * \brief Debug functions.
 * \author Nus
 * \date 2010/02/26 23:45
 */

#include "nuDebug.h"

// Log string.
void nude::LogStr(ccstr format, ...)
{
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}

// Assert.
void nude::Assert(ccstr file, ui32 line, ccstr format, ...)
{
  va_list args;

  LogStr("Assert: %s (%d)\n", file, line);

  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  
  assert(false);
}
