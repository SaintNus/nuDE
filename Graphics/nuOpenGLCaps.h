/*!
 * \file nuOpenGLCaps.h
 * \brief OpenGL capabilities.
 * \author Nus
 * \date 2012/05/19 21:22
 */

#ifndef __NUOPENGLCAPS_H__
#define __NUOPENGLCAPS_H__

class nuOpenGLCaps
{
  friend class nuRenderGL;

  GLint mMajorVersion;
  GLint mMinorVersion;

  GLint mMaxCombinedUniformBlocks;
  GLint mMaxVertexUniformBlocks;
  GLint mMaxGeometryUniformBlocks;
  GLint mMaxFragmentUniformBlocks;

  GLint mMaxUniformBufferBindings;

  GLint mMaxCombinedVertexUniformComponents;
  GLint mMaxCombinedGeometryUniformComponents;
  GLint mMaxCombinedFragmentUniformComponents;

  nuOpenGLCaps() {}
  ~nuOpenGLCaps() {}

  void initialize(void);

public:
  GLint getMajorVersion(void) const {
    return mMajorVersion;
  }

  GLint getMinorVersion(void) const {
    return mMinorVersion;
  }

  GLint getMaxCombinedUniformBlocks(void) const {
    return mMaxCombinedUniformBlocks;
  }

  GLint getMaxVertexUniformBlocks(void) const {
    return mMaxVertexUniformBlocks;
  }

  GLint getMaxGeometryUniformBlocks(void) const {
    return mMaxGeometryUniformBlocks;
  }

  GLint getMaxFragmentUniformBlocks(void) const {
    return mMaxFragmentUniformBlocks;
  }

  GLint getMaxUniformBufferBindings(void) const {
    return mMaxUniformBufferBindings;
  }

  GLint getMaxCombinedVertexUniformComponents(void) const {
    return mMaxCombinedVertexUniformComponents;
  }

  GLint getMaxCombinedGeometryUniformComponents(void) const {
    return mMaxCombinedGeometryUniformComponents;
  }

  GLint getMaxCombinedFragmentUniformComponents(void) const {
    return mMaxCombinedFragmentUniformComponents;
  }

};

#endif
