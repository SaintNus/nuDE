/*!
 * \file Utility.h
 * \brief Utility functions.
 * \author Nus
 * \date 2011/12/29 23:37
 */

#include "Utility.h"
#include "StringTable.h"

extern "C" int yyget_lineno(void);

void yyerror(const char* err_str)
{
  fprintf(stderr, "Error at line-%d: %s\n", yyget_lineno(), err_str);
#if defined(DEBUG)
  assert(false);
#endif
}

int yyaddstr(const char* string)
{
  return StringTable::addString(string);
}
