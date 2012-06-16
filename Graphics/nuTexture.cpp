/*!
 * \file nuTexture.cpp
 * \brief Texture.
 * \author Nus
 * \date 2012/05/26 00:41
 */

#include "GraphicsInclude.h"
#include "nuTexture.h"

IMPLEMENT_TYPE_INFO_INST(nuTexture, nuGResource, nullptr);

const nuTexture::PixelFormat nuTexture::mPixelFormatTable[nude::PIXEL_FORMAT_NUM] = {
  { GL_UNSIGNED_BYTE, sizeof(GLubyte) * 4, 4, GL_RGBA, GL_RGBA, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_BYTE, sizeof(GLubyte) * 4, 4, GL_BGRA, GL_RGBA, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_BYTE, sizeof(GLubyte) * 3, 3, GL_RGB, GL_RGB, GL_COMPRESSED_RGB },
  { GL_BYTE, sizeof(GLbyte) * 4, 4, GL_RGBA, GL_RGBA, GL_COMPRESSED_RGBA },
  { GL_BYTE, sizeof(GLbyte) * 4, 4, GL_BGRA, GL_RGBA, GL_COMPRESSED_RGBA },
  { GL_BYTE, sizeof(GLbyte) * 3, 3, GL_RGB, GL_RGB, GL_COMPRESSED_RGB },
  { GL_UNSIGNED_SHORT, sizeof(GLushort) * 4, 4, GL_RGBA, GL_RGBA, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_SHORT, sizeof(GLushort) * 4, 4, GL_BGRA, GL_RGBA, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_SHORT, sizeof(GLushort) * 3, 3, GL_RGB, GL_RGB, GL_COMPRESSED_RGB },
  { GL_SHORT, sizeof(GLshort) * 4, 4, GL_RGBA, GL_RGBA, GL_COMPRESSED_RGBA },
  { GL_SHORT, sizeof(GLshort) * 4, 4, GL_BGRA, GL_RGBA, GL_COMPRESSED_RGBA },
  { GL_SHORT, sizeof(GLshort) * 3, 3, GL_RGB, GL_RGB, GL_COMPRESSED_RGB },
  { GL_UNSIGNED_INT, sizeof(GLuint) * 4, 4, GL_RGBA, GL_RGBA, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_INT, sizeof(GLuint) * 4, 4, GL_BGRA, GL_RGBA, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_INT, sizeof(GLuint) * 3, 3, GL_RGB, GL_RGB, GL_COMPRESSED_RGB },
  { GL_INT, sizeof(GLint) * 4, 4, GL_RGBA, GL_RGBA, GL_COMPRESSED_RGBA },
  { GL_INT, sizeof(GLint) * 4, 4, GL_BGRA, GL_RGBA, GL_COMPRESSED_RGBA },
  { GL_INT, sizeof(GLint) * 3, 3, GL_RGB, GL_RGB, GL_COMPRESSED_RGB },
  { GL_FLOAT, sizeof(GLfloat) * 4, 4, GL_RGBA, GL_RGBA, GL_COMPRESSED_RGBA },
  { GL_FLOAT, sizeof(GLfloat) * 4, 4, GL_BGRA, GL_RGBA, GL_COMPRESSED_RGBA },
  { GL_FLOAT, sizeof(GLfloat) * 3, 3, GL_RGB, GL_RGB, GL_COMPRESSED_RGB },
  { GL_UNSIGNED_BYTE_3_3_2, sizeof(GLubyte), 3, GL_RGB, GL_R3_G3_B2, GL_COMPRESSED_RGB },
  { GL_UNSIGNED_BYTE_2_3_3_REV, sizeof(GLubyte), 3, GL_RGB, GL_R3_G3_B2, GL_COMPRESSED_RGB },
  { GL_UNSIGNED_SHORT_5_6_5, sizeof(GLushort), 3, GL_RGB, GL_RGB5, GL_COMPRESSED_RGB },
  { GL_UNSIGNED_SHORT_5_6_5_REV, sizeof(GLushort), 3, GL_RGB, GL_RGB5, GL_COMPRESSED_RGB },
  { GL_UNSIGNED_SHORT_4_4_4_4, sizeof(GLushort), 4, GL_RGBA, GL_RGBA4, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_SHORT_4_4_4_4, sizeof(GLushort), 4, GL_BGRA, GL_RGBA4, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_SHORT_4_4_4_4_REV, sizeof(GLushort), 4, GL_RGBA, GL_RGBA4, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_SHORT_4_4_4_4_REV, sizeof(GLushort), 4, GL_BGRA, GL_RGBA4, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_SHORT_5_5_5_1, sizeof(GLushort), 4, GL_RGBA, GL_RGB5_A1, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_SHORT_5_5_5_1, sizeof(GLushort), 4, GL_BGRA, GL_RGB5_A1, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_SHORT_1_5_5_5_REV, sizeof(GLushort), 4, GL_RGBA, GL_RGB5_A1, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_SHORT_1_5_5_5_REV, sizeof(GLushort), 4, GL_BGRA, GL_RGB5_A1, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_INT_8_8_8_8, sizeof(GLuint), 4, GL_RGBA, GL_RGBA8, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_INT_8_8_8_8, sizeof(GLuint), 4, GL_BGRA, GL_RGBA8, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_INT_8_8_8_8_REV, sizeof(GLuint), 4, GL_RGBA, GL_RGBA8, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_INT_8_8_8_8_REV, sizeof(GLuint), 4, GL_BGRA, GL_RGBA8, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_INT_10_10_10_2, sizeof(GLuint), 4, GL_RGBA, GL_RGB10_A2, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_INT_10_10_10_2, sizeof(GLuint), 4, GL_BGRA, GL_RGB10_A2, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_INT_2_10_10_10_REV, sizeof(GLuint), 4, GL_RGBA, GL_RGB10_A2, GL_COMPRESSED_RGBA },
  { GL_UNSIGNED_INT_2_10_10_10_REV, sizeof(GLuint), 4, GL_BGRA, GL_RGB10_A2, GL_COMPRESSED_RGBA },
};


