/*!
 * \file nuOS.h
 * \brief OS common definition.
 * \author Nus
 * \date 2011/11/06 09:31
 */

#ifndef __NUOS_H__
#define __NUOS_H__

namespace nude
{
  enum FILE_ERROR {
    FERROR_CRITICAL = -1,
    FERROR_NONE = 0,
    FERROR_CANNOT_OPEN,
    FERROR_INVALID_OPERATION,
  };
  
  enum FILE_SEEK {
    FSEEK_SET = SEEK_SET,
    FSEEK_CURRENT = SEEK_CUR,
    FSEEK_END = SEEK_END,
  };

  enum FILE_ATTRIBUTE {
    FATTR_READ = 0,
    FATTR_WRITE,
    FATTR_UPDATE,
  };

}


#endif
