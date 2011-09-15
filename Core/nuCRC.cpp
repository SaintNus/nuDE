/*!
 * \file nuCRC.h
 * \brief CRC generator.
 * \author Nus
 * \date 2010/04/17 02:33
 */

#include "nuDebug.h"
#include "nuCRC.h"

// CRC table
ui32 nuCRC::mCrcTable[256];

// Generate full CRC for data.
ui32 nuCRC::calculate(const void* p_data, size_t sz)
{
  ui32 ret = 0xffffffff; // Initilaize return value.
  partial(&ret, p_data, sz);
  return ret ^ 0xffffffff;
}

// Generate full CRC for string.
ui32 nuCRC::calculate(ccstr p_str)
{
  size_t len = strlen(p_str);
  return calculate(p_str, len);
}

// Initialize CRC table.
void nuCRC::initialize(void)
{
  NU_TRACE("Initializing CRC.\n");
  // 0x04C11DB7 is the official polynomial used by PKZip, WinZip and Ethernet.
  const ui32 polynom = 0x04c11dB7;
  // 256 values representing ASCII character codes.
  for(ui32 ii = 0; ii < 256; ii++) {
    mCrcTable[ii] = reflect(ii, 8) << 24;
    for(ui32 jj = 0; jj < 8; jj++)
      mCrcTable[ii] = (mCrcTable[ii] << 1) ^ ((mCrcTable[ii] & (1 << 31)) ? polynom : 0);
    mCrcTable[ii] = reflect(mCrcTable[ii], 32);
  }
}

// Terminate CRC table.
void nuCRC::terminate(void)
{
  NU_TRACE("Terminating CRC.\n");
}



// Reference counter.
ui32 nuCRCInit::mRefCnt = 0;

//! \brief Default constructor.
nuCRCInit::nuCRCInit()
{
  if(mRefCnt == 0)
    nuCRC::initialize();
  mRefCnt++;
}

//! \brief Default destructor.
nuCRCInit::~nuCRCInit()
{
  mRefCnt--;
  if(mRefCnt == 0)
    nuCRC::terminate();
}