const nuTexture::TargetFormat nuTexture::mTextureBufferFormat[nude::TEXTUREBUFFER_FORMAT_NUM] = {
  { GL_UNSIGNED_BYTE, sizeof(GLubyte), 1, GL_RED, GL_R8 },
  { GL_BYTE, sizeof(GLbyte), 1, GL_RED, GL_R8I },
  { GL_UNSIGNED_BYTE, sizeof(GLubyte), 1, GL_RED, GL_R8UI },
  { GL_BYTE, sizeof(GLbyte), 1, GL_RED, GL_R8_SNORM },

  { GL_UNSIGNED_SHORT, sizeof(GLushort), 1, GL_RED, GL_R16 },
  { GL_SHORT, sizeof(GLshort), 1, GL_RED, GL_R16I },
  { GL_UNSIGNED_SHORT, sizeof(GLushort), 1, GL_RED, GL_R16UI },
  { GL_HALF_FLOAT, sizeof(GLhalf), 1, GL_RED, GL_R16F },
  { GL_SHORT, sizeof(GLshort), 1, GL_RED, GL_R16_SNORM },

  { GL_INT, sizeof(GLint), 1, GL_RED, GL_R32I },
  { GL_UNSIGNED_INT, sizeof(GLuint), 1, GL_RED, GL_R32UI },
  { GL_FLOAT, sizeof(GLfloat), 1, GL_RED, GL_R32F },

  { GL_INT, 4, 1, GL_RED, GL_COMPRESSED_RED_RGTC1 },
  { GL_INT, 4, 1, GL_RED, GL_COMPRESSED_SIGNED_RED_RGTC1 },

  { GL_UNSIGNED_BYTE, sizeof(GLubyte) * 2, 2, GL_RG, GL_RG8 },
  { GL_BYTE, sizeof(GLbyte) * 2, 2, GL_RG, GL_RG8I },
  { GL_UNSIGNED_BYTE, sizeof(GLubyte) * 2, 2, GL_RG, GL_RG8UI },
  { GL_BYTE, sizeof(GLbyte) * 2, 2, GL_RG, GL_RG8_SNORM },

  { GL_UNSIGNED_SHORT, sizeof(GLushort) * 2, 2, GL_RG, GL_RG16 },
  { GL_HALF_FLOAT, sizeof(GLhalf) * 2, 2, GL_RG, GL_RG16F },
  { GL_SHORT, sizeof(GLshort) * 2, 2, GL_RG, GL_RG16I },
  { GL_UNSIGNED_SHORT, sizeof(GLushort) * 2, 2, GL_RG, GL_RG16UI },
  { GL_SHORT, sizeof(GLshort) * 2, 2, GL_RG, GL_RG16_SNORM },

  { GL_FLOAT, sizeof(GLfloat) * 2, 2, GL_RG, GL_RG32F },
  { GL_INT, sizeof(GLint) * 2, 2, GL_RG, GL_RG32I },
  { GL_UNSIGNED_INT, sizeof(GLuint) * 2, 2, GL_RG, GL_RG32UI },

  { GL_INT, 4 * 2, 2, GL_RG, GL_COMPRESSED_RG_RGTC2 },
  { GL_INT, 4 * 2, 2, GL_RG, GL_COMPRESSED_SIGNED_RG_RGTC2 },

  { GL_INT, sizeof(GLint), 3, GL_RGB, GL_RGB9_E5 },

  { GL_UNSIGNED_BYTE, sizeof(GLubyte) * 3, 3, GL_RGB, GL_SRGB8 },
  { GL_UNSIGNED_BYTE, sizeof(GLubyte) * 3, 3, GL_RGB, GL_RGB8 },
  { GL_BYTE, sizeof(GLbyte) * 3, 3, GL_RGB, GL_RGB8I },
  { GL_UNSIGNED_BYTE, sizeof(GLubyte) * 3, 3, GL_RGB, GL_RGB8UI },
  { GL_BYTE, sizeof(GLbyte) * 3, 3, GL_RGB, GL_RGB8_SNORM },

  { GL_UNSIGNED_SHORT, sizeof(GLushort) * 3, 3, GL_RGB, GL_RGB16 },
  { GL_HALF_FLOAT, sizeof(GLhalf) * 3, 3, GL_RGB, GL_RGB16F },
  { GL_SHORT, sizeof(GLshort) * 3, 3, GL_RGB, GL_RGB16I },
  { GL_UNSIGNED_SHORT, sizeof(GLushort) * 3, 3, GL_RGB, GL_RGB16UI },
  { GL_SHORT, sizeof(GLshort) * 3, 3, GL_RGB, GL_RGB16_SNORM },

  { GL_INT, sizeof(GLint), 3, GL_RGB, GL_R11F_G11F_B10F },

  { GL_FLOAT, sizeof(GLfloat) * 3, 3, GL_RGB, GL_RGB32F },
  { GL_INT, sizeof(GLint) * 3, 3, GL_RGB, GL_RGB32I },
  { GL_UNSIGNED_INT, sizeof(GLuint) * 3, 3, GL_RGB, GL_RGB32UI },

  { GL_INT, sizeof(GLint), 4, GL_RGBA, GL_RGB10_A2 },

  { GL_INT, sizeof(GLint), 4, GL_RGBA, GL_SRGB8_ALPHA8 },

  { GL_INT, sizeof(GLint), 4, GL_RGBA, GL_RGBA8_SNORM },

  { GL_UNSIGNED_SHORT, sizeof(GLushort) * 4, 4, GL_RGBA, GL_RGBA16 },
  { GL_HALF_FLOAT, sizeof(GLhalf) * 4, 4, GL_RGBA, GL_RGBA16F },
  { GL_SHORT, sizeof(GLshort) * 4, 4, GL_RGBA, GL_RGBA16I },
  { GL_UNSIGNED_SHORT, sizeof(GLushort) * 4, 4, GL_RGBA, GL_RGBA16UI },
  { GL_SHORT, sizeof(GLshort) * 4, 4, GL_RGBA, GL_RGBA16_SNORM },

  { GL_FLOAT, sizeof(GLfloat) * 4, 4, GL_RGBA, GL_RGBA32F },
  { GL_INT, sizeof(GLint) * 4, 4, GL_RGBA, GL_RGBA32I },
  { GL_UNSIGNED_INT, sizeof(GLuint) * 4, 4, GL_RGBA, GL_RGBA32UI },

  { GL_UNSIGNED_SHORT, 16, 1, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT16 },
  { GL_UNSIGNED_BYTE, 24, 1, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24 },
  { GL_FLOAT, 32, 1, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT32F },

  { GL_UNSIGNED_INT, 32, 1, GL_DEPTH_STENCIL, GL_DEPTH24_STENCIL8 },
  { GL_UNSIGNED_BYTE, 40, 1, GL_DEPTH_STENCIL, GL_DEPTH32F_STENCIL8 },
};


