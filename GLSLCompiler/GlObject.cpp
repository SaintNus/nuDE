/*!
 * \file GlObject.cpp
 * \brief GL object.
 * \author Nus
 * \date 2012/01/01 20:55
 */

#include "GlObject.h"

GlObject::GlObject()
    : mContext(nullptr)
{
  const int attr[] = {
    kCGLPFADoubleBuffer,
    kCGLPFAAccelerated,
    kCGLPFAColorSize, 32,
    kCGLPFAAlphaSize, 8,
    kCGLPFADepthSize, 24,
    kCGLPFAStencilSize, 8,
    kCGLPFAOpenGLProfile,
    kCGLOGLPVersion_3_2_Core,
    0
  };
  CGLPixelFormatObj pix;
  GLint npix;

  CGLChoosePixelFormat(reinterpret_cast< const CGLPixelFormatAttribute* >(attr),
                       &pix,
                       &npix);
  if(pix) {
    CGLCreateContext(pix, nullptr, &mContext);
    CGLReleasePixelFormat(pix);
    if(mContext)
      CGLSetCurrentContext(mContext);
  }
}

GlObject::~GlObject()
{
  if(mContext)
    CGLReleaseContext(mContext);
}
