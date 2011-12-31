/*!
 * \file StringTable.h
 * \brief String table.
 * \author Nus
 * \date 2011/12/29 23:14
 */

#ifndef __STRING_TABLE_H__
#define __STRING_TABLE_H__

class StringTable
{
private:
  static StringTable mInstance;

  unsigned int mEntityNum;
  unsigned int mTableSize;
  char** mpTable;

  StringTable();
  ~StringTable();

public:
  static int addString(const char* string);
  static const char* getString(unsigned int index);

};

#endif
