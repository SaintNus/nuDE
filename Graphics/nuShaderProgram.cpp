/*!
 * \file nuShaderProgram.cpp
 * \brief GLSL program.
 * \author Nus
 * \date 2012/05/05 23:08
 */

#include "GraphicsInclude.h"
#include "nuShaderProgram.h"

IMPLEMENT_TYPE_INFO(nuShaderProgram, nuGResource);

nuShaderProgram::nuShaderProgram(nude::ProgramList program)
    : nuGResource(nuGResource::SHADER_PROGRAM, nuGResource::DYNAMIC_RESOURCE),
      mProgram(program),
      mGlslProgram(nude::GLSLPrograms[program]),
      mCurrentBuffer(0)
{
  mUniformBuffer[0].uniform = new UniformValue[mGlslProgram.uniform_num];
  mUniformBuffer[1].uniform = new UniformValue[mGlslProgram.uniform_num];
  mUniformBuffer[0].uniform_block = new UniformValue[mGlslProgram.uniform_block_num];
  mUniformBuffer[1].uniform_block = new UniformValue[mGlslProgram.uniform_block_num];

  
}

nuShaderProgram::~nuShaderProgram()
{
  if(mUniformBuffer[0].uniform) {
    delete[] mUniformBuffer[0].uniform;
    mUniformBuffer[0].uniform = nullptr;
  }
  if(mUniformBuffer[1].uniform) {
    delete[] mUniformBuffer[1].uniform;
    mUniformBuffer[1].uniform = nullptr;
  }
  if(mUniformBuffer[0].uniform_block) {
    delete[] mUniformBuffer[0].uniform_block;
    mUniformBuffer[0].uniform_block = nullptr;
  }
  if(mUniformBuffer[1].uniform_block) {
    delete[] mUniformBuffer[1].uniform_block;
    mUniformBuffer[1].uniform_block = nullptr;
  }
}

void nuShaderProgram::update(void)
{
  mCurrentBuffer ^= 1;
}
