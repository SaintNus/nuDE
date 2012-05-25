/*!
 * \file nuOpenGLCaps.cpp
 * \brief OpenGL capabilities.
 * \author Nus
 * \date 2012/05/19 21:22
 */

#include "GraphicsInclude.h"
#include "nuOpenGLCaps.h"

void nuOpenGLCaps::initialize(void)
{
  CHECK_GL_ERROR(glGetIntegerv(GL_MAJOR_VERSION, &mMajorVersion));
  CHECK_GL_ERROR(glGetIntegerv(GL_MINOR_VERSION, &mMinorVersion));

  CHECK_GL_ERROR(glGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, &mMaxCombinedUniformBlocks));
  CHECK_GL_ERROR(glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &mMaxVertexUniformBlocks));
  CHECK_GL_ERROR(glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &mMaxGeometryUniformBlocks));
  CHECK_GL_ERROR(glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &mMaxFragmentUniformBlocks));

  CHECK_GL_ERROR(glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &mMaxUniformBufferBindings));

  CHECK_GL_ERROR(glGetIntegerv(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS,
                               &mMaxCombinedVertexUniformComponents));
  CHECK_GL_ERROR(glGetIntegerv(GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS,
                               &mMaxCombinedGeometryUniformComponents));
  CHECK_GL_ERROR(glGetIntegerv(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS,
                               &mMaxCombinedFragmentUniformComponents));

  CHECK_GL_ERROR(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &mMaxAttributes));
  CHECK_GL_ERROR(glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, &mAliasedLineWidthRange));
  CHECK_GL_ERROR(glGetFloatv(GL_SMOOTH_LINE_WIDTH_RANGE, &mSmoothLineWidthRange));
  CHECK_GL_ERROR(glGetFloatv(GL_SMOOTH_LINE_WIDTH_GRANULARITY, &mSmoothLineWidthGranularity));
}
