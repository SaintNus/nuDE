/*!
 * \file nuShaderProgram.cpp
 * \brief GLSL program.
 * \author Nus
 * \date 2012/05/05 23:08
 */

#include "GraphicsInclude.h"
#include "nuShaderList.h"
#include "nuGResManager.h"
#include "nuShaderProgram.h"

#define UNIFORM_TYPE_SIZE(_type, _gl_type, _size) if(_type == _gl_type) return _size

IMPLEMENT_TYPE_INFO_INST(nuShaderProgram, nuGResource, nullptr);

nuShaderProgram::nuShaderProgram(nude::ProgramList program)
    : nuGResource(nuGResource::SHADER_PROGRAM, nuGResource::DYNAMIC_RESOURCE),
      mProgram(program),
      mGlslProgram(nude::GLSLPrograms[program]),
      mProgramID(0),
      mpUniform(nullptr),
      mpUniformBlock(nullptr)
{
  if(mGlslProgram.uniform_num > 0) {
    mpUniform = new Uniform[mGlslProgram.uniform_num];
    NU_ASSERT_C(mpUniform != nullptr);
    for(ui32 ui = 0; ui < mGlslProgram.uniform_num; ui++) {
      const nude::Variable& uniform = mGlslProgram.uniforms[ui];
      mpUniform[ui].location = 0;
      mpUniform[ui].size = getSize(uniform.type) * uniform.size;
    }
  }

  if(mGlslProgram.uniform_block_num > 0) {
    mpUniformBlock = new UniformBlock[mGlslProgram.uniform_block_num];
    NU_ASSERT_C(mpUniformBlock != nullptr);
    for(ui32 ui = 0; ui < mGlslProgram.uniform_block_num; ui++) {
      mpUniformBlock[ui].index = 0;
      mpUniformBlock[ui].binding = 0;
    }
  }

  setUpdate(true);
}

nuShaderProgram::~nuShaderProgram()
{
  if(mpUniform) {
    delete[] mpUniform;
    mpUniform = nullptr;
  }

  if(mpUniformBlock) {
    delete[] mpUniformBlock;
    mpUniformBlock = 0;
  }

  if(mProgramID) {
    CHECK_GL_ERROR(glDeleteProgram(mProgramID));
    mProgramID = 0;
  }
}

void nuShaderProgram::update(void)
{
  if(!isInitialized()) {
    const nuShaderList::Data& data = getGResManager().getShaderList()->getData(mProgram);
    CHECK_GL_ERROR(mProgramID = glCreateProgram());

    CHECK_GL_ERROR(glAttachShader(mProgramID, data.vsh_id));
    CHECK_GL_ERROR(glAttachShader(mProgramID, data.fsh_id));

    if(mGlslProgram.attributes) {
      for(GLuint ui = 0; ui < mGlslProgram.attribute_num; ui++)
        CHECK_GL_ERROR(glBindAttribLocation(mProgramID, ui, mGlslProgram.attributes[ui].name));
    }

    CHECK_GL_ERROR(glLinkProgram(mProgramID));

    GLint status;
    CHECK_GL_ERROR(glGetProgramiv(mProgramID, GL_LINK_STATUS, &status));
    if(status == 0) {
      GLint log_len;
      CHECK_GL_ERROR(glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &log_len));
      if(log_len > 0) {
        nude::TempBuffer<> temp(log_len);
        GLchar* p_log = static_cast< GLchar* >(temp.getBuffer());
        GLsizei len;
        CHECK_GL_ERROR(glGetProgramInfoLog(mProgramID, log_len, &len, p_log));
        NU_TRACE("%s\n", p_log);
      }
      NU_TRACE("Error linking program.\n");
    }

    CHECK_GL_ERROR(glValidateProgram(mProgramID));
    CHECK_GL_ERROR(glGetProgramiv(mProgramID, GL_VALIDATE_STATUS, &status));
    if(status == 0) {
      GLint log_len;
      CHECK_GL_ERROR(glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &log_len));
      if(log_len > 0) {
        nude::TempBuffer<> temp(log_len);
        GLchar* p_log = static_cast< GLchar* >(temp.getBuffer());
        GLsizei len;
        CHECK_GL_ERROR(glGetProgramInfoLog(mProgramID, log_len, &len, p_log));
        NU_TRACE("%s\n", p_log);
      }
      NU_TRACE("Error validating program.\n");
    }

    if(mGlslProgram.uniform_num > 0) {
      for(GLuint ui = 0; ui < mGlslProgram.uniform_num; ui++) {
        CHECK_GL_ERROR(mpUniform[ui].location = glGetUniformLocation(mProgramID,
                                                                     mGlslProgram.uniforms[ui].name));
      }
    }

    if(mGlslProgram.uniform_block_num > 0) {
      for(GLuint ui = 0; ui < mGlslProgram.uniform_block_num; ui++) {
        CHECK_GL_ERROR(mpUniformBlock[ui].index = glGetUniformBlockIndex(mProgramID,
                                                                         mGlslProgram.uniform_blocks[ui].name));
      }
    }

    setInitialized(true);
  }
}

