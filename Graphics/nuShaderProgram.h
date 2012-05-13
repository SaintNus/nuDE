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
  friend nude::Handle< nuShaderProgram >;

  struct UniformValue {
    GLint location;
    size_t size; 
    void* value;
  };

  struct UniformBlock {
    GLuint index;
    GLuint binding;
  };

  nuShaderProgram();

  nuShaderProgram(nude::ProgramList program);
  ~nuShaderProgram();

  nude::ProgramList mProgram;
  const nude::Program& mGlslProgram;
  GLuint mProgramID;

  UniformValue* mpUniformValue;
  UniformBlock* mpUniformBlock;

  void update(void);

  size_t getSize(GLenum type);

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
    if(mpUniformValue || index >= mGlslProgram.uniform_num)
      return 0;
    return mpUniformValue[index].size;
  }

  void setUniform(ui32 index, void* data) {
    if(mpUniformValue || index >= mGlslProgram.uniform_num)
      return;
    memcpy(mpUniformValue[index].value, data, mpUniformValue[index].size);
  }

};

namespace nude {
  typedef Handle< nuShaderProgram > ShaderProgram;
}

#endif
