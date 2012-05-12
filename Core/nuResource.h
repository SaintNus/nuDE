/*!
 * \file nuResource.h
 * \brief Resource.
 * \author Nus
 * \date 2012/01/29 20:13
 */

#ifndef __NURESOURCE_H__
#define __NURESOURCE_H__

#include "nuObject.h"
#include "nuOSType.h"
#include "nuHandle.h"
#include "nuStream.h"

typedef nude::Handle< class nuResource > nuResHandle;

/*!
 * \class nuResource
 * \brief Resource class.
 */
class nuResource : public nuObject
{
  DECLARE_TYPE_INFO;
  friend class nude::Handle< class nuResource >;
  friend class nuResourceManager;

public:
  enum ERROR_CODE {
    ERROR_NONE = 0,
    ERROR_NOT_FOUND,
    ERROR_INVALID,
    ERROR_CORRUPTED,
    ERROR_INVALID_VERSION,
    ERROR_INSUFFICIENT_MEMORY,
  };

private:
  volatile i32 mRefCount;
  cstr mPath;
  ui32 mCRC;
  class nuResourceManager* mpResourceManager;

  union {
    ui32 mAttribute;
    struct {
      ui32 mInitialized: 1;
      ui32 mErrorCode: 6;
      ui32 mReserved: 25;
    };
  };

  void setPath(ccstr path, ui32 crc) {
    size_t len = strlen(path) + 1;
    mPath = static_cast< c8* >(nude::Alloc(len));
    strncpy(mPath, path, len);
    mCRC = crc;
  }

  nuResource* incRefCount(void) {
    i32 curr = mRefCount;
    i32 res = curr + 1;
    while(1) {
      bool ret = nuAtomic::casBarrier(curr, res, &mRefCount);
      if(!ret) {
        curr = mRefCount;
        res = curr + 1;
        if(curr < 1)
          return nullptr;
      } else {
        return this;
      }
    }
  }

  void decRefCount(void) {
    i32 curr = mRefCount;
    i32 res = curr - 1;
    while(1) {
      bool ret = nuAtomic::casBarrier(curr, res, &mRefCount);
      if(!ret) {
        curr = mRefCount;
        res = curr - 1;
      } else {
        if(mRefCount == 0) {
          curr = mRefCount;
          res = curr - 1;
          while(res < 0) {
            ret = nuAtomic::casBarrier(curr, res, &mRefCount);
            if(!ret) {
              curr = mRefCount;
              res = curr - 1;
            } else {
              delete this;
              return;
            }
          }
        }
        return;
      }
    }
  }

protected:
  nuResource();
  virtual ~nuResource();

  virtual ccstr getExt(void) const = 0;
  virtual ERROR_CODE setup(const nuStream& stream) = 0;

public:
  bool isInitialized(void) const {
    return mInitialized == 1;
  }

  ERROR_CODE getErrorCode(void) const {
    return static_cast< ERROR_CODE >(mErrorCode);
  }

  ccstr getErrorStr(ERROR_CODE code) const;
};

#endif
