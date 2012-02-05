/*!
 * \file nuGLResource.h
 * \brief GL resource.
 * \author Nus
 * \date 2012/02/05 18:07
 */

#ifndef __NUGLRESOURCE_H__
#define __NUGLRESOURCE_H__

typedef nude::Handle< class nuGLResource > nuGLResHandle;

class nuGLResource : public nuObject
{
  DECLARE_TYPE_INFO;
  friend nuGLResHandle;

public:
  enum RESOURCE_TYPE {
    FRAME_BUFFER = 0,
    VERTEX_BUFFER,
    ELEMENT_BUFFER,
    TEXTURE,
  };

  nuGLResource(RESOURCE_TYPE type)
      : mRefCount(1),
        mType(type)
  {
    // None...
  }

  virtual ~nuGLResource() = 0;

  virtual void release(void) {
    decRefCount();
  }

protected:
  i32 mRefCount;

  virtual nuGLResource* incRefCount(void) {
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
  nuGLResource();

};

#endif
