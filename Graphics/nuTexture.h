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
    union {
      nude::PIXEL_FORMAT format;
      nude::TEXTUREBUFFER_FORMAT buffer_format;
    };
    bool compress;
    bool generate_mipmap;
    bool target_texture;
    Texture1D(ui32 ww, nude::PIXEL_FORMAT fmt, bool comp, bool mipmap)
        : width(ww),
          format(fmt),
          compress(comp),
          generate_mipmap(mipmap),
          target_texture(false)
    {
      // None...
    }
    Texture1D(ui32 ww, nude::TEXTUREBUFFER_FORMAT fmt, bool mipmap)
        : width(ww),
          buffer_format(fmt),
          generate_mipmap(mipmap),
          target_texture(true)
    {
      // None...
    }
  };

  struct Texture2D {
    ui32 width;
    ui32 height;
    union {
      nude::PIXEL_FORMAT format;
      nude::TEXTUREBUFFER_FORMAT buffer_format;
    };
    bool compress;
    bool generate_mipmap;
    bool target_texture;
    Texture2D(ui32 ww, ui32 hh, nude::PIXEL_FORMAT fmt, bool comp, bool mipmap)
        : width(ww),
          height(hh),
          format(fmt),
          compress(comp),
          generate_mipmap(mipmap),
          target_texture(false)
    {
      // None...
    }
    Texture2D(ui32 ww, ui32 hh, nude::TEXTUREBUFFER_FORMAT fmt, bool mipmap)
        : width(ww),
          height(hh),
          buffer_format(fmt),
          generate_mipmap(mipmap),
          target_texture(true)
    {
      // None...
    }
  };

  struct Texture3D {
    ui32 width;
    ui32 height;
    ui32 depth;
    union {
      nude::PIXEL_FORMAT format;
      nude::TEXTUREBUFFER_FORMAT buffer_format;
    };
    ui32 border;
    bool compress;
    bool generate_mipmap;
    bool target_texture;
    Texture3D(ui32 ww, ui32 hh, ui32 dd, nude::PIXEL_FORMAT fmt, bool comp, bool mipmap)
        : width(ww),
          height(hh),
          depth(dd),
          format(fmt),
          compress(comp),
          generate_mipmap(mipmap),
          target_texture(false)
    {
      // None...
    }
    Texture3D(ui32 ww, ui32 hh, ui32 dd, nude::TEXTUREBUFFER_FORMAT fmt, bool mipmap)
        : width(ww),
          height(hh),
          depth(dd),
          buffer_format(fmt),
          generate_mipmap(mipmap),
          target_texture(true)
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
    TARGET_TEXTURE = 1 << 2,
  };

  struct PixelFormat {
    GLenum type;
    size_t element_size;
    ui32 component;
    GLenum format;
    GLenum internal_format;
    GLenum compressed_format;
  };

  struct TargetFormat {
    GLenum type;
    size_t element_size;
    ui32 component;
    GLenum format;
    GLenum internal_format;
  };

  static const PixelFormat mPixelFormatTable[nude::PIXEL_FORMAT_NUM];
  static const TargetFormat mTextureBufferFormat[nude::TEXTUREBUFFER_FORMAT_NUM];

  GLuint mHandle;
  void* mpBuffer;
  size_t mSize;

  TEXTURE_TYPE mTextureType;

  ui32 mWidth;
  ui32 mHeight;
  ui32 mDepth;

  union {
    nude::PIXEL_FORMAT mFormat;
    nude::TEXTUREBUFFER_FORMAT mBufferFormat;
  };

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

  void setTargetTexture(bool target_texture) {
    ui32 ext = getExtension();
    if(target_texture)
      ext |= TARGET_TEXTURE;
    else
      ext &= ~TARGET_TEXTURE;
    setExtension(ext);
  }

  void createBuffer(size_t size, bool init_zero = false) {
    if(isTargetTexture())
      return;

    NU_ASSERT_C(mpBuffer == nullptr);
    mpBuffer = nude::Alloc(size);
    NU_ASSERT(mpBuffer, "Insufficient memory to initialize texture.\n");

    if(init_zero)
      memset(mpBuffer, 0x00, mSize);
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

  ui32 getWidth(void) const {
    return mWidth;
  }
  ui32 getHeight(void) const {
    return mHeight;
  }
  ui32 getDepth(void) const {
    return mDepth;
  }
  nude::PIXEL_FORMAT getFromat(void) const {
    return mFormat;
  }

  nude::WRAP_TEXTURE getWrapS(void) const {
    return mWrapS;
  }
  nude::WRAP_TEXTURE getWrapT(void) const {
    return mWrapT;
  }
  nude::WRAP_TEXTURE getWrapR(void) const {
    return mWrapR;
  }
  nude::FILTER_TEXTURE getMinFilter(void) const {
    return mMinFilter;
  }
  nude::FILTER_TEXTURE getMagFilter(void) const {
    return mMagFilter;
  }

  bool isTargetTexture(void) const {
    return (getExtension() & TARGET_TEXTURE) ? true : false;
  }

};

namespace nude {
  typedef Handle< nuTexture > Texture;
}

#endif
