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
  
  template< class T >
  class File
  {
  public:
    File() {}
    File(FILE_ATTRIBUTE attr, ccstr name) {
      mFileHandler.open(attr, name);
    }
    ~File() {}

    FILE_ERROR open(FILE_ATTRIBUTE attr, ccstr name) {
      return mFileHandler.open(attr, name);
    }
    bool isOpened(void) const {
      return mFileHandler.getHandle() != nullptr;
    }
    FILE_ERROR close(void) {
      return mFileHandler.close();
    }
    size_t read(void* ptr, size_t bytes) {
      return read(ptr, bytes);
    }
    size_t write(void* ptr, size_t bytes) {
      return mFileHandler.write(ptr, bytes);
    }
    FILE_ERROR seek(i64 pos, FILE_SEEK whence) {
      return mFileHandler.seek(pos, whence);
    }
    FILE_ERROR rewind(void) {
      seek(0, FSEEK_SET);
    }
    i64 tell(void) const {
      return mFileHandler.tell();
    }
    
  private:
    T mFileHandler;
    
  };
  
}

#endif
