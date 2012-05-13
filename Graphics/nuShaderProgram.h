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

  nuShaderProgram();
  nuShaderProgram(nude::ProgramList program);
  ~nuShaderProgram();

  nude::ProgramList mProgram;
  const nude::Program& mGlslProgram;

  struct UniformValue {
    void* value;
  };

  struct UniformBuffer {
    UniformValue* uniform;
    UniformValue* uniform_block;
  };

  UniformBuffer mUniformBuffer[2];
  ui32 mCurrentBuffer;

  void update(void);

public:

};

namespace nude {
  typedef Handle< nuShaderProgram > ShaderProgram;
}

#endif
