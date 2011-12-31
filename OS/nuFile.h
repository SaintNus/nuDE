/*!
 * \file nuFile.h
 * \brief File handler.
 * \author Nus
 * \date 2011/09/19 02:46
 */

#ifndef __NUFILE_H__
#define __NUFILE_H__

#include "nuOS.h"

namespace nude {

  /*!
   * \class File
   * \brief Template for file handler class.
   */
  template< class FileHandler >
  class File : public nuObject
  {
  public:
    //! \brief Default constructor.
    File() {}
    //! \brief Default constructor.
    File(FILE_ATTRIBUTE attr, ccstr name) {
      mFileHandler.open(attr, name);
    }
    //! \brief Default constructor.
    File(FILE_ATTRIBUTE attr, wccstr name) {
      mFileHandler.open(attr, name);
    }
    //! \brief Default destructor.
    ~File() {}

    //! \brief Open a file.
    FILE_ERROR open(FILE_ATTRIBUTE attr, ccstr name) {
      return mFileHandler.open(attr, name);
    }
    //! \brief Open a file.
    FILE_ERROR open(FILE_ATTRIBUTE attr, wccstr name) {
      return mFileHandler.open(attr, name);
    }
    //! \brief Is a file opened.
    bool isOpened(void) const {
      return mFileHandler.getHandle() != nullptr;
    }
    //! \brief Close and terminate file handle.
    FILE_ERROR close(void) {
      return mFileHandler.close();
    }
    //! \brief Read file content.
    size_t read(void* ptr, size_t bytes) {
      return mFileHandler.read(ptr, bytes);
    }
    //! \brief Write to file.
    size_t write(void* ptr, size_t bytes) {
      return mFileHandler.write(ptr, bytes);
    }
    //! \brief Seek.
    FILE_ERROR seek(i64 pos, FILE_SEEK whence) {
      return mFileHandler.seek(pos, whence);
    }
    //! \brief Rewind to beginning of file.
    FILE_ERROR rewind(void) {
      mFileHandler.seek(0, FSEEK_SET);
    }
    //! \brief Tell current position.
    i64 tell(void) const {
      return mFileHandler.tell();
    }

    //! \brief Get size.
    size_t getSize(void) const {
      return mFileHandler.getSize();
    }

    //! \brief Get file attribute.
    nude::FILE_ATTRIBUTE getAttribute(void) const {
      mFileHandler.getAttribute();
    }

  private:
    FileHandler mFileHandler;     //!< File handler.
    
  };

}

#endif
