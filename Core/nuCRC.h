/*!
 * \file nuCRC.h
 * \brief CRC generator.
 * \author Nus
 * \date 2010/04/17 02:33
 */

#ifndef __NUCRC_H__
#define __NUCRC_H__

/*!
 * \class nuCRC
 * \brief CRC generator.
 */
class nuCRC
{
  friend class nuCRCInit;

public:
  //! \brief Generate full CRC for data.
  static ui32 calculate(const void* p_data, size_t sz);

  //! \brief Generate full CRC for string.
  static ui32 calculate(ccstr p_str);

private:
  static ui32 mCrcTable[256];            //!< CRC table.

  //! \brief Initialize CRC table.
  static void initialize(void);
  //! \brief Terminate CRC table.
  static void terminate(void);

  //! \brief Reflect.
  static ui32 reflect(ui32 i_reflect, i8 i_ch) {
    ui32 ret = 0;
    i8 inc_ch = i_ch + 1;
    // Reflect bits.
    for(i32 ii = 1; ii < inc_ch; ii++) {
      if(i_reflect & 1) {
        ret |= (1 << (i_ch - ii));
      }
      i_reflect >>= 1;
    }
    return ret;
  }

  //! \brief Calculate partial CRC.
  static void partial(ui32* p_crc, const void* p_data, size_t sz) {
    const ui8* p_ch = (const ui8*) p_data;
    while(sz--)
      *p_crc = (*p_crc >> 8) ^ mCrcTable[(*p_crc & 0xff) ^ *p_ch++];
  }

  //! \brief Default constructor.
  //! \warning Do not use.
  nuCRC();
  //! \brief Default destructor.
  //! \warning Do not use.
  ~nuCRC();

};



/*!
 * \class nuCRCInit
 * \brief CRC initializer.
 */
static class nuCRCInit
{
public:
  //! \brief Default constructor.
  nuCRCInit();
  //! \brief Default destructor.
  ~nuCRCInit();
private:
  static ui32 mRefCnt;            //!< Reference counter.
} CrcInit;

#endif
