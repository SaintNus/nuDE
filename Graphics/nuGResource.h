/*!
 * \file nuGResource.h
 * \brief Graphic resource.
 * \author Nus
 * \date 2012/02/05 18:07
 */

#ifndef __NUGRESOURCE_H__
#define __NUGRESOURCE_H__

typedef nude::Handle< class nuGResource > nuGResourceHandle;

class nuGResource : public nuObject
{
  DECLARE_TYPE_INFO;
  friend class nude::Handle< class nuGResource >;

public:
  enum RESOURCE_TYPE {
    FRAME_BUFFER = 0,
    VERTEX_BUFFER,
    ELEMENT_BUFFER,
    TEXTURE,
  };

  virtual void release(void) {
    decRefCount();
  }

  RESOURCE_TYPE getType(void) const {
    return mType;
  }

protected:
  volatile i32 mRefCount;

  nuGResource(RESOURCE_TYPE type)
      : mRefCount(1),
        mType(type)
  {
    // None...
  }

  virtual ~nuGResource() = 0;

  virtual nuGResource* incRefCount(void) {
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

  virtual void decRefCount(void) {
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

private:
  RESOURCE_TYPE mType;
  nuGResource();

};

#endif
