/*!
 * \file nuTexture.h
 * \brief Texture.
 * \author Nus
 * \date 2012/05/26 00:41
 */

#ifndef __NUTEXTURE_H__
#define __NUTEXTURE_H__

#include "nuGResource.h"
#include "nuGraphics.h"

class nuTexture : public nuGResource
{
  DECLARE_TYPE_INFO;
  friend class nuRenderGL;
  friend class nuGResManager;
  friend nude::Handle< nuTexture >;

public:
  enum TEXTURE_TYPE {
    TEXTURE_1D = GL_TEXTURE_1D,
    TEXTURE_2D = GL_TEXTURE_2D,
    TEXTURE_3D = GL_TEXTURE_3D,
  };

  struct Texture1D {
    ui32 width;
    nude::PIXEL_FORMAT format;
    ui32 border;
    bool compress;
    bool generate_mipmap;
    Texture1D(ui32 ww, nude::PIXEL_FORMAT fmt, bool comp, bool mipmap)
        : width(ww),
          format(fmt),
          compress(comp),
          generate_mipmap(mipmap)
    {
      // None...
    }
  };

  struct Texture2D {
    ui32 width;
    ui32 height;
    nude::PIXEL_FORMAT format;
    bool compress;
    bool generate_mipmap;
    Texture2D(ui32 ww, ui32 hh, nude::PIXEL_FORMAT fmt, bool comp, bool mipmap)
        : width(ww),
          height(hh),
          format(fmt),
          compress(comp),
          generate_mipmap(false)
    {
      // None...
    }
  };

  struct Texture3D {
    ui32 width;
    ui32 height;
    ui32 depth;
    nude::PIXEL_FORMAT format;
    ui32 border;
    bool compress;
    bool generate_mipmap;
    Texture3D(ui32 ww, ui32 hh, ui32 dd, nude::PIXEL_FORMAT fmt, bool comp, bool mipmap)
        : width(ww),
          height(hh),
          depth(dd),
          format(fmt),
          compress(comp),
          generate_mipmap(false)
    {
      // None...
    }
  };

  struct Parameter {
    nude::WRAP_TEXTURE wrap_s;
    nude::WRAP_TEXTURE wrap_t;
    nude::WRAP_TEXTURE wrap_r;
    nude::FILTER_TEXTURE min_filter;
    nude::FILTER_TEXTURE mag_filter;

    Parameter()
        : wrap_s(nude::WRAP_CLAMP_TO_EDGE),
          wrap_r(nude::WRAP_CLAMP_TO_EDGE),
          wrap_t(nude::WRAP_CLAMP_TO_EDGE),
          min_filter(nude::FILTER_LINEAR),
          mag_filter(nude::FILTER_LINEAR)
    {
      // None...
    }

    Parameter(nude::WRAP_TEXTURE ws, nude::WRAP_TEXTURE wt, nude::WRAP_TEXTURE wr,
              nude::FILTER_TEXTURE min_f, nude::FILTER_TEXTURE mag_f)
        : wrap_s(ws),
          wrap_r(wt),
          wrap_t(wr),
          min_filter(min_f),
          mag_filter(mag_f)
    {
      // None...
    }
  };

private:
  enum EXTENSION_FLAG {
    COMPRESSED = 1 << 0,
    GENERATE_MIPMAP = 1 << 1,
  };

  struct PixelFormat {
    GLenum type;
    size_t element_size;
    ui32 component;
    GLenum format;
    GLenum internal_format;
    GLenum compressed_format;
  };

  static const PixelFormat mPixelFormatTable[nude::PIXEL_FORMAT_NUM];

  GLuint mHandle;
  void* mpBuffer;
  size_t mSize;

  TEXTURE_TYPE mTextureType;

  ui32 mWidth;
  ui32 mHeight;
  ui32 mDepth;
  nude::PIXEL_FORMAT mFormat;

  nude::WRAP_TEXTURE mWrapS;
  nude::WRAP_TEXTURE mWrapT;
  nude::WRAP_TEXTURE mWrapR;
  nude::FILTER_TEXTURE mMinFilter;
  nude::FILTER_TEXTURE mMagFilter;

  void update(void);
  void releaseBuffer(void) {
    if(mpBuffer) {
      nude::Dealloc(mpBuffer);
      mpBuffer = nullptr;
    }
  }

  void setCompressed(bool compressed) {
    ui32 ext = getExtension();
    if(compressed)
      ext |= COMPRESSED;
    else
      ext &= ~COMPRESSED;
    setExtension(ext);
  }

  bool isCompressed(void) const {
    return (getExtension() & COMPRESSED) ? true : false;
  }

  void setGenerateMipMap(bool mipmap) {
    ui32 ext = getExtension();
    if(mipmap)
      ext |= GENERATE_MIPMAP;
    else
      ext &= ~GENERATE_MIPMAP;
    setExtension(ext);
  }

  bool isGenerateMipMap(void) const {
    return (getExtension() & GENERATE_MIPMAP) ? true : false;
  }

  nuTexture();
  nuTexture(nuGResource::RESOURCE_USAGE usage);
  ~nuTexture();

public:
  GLuint getHandle(void) const {
    return mHandle;
  }

  void initialize(const Texture1D& tex_1d, const Parameter& parameter = Parameter());
  void initialize(const Texture2D& tex_2d, const Parameter& parameter = Parameter());
  void initialize(const Texture3D& tex_3d, const Parameter& parameter = Parameter());

  void* beginInitialize(const Texture1D& tex_1d, const Parameter& parameter = Parameter());
  void* beginInitialize(const Texture2D& tex_2d, const Parameter& parameter = Parameter());
  void* beginInitialize(const Texture3D& tex_3d, const Parameter& parameter = Parameter());
  void endInitialize(void);

  size_t getSize(void) const {
    return mSize;
  }

  TEXTURE_TYPE getType(void) const {
    return mTextureType;
  }

  void* beginUpdate(void);
  void endUpdate(void);

  bool isMipMapped(void) const {
    return isGenerateMipMap();
  }

};

namespace nude {
  typedef Handle< nuTexture > Texture;
}

#endif
