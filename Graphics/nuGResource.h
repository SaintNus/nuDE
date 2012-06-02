/*!
 * \file nuGResource.h
 * \brief Graphic resource.
 * \author Nus
 * \date 2012/02/05 18:07
 */

#ifndef __NUGRESOURCE_H__
#define __NUGRESOURCE_H__

class nuGResource : public nuObject
{
  DECLARE_TYPE_INFO;
  friend class nuGResManager;

public:
  enum RESOURCE_TYPE {
    FRAME_BUFFER = 0,
    VERTEX_BUFFER,
    ELEMENT_BUFFER,
    VERTEX_ARRAY,
    TEXTURE,
    SHADER_PROGRAM,
    UNIFORM_BUFFER,
  };

  enum RESOURCE_USAGE {
    STATIC_RESOURCE = 0,
    DYNAMIC_RESOURCE,
    STREAM_RESOURCE
  };

  GLenum getResourceUsage(void) const {
    const GLenum usage_tbl[] = {
      GL_STATIC_DRAW,
      GL_DYNAMIC_DRAW,
      GL_STREAM_DRAW,
    };
    return usage_tbl[mUsage];
  }

  virtual void release(void) {
    decRefCount();
  }

  RESOURCE_TYPE getType(void) const {
    return mType;
  }

  RESOURCE_USAGE getUsage(void) const {
    return mUsage;
  }

  bool protect(i64 frame_id) {
    i64 sfid = mFrameID;
    i64 dfid = frame_id;

    while(sfid < dfid) {
      bool ret = nuAtomic::casBarrier(sfid, dfid, &mFrameID);
      if(!ret) {
        sfid = mFrameID;
        dfid = frame_id;
      } else {
        return true;
      }
    }
    return false;
  }

protected:
  volatile i32 mRefCount;

  nuGResource(RESOURCE_TYPE type, RESOURCE_USAGE usage)
      : mRefCount(1),
        mType(type),
        mUsage(usage),
        mAttribute(0),
        mFrameID(0),
        mpGResManager(nullptr)
  {
    // None.
  }

  virtual ~nuGResource() = 0;

  nuGResource* incRefCount(void) {
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

  bool decRefCount(void) {
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
              return true;
            }
          }
        }
        return false;
      }
    }
  }

  virtual void update(void) = 0;

  bool isInitialized(void) const {
    return mInitialized == 1;
  }

  void setInitialized(bool initialized) {
    mInitialized = initialized ? 1 : 0;
  }

  bool isUpdated(void) const {
    return mUpdated == 1;
  }

  void setUpdate(bool update) {
    mUpdated = update ? 1 : 0;
  }

  ui32 getExtension(void) const {
    return mExtension;
  }

  void setExtension(ui32 extension) {
    mExtension = extension;
  }

  class nuGResManager& getGResManager(void) const {
    return *mpGResManager;
  }

private:
  RESOURCE_TYPE mType;
  RESOURCE_USAGE mUsage;
  class nuGResManager* mpGResManager;
  volatile i64 mFrameID;
  union {
    ui32 mAttribute;
    struct {
      ui32 mInitialized: 1;
      ui32 mUpdated: 1;
      ui32 mExtension: 30;
    };
  };

  nuGResource();

};

#endif
