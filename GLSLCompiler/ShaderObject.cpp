/*!
 * \file ShaderObject.cpp
 * \brief Shader object.
 * \author Nus
 * \date 2012/01/15 00:24
 */

#include "ShaderObject.h"

#define WRITE_FILE(_file, ...) if(!fprintf(_file, __VA_ARGS__)) return 1
#define CASE_STR(_type) case _type: return #_type

ShaderObject::ShaderObject()
{
  // None...
}

ShaderObject::~ShaderObject()
{
  ProgramListIterator it;
  for(it = mProgramList.begin(); it != mProgramList.end(); it++) {
    Program* p_program = *it;
    delete p_program;
    *it = nullptr;
  }
}

int ShaderObject::writeToFile(FILE* output_h, const char* name_space)
{
  // Write header file.
  WRITE_FILE(output_h, "#ifndef __cplusplus\n");
  WRITE_FILE(output_h, "#define nullptr NULL\n");
  WRITE_FILE(output_h, "#endif\n");
  WRITE_FILE(output_h, "\n");
  WRITE_FILE(output_h, "#ifdef __cplusplus\n");
  WRITE_FILE(output_h, "namespace %s\n", name_space);
  WRITE_FILE(output_h, "{\n");
  WRITE_FILE(output_h, "#endif\n\n");

  WRITE_FILE(output_h, "  // Program list.\n");
  WRITE_FILE(output_h, "  enum ProgramList {\n");
  {
    ProgramListConstIterator it;
    for(it = mProgramList.begin(); it != mProgramList.end(); it++) {
      WRITE_FILE(output_h, "    Program_%s,\n", (*it)->program_name);
    }
    WRITE_FILE(output_h, "    ProgramList_Num,\n");
  }
  WRITE_FILE(output_h, "  };\n\n");

  WRITE_FILE(output_h, "  /*\n");
  WRITE_FILE(output_h, "   * Attributes.\n");
  WRITE_FILE(output_h, "   */\n\n");
  {
    ProgramListConstIterator it;
    for(it = mProgramList.begin(); it != mProgramList.end(); it++) {
      Program* p_program = *it;
      if(p_program->attribute.size() > 0) {
        WRITE_FILE(output_h, "  enum %sAttribute {\n", p_program->program_name);
        std::vector< Variable >::const_iterator vit;
        char buffer[256];
        for(vit = p_program->attribute.begin(); vit != p_program->attribute.end(); vit++) {
          snprintf(buffer, 256, "%s", vit->name);
          buffer[255] = 0x00;
          for(char* pc = buffer; *pc != 0x00; pc++) {
            if(*pc == '.' || *pc == '[' || *pc == ']')
              *pc = '_';
          }
          WRITE_FILE(output_h, "    %s_%s,\n", p_program->program_name, buffer);
        }
        WRITE_FILE(output_h, "    %sAttribute_Num,\n", p_program->program_name);
        WRITE_FILE(output_h, "  };\n\n");
      }
    }
  }

  WRITE_FILE(output_h, "  /*\n");
  WRITE_FILE(output_h, "   * Uniforms.\n");
  WRITE_FILE(output_h, "   */\n\n");
  {
    ProgramListConstIterator it;
    for(it = mProgramList.begin(); it != mProgramList.end(); it++) {
      Program* p_program = *it;
      if(p_program->uniform.size() > 0) {
        WRITE_FILE(output_h, "  enum %sUniform {\n", p_program->program_name);
        std::vector< Variable >::const_iterator vit;
        char buffer[256];
        for(vit = p_program->uniform.begin(); vit != p_program->uniform.end(); vit++) {
          snprintf(buffer, 256, "%s", vit->name);
          buffer[255] = 0x00;
          for(char* pc = buffer; *pc != 0x00; pc++) {
            if(*pc == '.' || *pc == '[' || *pc == ']')
              *pc = '_';
          }
          WRITE_FILE(output_h, "    %s_%s,\n", p_program->program_name, buffer);
        }
        WRITE_FILE(output_h, "    %sUniform_Num,\n", p_program->program_name);
        WRITE_FILE(output_h, "  };\n\n");
      }
    }
  }

  WRITE_FILE(output_h, "  /*\n");
  WRITE_FILE(output_h, "   * Uniform blocks.\n");
  WRITE_FILE(output_h, "   */\n\n");
  {
    ProgramListConstIterator it;
    for(it = mProgramList.begin(); it != mProgramList.end(); it++) {
      Program* p_program = *it;
      if(p_program->uniform_block.size() > 0) {
        WRITE_FILE(output_h, "  enum %sUniformBlock {\n", p_program->program_name);
        std::vector< Variable >::const_iterator vit;
        char buffer[256];
        for(vit = p_program->uniform_block.begin(); vit != p_program->uniform_block.end(); vit++) {
          snprintf(buffer, 256, "%s", vit->name);
          buffer[255] = 0x00;
          for(char* pc = buffer; *pc != 0x00; pc++) {
            if(*pc == '.' || *pc == '[' || *pc == ']')
              *pc = '_';
          }
          WRITE_FILE(output_h, "    %s_%s,\n", p_program->program_name, buffer);
        }
        WRITE_FILE(output_h, "    %sUniformBlock_Num,\n", p_program->program_name);
        WRITE_FILE(output_h, "  };\n\n");
      }
    }
  }

  WRITE_FILE(output_h, "  /*\n");
  WRITE_FILE(output_h, "   * Structures.\n");
  WRITE_FILE(output_h, "   */\n\n");

  WRITE_FILE(output_h, "  struct Variable {\n");
  WRITE_FILE(output_h, "    const GLchar* name;\n");
  WRITE_FILE(output_h, "    GLenum type;\n");
  WRITE_FILE(output_h, "    GLint size;\n");
  WRITE_FILE(output_h, "  };\n\n");

  WRITE_FILE(output_h, "  struct Structure {\n");
  WRITE_FILE(output_h, "    const GLchar* name;\n");
  WRITE_FILE(output_h, "    GLint size;\n");
  WRITE_FILE(output_h, "  };\n\n");

  WRITE_FILE(output_h, "  struct Program {\n");
  WRITE_FILE(output_h, "    const GLchar* vsh_file; // Vertex shader file name.\n");
  WRITE_FILE(output_h, "    const GLchar* fsh_file; // Fragment shader file name.\n");
  WRITE_FILE(output_h, "    const GLuint attribute_num; // Number of program attribute.\n");
  WRITE_FILE(output_h, "    const Variable* attributes; // Program attributes.\n");
  WRITE_FILE(output_h, "    const GLuint uniform_num; // Number of program uniform.\n");
  WRITE_FILE(output_h, "    const Variable* uniforms; // Program uniforms.\n");
  WRITE_FILE(output_h, "    const GLuint uniform_block_num; // Number of uniform block.\n");
  WRITE_FILE(output_h, "    const Structure* uniform_blocks; // Uniform block names.\n");
  WRITE_FILE(output_h, "  };\n\n");

  WRITE_FILE(output_h, "  /*\n");
  WRITE_FILE(output_h, "   * Constants.\n");
  WRITE_FILE(output_h, "   */\n\n");

  {
    ProgramListConstIterator it;
    for(it = mProgramList.begin(); it != mProgramList.end(); it++) {
      if((*it)->attribute.size() > 0) {
        WRITE_FILE(output_h, "  static const Variable %sAttributes[%sAttribute_Num] = {\n",
                   (*it)->program_name,
                   (*it)->program_name);
        {
          std::vector< Variable >::const_iterator vit;
          for(vit = (*it)->attribute.begin(); vit != (*it)->attribute.end(); vit++) {
            WRITE_FILE(output_h,
                       "    { \"%s\", %s, %d },\n",
                       vit->name,
                       getVarTypeString(vit->type),
                       vit->size);
          }
        }
        WRITE_FILE(output_h, "  };\n\n");
      }

      if((*it)->uniform.size() > 0) {
        WRITE_FILE(output_h, "  static const Variable %sUniforms[%sUniform_Num] = {\n",
                   (*it)->program_name,
                   (*it)->program_name);
        {
          std::vector< Variable >::const_iterator vit;
          for(vit = (*it)->uniform.begin(); vit != (*it)->uniform.end(); vit++) {
            WRITE_FILE(output_h,
                       "    { \"%s\", %s, %d },\n",
                       vit->name,
                       getVarTypeString(vit->type),
                       vit->size);
          }
        }
        WRITE_FILE(output_h, "  };\n\n");
      }

      if((*it)->uniform_block.size() > 0) {
        WRITE_FILE(output_h, "  static const Structure %sUniformBlocks[%sUniformBlock_Num] = {\n",
                   (*it)->program_name,
                   (*it)->program_name);
        {
          std::vector< Variable >::const_iterator vit;
          for(vit = (*it)->uniform_block.begin(); vit != (*it)->uniform_block.end(); vit++) {
            WRITE_FILE(output_h, "    { \"%s\", %d },\n", vit->name, vit->size);
          }
        }
        WRITE_FILE(output_h, "  };\n\n");
      }
    }
  }

  if(mProgramList.size() > 0) {
    WRITE_FILE(output_h, "  static const Program GLSLPrograms[ProgramList_Num] = {\n");
    ProgramListConstIterator it;
    for(it = mProgramList.begin(); it != mProgramList.end(); it++) {
      WRITE_FILE(output_h, "    {\n");
      WRITE_FILE(output_h, "      \"%s\",\n", (*it)->vertex_shd.file_name);
      WRITE_FILE(output_h, "      \"%s\",\n", (*it)->fragment_shd.file_name);
      if((*it)->attribute.size() > 0) {
        WRITE_FILE(output_h, "      %sAttribute_Num,\n", (*it)->program_name);
        WRITE_FILE(output_h, "      %sAttributes,\n", (*it)->program_name);
      } else {
        WRITE_FILE(output_h, "      0,\n");
        WRITE_FILE(output_h, "      nullptr,\n");
      }
      if((*it)->uniform.size() > 0) {
        WRITE_FILE(output_h, "      %sUniform_Num,\n", (*it)->program_name);
        WRITE_FILE(output_h, "      %sUniforms,\n", (*it)->program_name);
      } else {
        WRITE_FILE(output_h, "      0,\n");
        WRITE_FILE(output_h, "      nullptr,\n");
      }
      if((*it)->uniform_block.size() > 0) {
        WRITE_FILE(output_h, "      %sUniformBlock_Num,\n", (*it)->program_name);
        WRITE_FILE(output_h, "      %sUniformBlocks,\n", (*it)->program_name);
      } else {
        WRITE_FILE(output_h, "      0,\n");
        WRITE_FILE(output_h, "      nullptr,\n");
      }
      WRITE_FILE(output_h, "    },\n");
    }
  }
  WRITE_FILE(output_h, "  };\n\n");

  WRITE_FILE(output_h, "\n#ifdef __cplusplus\n");
  WRITE_FILE(output_h, "}\n");
  WRITE_FILE(output_h, "#endif\n");

  return 0;
}

