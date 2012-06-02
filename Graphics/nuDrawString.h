/*!
 * \file nuDrawString.h
 * \brief String drawer.
 * \author Nus
 * \date 2012/05/27 21:41
 */

#ifndef __NUDRAWSTRING_H__
#define __NUDRAWSTRING_H__

#include "nuRenderGL.h"

class nuDrawString : public nuObject
{
  DECLARE_TYPE_INFO;
  friend nuRenderGL;
  friend class _DrawStringResource;
  friend nude::Handle< nuDrawString >;

  void* mpDrawString;

  nuDrawString();
  nuDrawString(const nuRect& rect, f32 size);
  nuDrawString(const c8* str, f32 size);
  nuDrawString(const c8* str, const nuRect& rect, f32 size);
  nuDrawString(const c8* str, const nuSize& margin, f32 size);
  ~nuDrawString();

  void update(void);

  static void initialize(nuRenderGL& render_gl);
  static void terminate(void);
  static void updateResource(void);
  static void draw(nuRenderGL::RenderContext& render_context);

  nuDrawString* incRefCount(void) {
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

  volatile i32 mRefCount;
  ui32 mDrawString: 1;
  ui32 mRegistered: 1;
  ui32 mReserved: 30;
  nuColor mDrawColor;
  nuColor mColor;

public:
  bool isAntiAliased(void) const;
  nuSize getMargin(void) const;

  void enableAntiAlias(bool enable);
  void setMargin(const nuSize& size);

  const c8* getString(void) const;
  const nuColor& getColor(void) const;

  void setString(const c8* string);
  void setColor(const nuColor& color);

  void drawAt(const nuPoint& pos);

  static nude::Handle< nuDrawString > create(const nuRect& rect, f32 size = 12.0f);
  static nude::Handle< nuDrawString > create(const c8* string, f32 size = 12.0f);
  static nude::Handle< nuDrawString > create(const c8* string, const nuRect& rect, f32 size = 12.0f);
  static nude::Handle< nuDrawString > create(const c8* string, const nuSize& margin, f32 size = 12.0f);

};

namespace nude {
  typedef Handle< nuDrawString > DrawString;
}

#endif
