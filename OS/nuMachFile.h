/*!
 * \file nuMachFile.h
 * \brief File handler for MacOSX.
 * \author Nus
 * \date 2011/09/19 18:59
 */

#ifndef __NUMACHFILE_H__
#define __NUMACHFILE_H__

#include "nuOS.h"

/*!
 * \class nuMachFile
 * \brief File handler for MacOSX.
 */
class nuMachFile
{
public:
  //! \brief Default constructor.
  nuMachFile()
      : mFileHandle(nullptr),
        mFileSize(0),
        mAttribute(nude::FATTR_READ)
  {
    // None...
  }
  //! \brief Default destructor.
  ~nuMachFile() {
    close();
  }

  //! \brief Get file handle.
  void* getHandle(void) const {
    return mFileHandle;
  }

  //! \brief Open a file.
  nude::FILE_ERROR open(nude::FILE_ATTRIBUTE attr, ccstr name);
  //! \brief Open a file.
  nude::FILE_ERROR open(nude::FILE_ATTRIBUTE attr, wccstr name);
  //! \brief Close a file.
  nude::FILE_ERROR close(void);

  //! \brief Read file content.
  size_t read(void* ptr, size_t bytes);
  //! \brief Write file content.
  size_t write(void* ptr, size_t bytes);
  //! \brief Seek file.
  nude::FILE_ERROR seek(i64 pos, nude::FILE_SEEK whence);
  //! \brief Tell file position.
  i64 tell(void) const;

  //! \brief Get file size.
  size_t getSize(void) const {
    return mFileSize;
  }

  //! \brief Get file attribute.
  nude::FILE_ATTRIBUTE getAttribute(void) const {
    return mAttribute;
  }

private:
  void* mFileHandle;              //!< File handle.
  size_t mFileSize;               //!< File size.
  nude::FILE_ATTRIBUTE mAttribute; //!< File attribute.

};

#endif
