/*!
 * \file StringTable.h
 * \brief String table.
 * \author Nus
 * \date 2011/12/29 23:14
 */

#ifndef __STRING_TABLE_H__
#define __STRING_TABLE_H__

#include <vector>

class String {
private:
  unsigned int mLineNo;
  char* mpString;

public:
  String(unsigned int ln, const char* string)
      : mLineNo(ln)
  {
    size_t len = strlen(string);
    if(len > 0) {
      mpString = static_cast< char* >(malloc(len + 1));
      memcpy(mpString, string, len + 1);
    } else {
      mpString = nullptr;
    }
  }
  ~String() {
    if(mpString)
      free(mpString);
  }

  unsigned int lineNo(void) const {
    return mLineNo;
  }

  const char* string(void) const {
    return mpString;
  }

};

class StringTable
{
private:
  static StringTable mInstance;
  std::vector< String* > mStringTable;

  StringTable();
  ~StringTable();

public:
  static int addString(unsigned int line_no, const char* string);
  static const String* getString(unsigned int index);

};

#endif
