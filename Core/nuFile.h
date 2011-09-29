/*!
 * \file nuFile.h
 * \brief File handler.
 * \author Nus
 * \date 2011/09/19 02:46
 */

#ifndef __NUFILE_H__
#define __NUFILE_H__

#include "nuType.h"

namespace nude {

  template< class T >
  class File
  {
  public:
    enum ERROR_CODE {
      CRITICAL = -1,
      NO_ERROR = 0,
    };

    enum SEEK {
      SET = SEEK_SET,
      CURRENT = SEEK_CUR,
      END = SEEK_END,
    };
    
    File() {
      // None...
    }
    ~File() {
      close();
    }

    ERROR_CODE open(ccstr name) {
      return mFileHandler.open(name);
    }
    bool isOpened(void) const {
      return mFileHandler.getFileHandle() != NULL;
    }
    ERROR_CODE close(void) {
      return mFileHandler.close();
    }
    size_t read(void* ptr, size_t bytes) {
      return read(ptr, bytes);
    }
    size_t write(void* ptr, size_t bytes) {
      return mFileHandler.write(ptr, bytes);
    }
    ERROR_CODE seek(i64 pos, SEEK whence) {
      return mFileHandler.seek(pos, whence);
    }
    ERROR_CODE rewind(void) {
      seek(0, SET);
    }
    i64 tell(void) const {
      return mFileHandler.tell();
    }
    
  private:
    T mFileHandler;
    
  };
  
}

#endif
