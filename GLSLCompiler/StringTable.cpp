/*!
 * \file StringTable.cpp
 * \brief String table.
 * \author Nus
 * \date 2011/12/29 23:14
 */

#include "StringTable.h"
#include "Utility.h"

using namespace std;

StringTable StringTable::mInstance;

StringTable::StringTable()
{
  // None...
}

StringTable::~StringTable()
{
  vector< String* >::iterator it;
  for(it = mStringTable.begin(); it < mStringTable.end(); it++) {
    delete *it;
    *it = nullptr;
  }
  mStringTable.clear();
}

int StringTable::addString(unsigned int line_no, const char* string)
{
  mInstance.mStringTable.push_back(new String(line_no, string));
  return static_cast< int >(mInstance.mStringTable.size() - 1);
}

const String* StringTable::getString(unsigned int index)
{
  if(index >= mInstance.mStringTable.size())
    return nullptr;
  return mInstance.mStringTable[index];
}