size_t nuShaderProgram::getSize(GLenum type)
{
  UNIFORM_TYPE_SIZE(type, GL_FLOAT, sizeof(GLfloat));
  UNIFORM_TYPE_SIZE(type, GL_FLOAT_VEC2, sizeof(GLfloat) * 2);
  UNIFORM_TYPE_SIZE(type, GL_FLOAT_VEC3, sizeof(GLfloat) * 2);
  UNIFORM_TYPE_SIZE(type, GL_FLOAT_VEC4, sizeof(GLfloat) * 4);
  UNIFORM_TYPE_SIZE(type, GL_INT, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_INT_VEC2, sizeof(GLint) * 2);
  UNIFORM_TYPE_SIZE(type, GL_INT_VEC3, sizeof(GLint) * 3);
  UNIFORM_TYPE_SIZE(type, GL_INT_VEC4, sizeof(GLint) * 4);
  UNIFORM_TYPE_SIZE(type, GL_UNSIGNED_INT, sizeof(GLuint));
  UNIFORM_TYPE_SIZE(type, GL_UNSIGNED_INT_VEC2, sizeof(GLuint) * 2);
  UNIFORM_TYPE_SIZE(type, GL_UNSIGNED_INT_VEC3, sizeof(GLuint) * 3);
  UNIFORM_TYPE_SIZE(type, GL_UNSIGNED_INT_VEC4, sizeof(GLuint) * 4);
  UNIFORM_TYPE_SIZE(type, GL_BOOL, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_BOOL_VEC2, sizeof(GLint) * 2);
  UNIFORM_TYPE_SIZE(type, GL_FLOAT_MAT2, sizeof(GLfloat) * 2 * 2);
  UNIFORM_TYPE_SIZE(type, GL_FLOAT_MAT2x3, sizeof(GLfloat) * 2 * 3);
  UNIFORM_TYPE_SIZE(type, GL_FLOAT_MAT2x4, sizeof(GLfloat) * 2 * 4);
  UNIFORM_TYPE_SIZE(type, GL_FLOAT_MAT3, sizeof(GLfloat) * 3 * 3);
  UNIFORM_TYPE_SIZE(type, GL_FLOAT_MAT3x2, sizeof(GLfloat) * 3 * 2);
  UNIFORM_TYPE_SIZE(type, GL_FLOAT_MAT3x4, sizeof(GLfloat) * 3 * 4);
  UNIFORM_TYPE_SIZE(type, GL_FLOAT_MAT4, sizeof(GLfloat) * 4 * 4);
  UNIFORM_TYPE_SIZE(type, GL_FLOAT_MAT4x2, sizeof(GLfloat) * 4 * 2);
  UNIFORM_TYPE_SIZE(type, GL_FLOAT_MAT4x3, sizeof(GLfloat) * 4 * 3);
  UNIFORM_TYPE_SIZE(type, GL_SAMPLER_1D, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_SAMPLER_1D_ARRAY, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_SAMPLER_1D_ARRAY_SHADOW, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_SAMPLER_1D_SHADOW, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_SAMPLER_2D, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_SAMPLER_2D_ARRAY, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_SAMPLER_2D_ARRAY_SHADOW, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_SAMPLER_2D_MULTISAMPLE, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_SAMPLER_2D_MULTISAMPLE_ARRAY, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_SAMPLER_2D_RECT, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_SAMPLER_2D_RECT_SHADOW, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_SAMPLER_2D_SHADOW, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_SAMPLER_3D, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_SAMPLER_BUFFER, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_SAMPLER_CUBE, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_SAMPLER_CUBE_SHADOW, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_INT_SAMPLER_1D, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_INT_SAMPLER_1D_ARRAY, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_INT_SAMPLER_2D, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_INT_SAMPLER_2D_ARRAY, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_INT_SAMPLER_2D_RECT, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_INT_SAMPLER_2D_MULTISAMPLE, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_INT_SAMPLER_3D, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_INT_SAMPLER_BUFFER, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_INT_SAMPLER_CUBE, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_UNSIGNED_INT_SAMPLER_1D, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_UNSIGNED_INT_SAMPLER_1D_ARRAY, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_UNSIGNED_INT_SAMPLER_2D, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_UNSIGNED_INT_SAMPLER_2D_ARRAY, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_UNSIGNED_INT_SAMPLER_2D_RECT, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_UNSIGNED_INT_SAMPLER_3D, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_UNSIGNED_INT_SAMPLER_BUFFER, sizeof(GLint));
  UNIFORM_TYPE_SIZE(type, GL_UNSIGNED_INT_SAMPLER_CUBE, sizeof(GLint));

  NU_ASSERT_C(false);
  return 0;
}