nuTexture::nuTexture(nuGResource::RESOURCE_USAGE usage)
    : nuGResource(nuGResource::TEXTURE, usage),
      mpBuffer(nullptr),
      mSize(0),
      mHandle(0),
      mTextureType(TEXTURE_1D),
      mWidth(0),
      mHeight(0),
      mDepth(0),
      mFormat(nude::PIXEL_UINT_BGRA_8_8_8_8_REV),
      mWrapS(nude::WRAP_CLAMP_TO_EDGE),
      mWrapT(nude::WRAP_CLAMP_TO_EDGE),
      mWrapR(nude::WRAP_CLAMP_TO_EDGE),
      mMinFilter(nude::FILTER_LINEAR),
      mMagFilter(nude::FILTER_LINEAR)
{
  setTargetTexture(false);
}

nuTexture::~nuTexture()
{
  releaseBuffer();
}

void nuTexture::update(void)
{
  if(!isInitialized()) {
    if(mHandle == 0) {
      CHECK_GL_ERROR(glGenTextures(1, &mHandle));
      NU_ASSERT(mHandle != 0, "Cannot generate texture handle.\n");
    }

    CHECK_GL_ERROR(glBindTexture(static_cast< GLenum >(mTextureType), mHandle));

    CHECK_GL_ERROR(glTexParameteri(static_cast< GLenum >(mTextureType),
                                   GL_TEXTURE_WRAP_S,
                                   mWrapS));
    CHECK_GL_ERROR(glTexParameteri(static_cast< GLenum >(mTextureType),
                                   GL_TEXTURE_WRAP_T,
                                   mWrapT));
    CHECK_GL_ERROR(glTexParameteri(static_cast< GLenum >(mTextureType),
                                   GL_TEXTURE_WRAP_R,
                                   mWrapR));

    CHECK_GL_ERROR(glTexParameteri(static_cast< GLenum >(mTextureType),
                                   GL_TEXTURE_MIN_FILTER,
                                   mMinFilter));
    CHECK_GL_ERROR(glTexParameteri(static_cast< GLenum >(mTextureType),
                                   GL_TEXTURE_MAG_FILTER,
                                   mMagFilter));

    GLenum internal_format;
    if(isCompressed())
      internal_format = mPixelFormatTable[mFormat].compressed_format;
    else
      internal_format = mPixelFormatTable[mFormat].internal_format;

    GLint alignment = mPixelFormatTable[mFormat].element_size % 4 == 0 ? 4 : 1;
    CHECK_GL_ERROR(glPixelStorei(GL_UNPACK_ALIGNMENT, alignment));

    switch(mTextureType) {
    case TEXTURE_1D:
      CHECK_GL_ERROR(glTexImage1D(static_cast< GLenum >(mTextureType),
                                  0,
                                  internal_format,
                                  static_cast< GLsizei >(mWidth),
                                  0,
                                  mPixelFormatTable[mFormat].format,
                                  mPixelFormatTable[mFormat].type,
                                  mpBuffer));
      break;
    case TEXTURE_2D:
      CHECK_GL_ERROR(glTexImage2D(static_cast< GLenum >(mTextureType),
                                  0,
                                  internal_format,
                                  static_cast< GLsizei >(mWidth),
                                  static_cast< GLsizei >(mHeight),
                                  0,
                                  mPixelFormatTable[mFormat].format,
                                  mPixelFormatTable[mFormat].type,
                                  mpBuffer));
      break;
    case TEXTURE_3D:
      CHECK_GL_ERROR(glTexImage3D(static_cast< GLenum >(mTextureType),
                                  0,
                                  internal_format,
                                  static_cast< GLsizei >(mWidth),
                                  static_cast< GLsizei >(mHeight),
                                  static_cast< GLsizei >(mDepth),
                                  0,
                                  mPixelFormatTable[mFormat].format,
                                  mPixelFormatTable[mFormat].type,
                                  mpBuffer));
      break;
    default:
      NU_ASSERT_C(false);
    }

    if(isGenerateMipMap()) {
      CHECK_GL_ERROR(glGenerateMipmap(static_cast< GLenum >(mTextureType)));
    }

    if(getUsage() == nuGResource::STATIC_RESOURCE)
      releaseBuffer();

    setInitialized(true);
  } else if(mpBuffer) {
    CHECK_GL_ERROR(glBindTexture(static_cast< GLenum >(mTextureType), mHandle));

    GLint alignment = mPixelFormatTable[mFormat].element_size % 4 == 0 ? 4 : 1;
    CHECK_GL_ERROR(glPixelStorei(GL_UNPACK_ALIGNMENT, alignment));

    switch(mTextureType) {
    case TEXTURE_1D:
      CHECK_GL_ERROR(glTexSubImage1D(static_cast< GLenum >(mTextureType),
                                     0,
                                     0,
                                     static_cast< GLsizei >(mWidth),
                                     mPixelFormatTable[mFormat].format,
                                     mPixelFormatTable[mFormat].type,
                                     mpBuffer));
      break;
    case TEXTURE_2D:
      CHECK_GL_ERROR(glTexSubImage2D(static_cast< GLenum >(mTextureType),
                                     0,
                                     0,
                                     0,
                                     static_cast< GLsizei >(mWidth),
                                     static_cast< GLsizei >(mHeight),
                                     mPixelFormatTable[mFormat].format,
                                     mPixelFormatTable[mFormat].type,
                                     mpBuffer));
      break;
    case TEXTURE_3D:
      CHECK_GL_ERROR(glTexSubImage3D(static_cast< GLenum >(mTextureType),
                                     0,
                                     0,
                                     0,
                                     0,
                                     static_cast< GLsizei >(mWidth),
                                     static_cast< GLsizei >(mHeight),
                                     static_cast< GLsizei >(mDepth),
                                     mPixelFormatTable[mFormat].format,
                                     mPixelFormatTable[mFormat].type,
                                     mpBuffer));
      break;
    default:
      NU_ASSERT_C(false);
    }

    if(isGenerateMipMap()) {
      CHECK_GL_ERROR(glGenerateMipmap(static_cast< GLenum >(mTextureType)));
    }
  }
}

