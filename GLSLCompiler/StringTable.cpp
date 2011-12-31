/*!
 * \file StringTable.cpp
 * \brief String table.
 * \author Nus
 * \date 2011/12/29 23:14
 */

#include "StringTable.h"
#include "Utility.h"

StringTable StringTable::mInstance;

StringTable::StringTable()
    : mEntityNum(0),
      mTableSize(0),
      mpTable(nullptr)
{
  // None...
}

StringTable::~StringTable()
{
  if(mpTable) {
    for(unsigned int ui = 0; ui < mEntityNum; ui++)
      free(mpTable[ui]);
    free(mpTable);
  }
}

int StringTable::addString(const char* string)
{
  int num = mInstance.mEntityNum + 1;
  if(mInstance.mTableSize < num) {
    char** p_curr = mInstance.mpTable;
    size_t sz = sizeof(char*) * (mInstance.mTableSize + 1024);
    mInstance.mpTable = static_cast< char** >(malloc(sz));
    if(!mInstance.mpTable) {
      yyerror("Cannot allocate memory for string constant.");
      return 0;
    }
    if(p_curr) {
      memcpy(mInstance.mpTable, p_curr, sizeof(char*) * mInstance.mTableSize);
      free(p_curr);
    }
    mInstance.mTableSize += 1024;
  }
  mInstance.mpTable[mInstance.mEntityNum] = static_cast< char* >(malloc(strlen(string) + 1));
  strcpy(mInstance.mpTable[mInstance.mEntityNum], string);
  mInstance.mEntityNum = num;
  return num - 1;
}

const char* StringTable::getString(unsigned int index)
{
  if(index >= mInstance.mEntityNum)
    return nullptr;
  return mInstance.mpTable[index];
}
