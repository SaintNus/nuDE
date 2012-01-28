/*!
 * \file Utility.h
 * \brief Utility functions.
 * \author Nus
 * \date 2011/12/29 23:37
 */

#ifndef __UTILITY_H__
#define __UTILITY_H__

enum ErrorCodes
{
  ErrorNone = 0,
  ErrorAppStartup,
  ErrorIO,
  ErrorFileSystem,
  ErrorShdListParse,
  ErrorMemory,
  ErrorException,
  ErrorShaderCompile,
  ErrorProgramLink,
};

#if defined(__cplusplus)
extern "C"
{
#endif

void yyerror(const char* err_string);
int yyaddstr(const char* string);

int yyget_iserror(void);

FILE* OutputHeader(void);
FILE* OutputSource(void);
void SetOutputHeader(FILE* p_file);
void SetOutputSource(FILE* p_file);

void DumpList(void);

int MakePath(const char* path);

int BuildList(const char* intermediate_dir);

#if defined(__cplusplus)
}
#endif

#endif