void nuTexture::initialize(const Texture1D& tex_1d, const Parameter& parameter)
{
  releaseBuffer();

  mTextureType = TEXTURE_1D;
  mWidth = tex_1d.width;
  mHeight = 0;
  mDepth = 0;

  setCompressed(tex_1d.compress);
  setGenerateMipMap(tex_1d.generate_mipmap);
  setTargetTexture(tex_1d.target_texture);

  if(isTargetTexture()) {
    mBufferFormat = tex_1d.buffer_format;
    mSize = mTextureBufferFormat[mBufferFormat].element_size * mWidth;
  } else {
    mFormat = tex_1d.format;
    mSize = mPixelFormatTable[mFormat].element_size * mWidth;
    createBuffer(mSize, true);
  }

  mWrapS = parameter.wrap_s;
  mWrapT = parameter.wrap_t;
  mWrapR = parameter.wrap_r;
  mMinFilter = parameter.min_filter;
  mMagFilter = parameter.mag_filter;

  setInitialized(false);
  setUpdate(true);
}

void nuTexture::initialize(const Texture2D& tex_2d, const Parameter& parameter)
{
  releaseBuffer();

  mTextureType = TEXTURE_2D;
  mWidth = tex_2d.width;
  mHeight = tex_2d.height;
  mDepth = 0;
  setCompressed(tex_2d.compress);
  setGenerateMipMap(tex_2d.generate_mipmap);
  setTargetTexture(tex_2d.target_texture);

  if(isTargetTexture()) {
    mBufferFormat = tex_2d.buffer_format;
    mSize = mTextureBufferFormat[mBufferFormat].element_size * mWidth * mHeight;
  } else {
    mFormat =	 tex_2d.format;
    mSize = mPixelFormatTable[mFormat].element_size * mWidth * mHeight;
    createBuffer(mSize, true);
  }

  mWrapS = parameter.wrap_s;
  mWrapT = parameter.wrap_t;
  mWrapR = parameter.wrap_r;
  mMinFilter = parameter.min_filter;
  mMagFilter = parameter.mag_filter;

  setInitialized(false);
  setUpdate(true);
}

