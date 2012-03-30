/*!
 * \file nuGResource.h
 * \brief Graphic resource.
 * \author Nus
 * \date 2012/02/05 18:07
 */

#ifndef __NUGRESOURCE_H__
#define __NUGRESOURCE_H__

typedef nude::Handle< class nuGResource > nuGResHandle;

class nuGResource : public nuObject
{
  DECLARE_TYPE_INFO;
  friend class nude::Handle< class nuGResource >;
  friend class nuGResManager;

public:
  enum RESOURCE_TYPE {
    FRAME_BUFFER = 0,
    RENDER_TARGET,
    VERTEX_BUFFER,
    ELEMENT_BUFFER,
    TEXTURE,
  };

  enum RESOURCE_USAGE {
    STATIC_RESOURCE = 0,
    DYNAMIC_RESOURCE,
  };

  virtual void release(void) {
    decRefCount();
  }

  RESOURCE_TYPE getType(void) const {
    return mType;
  }

  RESOURCE_USAGE getUsage(void) const {
    return mUsage;
  }

protected:
  volatile i32 mRefCount;

  nuGResource(RESOURCE_TYPE type, RESOURCE_USAGE usage)
      : mRefCount(1),
        mType(type),
        mUsage(usage),
        mAttribute(0)
  {
    // None...
  }

  virtual ~nuGResource() = 0;

  nuGResource* incRefCount(void) {
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

  virtual void preUpdate(void) {
    // None...
  }

  virtual void update(void) = 0;

  virtual void postUpdate(void) {
    if(mUpdated == 1)
      mUpdated = 0;
    if(mInitialized == 0)
      mInitialized = 1;
  }

  bool isInitialized(void) const {
    return mInitialized == 1;
  }

  bool isUpdated(void) const {
    return mUpdated == 1;
  }

  nuGResource() {}

private:
  RESOURCE_TYPE mType;
  RESOURCE_USAGE mUsage;
  union {
    ui32 mAttribute;
    struct {
      ui32 mInitialized: 1;
      ui32 mUpdated: 1;
      ui32 mReserved: 30;
    };
  };

};

#endif
