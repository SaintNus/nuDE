/*!
 * \file ShaderObject.cpp
 * \brief Shader object.
 * \author Nus
 * \date 2012/01/15 00:24
 */

#include "ShaderObject.h"

#define WRITE_FILE(_err, _file, ...) if(!fprintf(_file, __VA_ARGS__)) return _err

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

int ShaderObject::writeToFile(FILE* output_h, FILE* output_cpp)
{
#if !NDEBUG
  WRITE_FILE(0, output_h, "const int NULL = 0;\n");
  WRITE_FILE(0, output_h, "typedef int GLenum;\n");
  WRITE_FILE(0, output_h, "typedef int GLint;\n\n");
#endif

  // Write header file.
  WRITE_FILE(0, output_h, "namespace glslp\n");
  WRITE_FILE(0, output_h, "{\n\n");

  WRITE_FILE(0, output_h, "  // Program list.\n");
  WRITE_FILE(0, output_h, "  enum ProgramList {\n");
  {
    ProgramListConstIterator it;
    for(it = mProgramList.begin(); it != mProgramList.end(); it++) {
      WRITE_FILE(0, output_h, "    Program_%s,\n", (*it)->program_name);
    }
    WRITE_FILE(0, output_h, "    ProgramList_Num,\n");
  }
  WRITE_FILE(0, output_h, "  };\n\n");

  WRITE_FILE(0, output_h, "  /*\n");
  WRITE_FILE(0, output_h, "   * Attributes.\n");
  WRITE_FILE(0, output_h, "   */\n\n");
  {
    ProgramListConstIterator it;
    for(it = mProgramList.begin(); it != mProgramList.end(); it++) {
      Program* p_program = *it;
      if(p_program->attribute.size() > 0) {
        WRITE_FILE(0, output_h, "  enum %sAttribute {\n", p_program->program_name);
        std::vector< Variable >::const_iterator vit;
        for(vit = p_program->attribute.begin(); vit != p_program->attribute.end(); vit++) {
          WRITE_FILE(0, output_h, "    %s_%s,\n", p_program->program_name, vit->name);
        }
        WRITE_FILE(0, output_h, "    %sAttribute_Num,\n", p_program->program_name);
        WRITE_FILE(0, output_h, "  };\n\n");
      }
    }
  }

  WRITE_FILE(0, output_h, "  /*\n");
  WRITE_FILE(0, output_h, "   * Uniforms.\n");
  WRITE_FILE(0, output_h, "   */\n\n");
  {
    ProgramListConstIterator it;
    for(it = mProgramList.begin(); it != mProgramList.end(); it++) {
      Program* p_program = *it;
      if(p_program->uniform.size() > 0) {
        WRITE_FILE(0, output_h, "  enum %sUniform {\n", p_program->program_name);
        std::vector< Variable >::const_iterator vit;
        for(vit = p_program->uniform.begin(); vit != p_program->uniform.end(); vit++) {
          WRITE_FILE(0, output_h, "    %s_%s,\n", p_program->program_name, vit->name);
        }
        WRITE_FILE(0, output_h, "    %sUniform_Num,\n", p_program->program_name);
        WRITE_FILE(0, output_h, "  };\n\n");
      }
    }
  }

  WRITE_FILE(0, output_h, "  /*\n");
  WRITE_FILE(0, output_h, "   * Uniform blocks.\n");
  WRITE_FILE(0, output_h, "   */\n\n");
  {
    ProgramListConstIterator it;
    for(it = mProgramList.begin(); it != mProgramList.end(); it++) {
      Program* p_program = *it;
      if(p_program->uniform_block.size() > 0) {
        WRITE_FILE(0, output_h, "  enum %sUniformBlock {\n", p_program->program_name);
        std::vector< Variable >::const_iterator vit;
        for(vit = p_program->uniform_block.begin(); vit != p_program->uniform_block.end(); vit++) {
          WRITE_FILE(0, output_h, "    %s_%s,\n", p_program->program_name, vit->name);
        }
        WRITE_FILE(0, output_h, "    %sUniformBlock_Num,\n", p_program->program_name);
        WRITE_FILE(0, output_h, "  };\n\n");
      }
    }
  }

  WRITE_FILE(0, output_h, "  /*\n");
  WRITE_FILE(0, output_h, "   * Structures.\n");
  WRITE_FILE(0, output_h, "   */\n\n");

  WRITE_FILE(0, output_h, "  struct Variable {\n");
  WRITE_FILE(0, output_h, "    const char* name;\n");
  WRITE_FILE(0, output_h, "    GLenum type;\n");
  WRITE_FILE(0, output_h, "    GLint size;\n");
  WRITE_FILE(0, output_h, "  };\n\n");

  WRITE_FILE(0, output_h, "  struct Program {\n");
  WRITE_FILE(0, output_h, "    const char* vsh_file; // Vertex shader file name.\n");
  WRITE_FILE(0, output_h, "    const char* fsh_file; // Fragment shader file name.\n");
  WRITE_FILE(0, output_h, "    const Variable* attributes; // Program attributes.\n");
  WRITE_FILE(0, output_h, "    const Variable* uniforms; // Program uniforms.\n");
  WRITE_FILE(0, output_h, "    const char** uniform_blocks; // Program uniform blocks.\n");
  WRITE_FILE(0, output_h, "  };\n\n");

  WRITE_FILE(0, output_h, "  // Program table.\n");
  WRITE_FILE(0, output_h, "  extern const Program GLSLPrograms[ProgramList_Num];\n");

  WRITE_FILE(0, output_h, "\n}\n");

  // Write source.
  WRITE_FILE(1, output_cpp, "namespace glslp\n{\n\n");
  {
    ProgramListConstIterator it;
    for(it = mProgramList.begin(); it != mProgramList.end(); it++) {
      if((*it)->attribute.size() > 0) {
        WRITE_FILE(1, output_cpp, "  const Variable %sAttributes[%sAttribute_Num] = {\n",
                   (*it)->program_name,
                   (*it)->program_name);
        {
          std::vector< Variable >::const_iterator vit;
          for(vit = (*it)->attribute.begin(); vit != (*it)->attribute.end(); vit++) {
            WRITE_FILE(1, output_cpp, "    {\n");
            WRITE_FILE(1, output_cpp, "      \"%s\",\n", vit->name);
            WRITE_FILE(1, output_cpp, "      static_cast< GLenum >(%d),\n", vit->type);
            WRITE_FILE(1, output_cpp, "      %d\n", vit->size);
            WRITE_FILE(1, output_cpp, "    },\n");
          }
        }
        WRITE_FILE(1, output_cpp, "  };\n\n");
      }

      if((*it)->uniform.size() > 0) {
        WRITE_FILE(1, output_cpp, "  const Variable %sUniforms[%sUniform_Num] = {\n",
                   (*it)->program_name,
                   (*it)->program_name);
        {
          std::vector< Variable >::const_iterator vit;
          for(vit = (*it)->uniform.begin(); vit != (*it)->uniform.end(); vit++) {
            WRITE_FILE(1, output_cpp, "    {\n");
            WRITE_FILE(1, output_cpp, "      \"%s\",\n", vit->name);
            WRITE_FILE(1, output_cpp, "      static_cast< GLenum >(%d),\n", vit->type);
            WRITE_FILE(1, output_cpp, "      %d\n", vit->size);
            WRITE_FILE(1, output_cpp, "    },\n");
          }
        }
        WRITE_FILE(1, output_cpp, "  };\n\n");
      }

      if((*it)->uniform_block.size() > 0) {
        WRITE_FILE(1, output_cpp, "  const char* %sUniformBlocks[%sUniformBlock_Num] = {\n",
                   (*it)->program_name,
                   (*it)->program_name);
        {
          std::vector< Variable >::const_iterator vit;
          for(vit = (*it)->uniform_block.begin(); vit != (*it)->uniform_block.end(); vit++) {
            WRITE_FILE(1, output_cpp, "    \"%s\",\n", vit->name);
          }
        }
        WRITE_FILE(1, output_cpp, "  };\n\n");
      }
    }
  }

  WRITE_FILE(1, output_cpp, "  const Program GLSLPrograms[ProgramList_Num] = {\n");
  {
    ProgramListConstIterator it;
    for(it = mProgramList.begin(); it != mProgramList.end(); it++) {
      WRITE_FILE(1, output_cpp, "    {\n");
      WRITE_FILE(1, output_cpp, "      \"%s\",\n", (*it)->vertex_shd.file_name);
      WRITE_FILE(1, output_cpp, "      \"%s\",\n", (*it)->fragment_shd.file_name);
      if((*it)->attribute.size() > 0) {
        WRITE_FILE(1, output_cpp, "      %sAttributes,\n", (*it)->program_name);
      } else {
        WRITE_FILE(1, output_cpp, "      NULL,\n");
      }
      if((*it)->uniform.size() > 0) {
        WRITE_FILE(1, output_cpp, "      %sUniforms,\n", (*it)->program_name);
      } else {
        WRITE_FILE(1, output_cpp, "      NULL,\n");
      }
      if((*it)->uniform_block.size() > 0) {
        WRITE_FILE(1, output_cpp, "      %sUniformBlocks,\n", (*it)->program_name);
      } else {
        WRITE_FILE(1, output_cpp, "      NULL,\n");
      }
      WRITE_FILE(1, output_cpp, "    },\n");
    }
  }
  WRITE_FILE(1, output_cpp, "  };\n\n");
  WRITE_FILE(1, output_cpp, "}\n");

  return 0;
}