void nuTexture::initialize(const Texture3D& tex_3d, const Parameter& parameter)
{
  releaseBuffer();

  mTextureType = TEXTURE_3D;
  mWidth = tex_3d.width;
  mHeight = tex_3d.height;
  mDepth = tex_3d.depth;
  setCompressed(tex_3d.compress);
  setGenerateMipMap(tex_3d.generate_mipmap);
  setTargetTexture(tex_3d.target_texture);

  if(isTargetTexture()) {
    mBufferFormat = tex_3d.buffer_format;
    mSize = mTextureBufferFormat[mBufferFormat].element_size * mWidth * mHeight * mDepth;
  } else {
    mFormat =	 tex_3d.format;
    mSize = mPixelFormatTable[mFormat].element_size * mWidth * mHeight * mDepth;
    createBuffer(mSize, true);
  }

  mWrapS = parameter.wrap_s;
  mWrapT = parameter.wrap_t;
  mWrapR = parameter.wrap_r;
  mMinFilter = parameter.min_filter;
  mMagFilter = parameter.mag_filter;

  setInitialized(false);
  setUpdate(true);
}

void* nuTexture::beginInitialize(const Texture1D& tex_1d, const Parameter& parameter)
{
  releaseBuffer();

  mTextureType = TEXTURE_1D;
  mWidth = tex_1d.width;
  mHeight = 0;
  mDepth = 0;
  setCompressed(tex_1d.compress);
  setGenerateMipMap(tex_1d.generate_mipmap);
  setTargetTexture(tex_1d.target_texture);

  if(isTargetTexture()) {
    mBufferFormat = tex_1d.buffer_format;
    mSize = mTextureBufferFormat[mBufferFormat].element_size * mWidth;  
  } else {
    mFormat = tex_1d.format;
    mSize = mPixelFormatTable[mFormat].element_size * mWidth;
  }

  createBuffer(mSize);

  mWrapS = parameter.wrap_s;
  mWrapT = parameter.wrap_t;
  mWrapR = parameter.wrap_r;
  mMinFilter = parameter.min_filter;
  mMagFilter = parameter.mag_filter;

  return mpBuffer;
}

