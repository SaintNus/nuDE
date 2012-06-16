/*!
 * \file nuRenderBuffer.h
 * \brief Render buffer.
 * \author Nus
 * \date 2012/06/03 22:29
 */

#ifndef __NURENDERBUFFER_H__
#define __NURENDERBUFFER_H__

#include "nuGResource.h"
#include "nuGraphics.h"

class nuRenderBuffer : public nuGResource
{
  DECLARE_TYPE_INFO;
  friend class nuGResManager;
  friend nude::Handle< nuRenderBuffer >;

  GLuint mHandle;
  nude::RENDERBUFFER_FORMAT mInternalFormat;
  ui32 mWidth;
  ui32 mHeight;

  void update(void);

  nuRenderBuffer();
  ~nuRenderBuffer();

public:
  GLuint getHandle(void) const {
    return mHandle;
  }

  void initialize(nude::RENDERBUFFER_FORMAT internal_format, ui32 width, ui32 height) {
    mInternalFormat = internal_format;
    mWidth = width;
    mHeight = height;
    setInitialized(true);
    setUpdate(true);
  }

  nude::RENDERBUFFER_FORMAT getFormat(void) const {
    return mInternalFormat;
  }

  ui32 getWidth(void) const {
    return mWidth;
  }

  ui32 getHeight(void) const {
    return mHeight;
  }

};

namespace nude {
  typedef nude::Handle< nuRenderBuffer > RenderBuffer;
}

#endif
