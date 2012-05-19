/*!
 * \file nuShaderProgram.h
 * \brief GLSL program.
 * \author Nus
 * \date 2012/05/05 23:08
 */

#ifndef __NUSHADERPROGRAM_H__
#define __NUSHADERPROGRAM_H__

#include "nuShaderInfo.h"
#include "nuGResource.h"

class nuShaderProgram : public nuGResource
{
  DECLARE_TYPE_INFO;
  friend class nuGResManager;
  friend class nuGResManager;
  friend class nuRenderGL;
  friend class nuGContext;
  friend nude::Handle< nuShaderProgram >;

  struct Uniform {
    GLint location;
    size_t size;
  };

  struct UniformBlock {
    GLuint index;
  };

  nuShaderProgram();

  nuShaderProgram(nude::ProgramList program);
  ~nuShaderProgram();

  nude::ProgramList mProgram;
  const nude::Program& mGlslProgram;
  GLuint mProgramID;

  Uniform* mpUniform;
  UniformBlock* mpUniformBlock;

  void* mpCurrentUniformValue;
  void* mpRenderedUniformValue;

  void* mpCurrentUniformBlock;
  void* mpRenderedUniformBlock;

  void update(void);

  size_t getSize(GLenum type);

  void setRenderedUniformValue(void* p_uniform) {
    mpRenderedUniformValue = p_uniform;
    setUpdate(true);
  }

  void setRenderedUniformBlock(void* p_block) {
    mpRenderedUniformBlock = p_block;
    setUpdate(true);
  }

public:
  GLuint getHandle(void) const {
    return mProgramID;
  }

  ui32 getUniformNum(void) const {
    return mGlslProgram.uniform_num;
  }

  ui32 getUniformBlockNum(void) const {
    return mGlslProgram.uniform_block_num;
  }

  size_t getUniformSize(ui32 index) const {
    if(!mpUniform || index >= mGlslProgram.uniform_num)
      return 0;
    return mpUniform[index].size;
  }

  GLuint getUniformLocation(ui32 index) const {
    if(!mpUniform || index >= mGlslProgram.uniform_num)
      return 0;
    return mpUniform[index].location;
  }

  GLuint getUniformBlockIndex(ui32 index) const {
    if(!mpUniformBlock || index >= mGlslProgram.uniform_block_num)
      return 0;
    return mpUniformBlock[index].index;
  }

  const nude::Program& getGlslProgram(void) const {
    return mGlslProgram;
  }

};

namespace nude {
  typedef Handle< nuShaderProgram > ShaderProgram;
}

#endif