void* nuTexture::beginInitialize(const Texture2D& tex_2d, const Parameter& parameter)
{
  releaseBuffer();

  mTextureType = TEXTURE_2D;
  mWidth = tex_2d.width;
  mHeight = tex_2d.height;
  mDepth = 0;
  setCompressed(tex_2d.compress);
  setGenerateMipMap(tex_2d.generate_mipmap);
  setTargetTexture(tex_2d.target_texture);

  if(isTargetTexture()) {
    mBufferFormat = tex_2d.buffer_format;
    mSize = mTextureBufferFormat[mBufferFormat].element_size * mWidth * mHeight;
  } else {
    mFormat =	 tex_2d.format;
    mSize = mPixelFormatTable[mFormat].element_size * mWidth * mHeight;
  }

  createBuffer(mSize);

  mWrapS = parameter.wrap_s;
  mWrapT = parameter.wrap_t;
  mWrapR = parameter.wrap_r;
  mMinFilter = parameter.min_filter;
  mMagFilter = parameter.mag_filter;

  return mpBuffer;
}

void* nuTexture::beginInitialize(const Texture3D& tex_3d, const Parameter& parameter)
{
  releaseBuffer();

  mTextureType = TEXTURE_3D;
  mWidth = tex_3d.width;
  mHeight = tex_3d.height;
  mDepth = tex_3d.depth;
  setCompressed(tex_3d.compress);
  setGenerateMipMap(tex_3d.generate_mipmap);
  setTargetTexture(tex_3d.target_texture);

  if(isTargetTexture()) {
    mBufferFormat = tex_3d.buffer_format;
    mSize = mTextureBufferFormat[mBufferFormat].element_size * mWidth * mHeight * mDepth;
  } else {
    mFormat =	 tex_3d.format;
    mSize = mPixelFormatTable[mFormat].element_size * mWidth * mHeight * mDepth;
  }

  createBuffer(mSize);

  mWrapS = parameter.wrap_s;
  mWrapT = parameter.wrap_t;
  mWrapR = parameter.wrap_r;
  mMinFilter = parameter.min_filter;
  mMagFilter = parameter.mag_filter;

  return mpBuffer;
}

void nuTexture::endInitialize(void)
{
  setInitialized(false);
  setUpdate(true);
}

void* nuTexture::beginUpdate(void)
{
  if(isInitialized())
    return mpBuffer;
  return nullptr;
}

void nuTexture::endUpdate(void)
{
  if(isInitialized() && mpBuffer)
    setUpdate(true);
}
