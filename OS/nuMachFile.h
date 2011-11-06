/*!
 * \file nuMachFile.h
 * \brief File handler for MacOSX.
 * \author Nus
 * \date 2011/09/19 18:59
 */

#ifndef __NUMACHFILE_H__
#define __NUMACHFILE_H__

#include "nuOS.h"

class nuMachFile
{
public:
  nuMachFile()
      : mFileHandle(nullptr),
        mFileSize(0),
        mAttribute(nude::FATTR_READ)
  {
    // None...
  }
  ~nuMachFile() {
    close();
  }

  void* getHandle(void) const {
    return mFileHandle;
  }

  nude::FILE_ERROR open(nude::FILE_ATTRIBUTE attr, ccstr name);
  nude::FILE_ERROR close(void);

  size_t read(void* ptr, size_t bytes);
  size_t write(void* ptr, size_t bytes);
  nude::FILE_ERROR seek(i64 pos, nude::FILE_SEEK whence);
  i64 tell(void) const;

  size_t getSize(void) const {
    return mFileSize;
  }

private:
  void* mFileHandle;
  size_t mFileSize;
  nude::FILE_ATTRIBUTE mAttribute;
  
};

#endif
