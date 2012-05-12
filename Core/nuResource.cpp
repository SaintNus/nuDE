/*!
 * \file nuResource.cpp
 * \brief Resource.
 * \author Nus
 * \date 2012/01/29 20:13
 */

#include "nuResourceManager.h"
#include "nuResource.h"

IMPLEMENT_TYPE_INFO_ABSTRACT(nuResource, nuObject);

nuResource::nuResource()
    : mRefCount(0),
      mPath(nullptr),
      mCRC(0),
      mpResourceManager(nullptr),
      mAttribute(0)
{
  // None...
}

nuResource::~nuResource()
{
  if(mpResourceManager)
    mpResourceManager->unregisterResource(this);

  if(mPath) {
    nude::Dealloc(mPath);
    mPath = nullptr;
  }
}

ccstr nuResource::getErrorStr(ERROR_CODE code) const
{
#if NDEBUG == 0
  ccstr err_str[] = {
    "None",
    "Resource not found",
    "Invalid resource",
    "Resource is corrupted",
    "Invalid version",
    "Insufficient memory",
  };
  return err_str[code];
#else
  return nullptr;
#endif
}