const char* ShaderObject::getVarTypeString(GLenum type)
{
  switch(type) {
  CASE_STR(GL_FLOAT);
  CASE_STR(GL_FLOAT_VEC2);
  CASE_STR(GL_FLOAT_VEC3);
  CASE_STR(GL_FLOAT_VEC4);
  CASE_STR(GL_INT);
  CASE_STR(GL_INT_VEC2);
  CASE_STR(GL_INT_VEC3);
  CASE_STR(GL_INT_VEC4);
  CASE_STR(GL_UNSIGNED_INT);
  CASE_STR(GL_UNSIGNED_INT_VEC2);
  CASE_STR(GL_UNSIGNED_INT_VEC3);
  CASE_STR(GL_UNSIGNED_INT_VEC4);
  CASE_STR(GL_BOOL);
  CASE_STR(GL_BOOL_VEC2);
  CASE_STR(GL_FLOAT_MAT2);
  CASE_STR(GL_FLOAT_MAT2x3);
  CASE_STR(GL_FLOAT_MAT2x4);
  CASE_STR(GL_FLOAT_MAT3);
  CASE_STR(GL_FLOAT_MAT3x2);
  CASE_STR(GL_FLOAT_MAT3x4);
  CASE_STR(GL_FLOAT_MAT4);
  CASE_STR(GL_FLOAT_MAT4x2);
  CASE_STR(GL_FLOAT_MAT4x3);
  CASE_STR(GL_SAMPLER_1D);
  CASE_STR(GL_SAMPLER_1D_ARRAY);
  CASE_STR(GL_SAMPLER_1D_ARRAY_SHADOW);
  CASE_STR(GL_SAMPLER_1D_SHADOW);
  CASE_STR(GL_SAMPLER_2D);
  CASE_STR(GL_SAMPLER_2D_ARRAY);
  CASE_STR(GL_SAMPLER_2D_ARRAY_SHADOW);
  CASE_STR(GL_SAMPLER_2D_MULTISAMPLE);
  CASE_STR(GL_SAMPLER_2D_MULTISAMPLE_ARRAY);
  CASE_STR(GL_SAMPLER_2D_RECT);
  CASE_STR(GL_SAMPLER_2D_RECT_SHADOW);
  CASE_STR(GL_SAMPLER_2D_SHADOW);
  CASE_STR(GL_SAMPLER_3D);
  CASE_STR(GL_SAMPLER_BUFFER);
  CASE_STR(GL_SAMPLER_CUBE);
  CASE_STR(GL_SAMPLER_CUBE_SHADOW);
  CASE_STR(GL_INT_SAMPLER_1D);
  CASE_STR(GL_INT_SAMPLER_1D_ARRAY);
  CASE_STR(GL_INT_SAMPLER_2D);
  CASE_STR(GL_INT_SAMPLER_2D_ARRAY);
  CASE_STR(GL_INT_SAMPLER_2D_RECT);
  CASE_STR(GL_INT_SAMPLER_2D_MULTISAMPLE);
  CASE_STR(GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY);
  CASE_STR(GL_INT_SAMPLER_3D);
  CASE_STR(GL_INT_SAMPLER_BUFFER);
  CASE_STR(GL_INT_SAMPLER_CUBE);
  CASE_STR(GL_UNSIGNED_INT_SAMPLER_1D);
  CASE_STR(GL_UNSIGNED_INT_SAMPLER_1D_ARRAY);
  CASE_STR(GL_UNSIGNED_INT_SAMPLER_2D);
  CASE_STR(GL_UNSIGNED_INT_SAMPLER_2D_ARRAY);
  CASE_STR(GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE);
  CASE_STR(GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY);
  CASE_STR(GL_UNSIGNED_INT_SAMPLER_2D_RECT);
  CASE_STR(GL_UNSIGNED_INT_SAMPLER_3D);
  CASE_STR(GL_UNSIGNED_INT_SAMPLER_BUFFER);
  CASE_STR(GL_UNSIGNED_INT_SAMPLER_CUBE);
  default:
    return "Unsupported";
  }
}
