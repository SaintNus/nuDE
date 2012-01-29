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

typedef nude::Handle< class nuResource > nuResHandle;

/*!
 * \class nuResource
 * \brief Resource class.
 */
class nuResource : public nuObject
{
  DECLARE_TYPE_INFO;
  friend nuResHandle;

public:
  enum ERROR_CODE {
    ERROR_NONE = 0,
    ERROR_INVALID_DATA,
    ERROR_CORRUPTED,
  };

  nuResource()
      : mAttribute(0),
        mRefCount(1),
        mPath(nullptr)
  {
    // None...
  }

  virtual ~nuResource() {
    if(mPath) {
      nude::Dealloc(mPath);
      mPath = nullptr;
    }
  }

  virtual ccstr getExt(void) const = 0;
  virtual ERROR_CODE setup(const void* p_data, size_t size) = 0;

  static nuResHandle createResource(nuResource* p_res) {
    return nuResHandle(p_res);
  }

private:
  ui32 mAttribute;
  i32 mRefCount;
  cstr mPath;

  void setPath(ccstr path) {
    size_t len = strlen(path) + 1;
    mPath = static_cast< c8* >(nude::Alloc(len));
    strncpy(mPath, path, len);
  }

  nuResource* incRefCount(void) {
    i32 curr = mRefCount;
    i32 res = curr + 1;
    while(1) {
      bool ret = nuAtomic::cas(curr, res, &mRefCount);
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
      bool ret = nuAtomic::cas(curr, res, &mRefCount);
      if(!ret) {
        curr = mRefCount;
        res = curr - 1;
      } else {
        if(mRefCount == 0) {
          curr = mRefCount;
          res = curr - 1;
          while(res < 0) {
            ret = nuAtomic::cas(curr, res, &mRefCount);
            if(!ret) {
              curr = mRefCount;
              res = curr - 1;
            } else {
              delete this;
              return;
            }
          }
        }
      }
    }
  }

};

#endif
