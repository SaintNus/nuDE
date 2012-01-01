/*!
 * \file Utility.h
 * \brief Utility functions.
 * \author Nus
 * \date 2011/12/29 23:37
 */

#ifndef __UTILITY_H__
#define __UTILITY_H__

#if defined(__cplusplus)
extern "C"
{
#endif

void yyerror(const char* err_string);
int yyaddstr(const char* string);

int yyget_iserror(void);

void DumpList(void);
void BuildList(void);

#if defined(__cplusplus)
}
#endif

#endif
