/*!
 * \file ShaderObject.h
 * \brief Shader object.
 * \author Nus
 * \date 2012/01/15 00:24
 */

#ifndef __SHADEROBJECT_H__
#define __SHADEROBJECT_H__

#include <vector>

class ShaderObject
{
public:
  struct Variable
  {
    const char* name;
    GLenum type;
    GLint size;

    Variable() {
      name = nullptr;
      type = static_cast< GLenum >(0);
      size = 0;
    }
  };

  struct Shader
  {
    const char* file_name;
    const char* immed_name;

    Shader() {
      file_name = nullptr;
      immed_name = nullptr;
    }
  };

  struct Program
  {
    const char* program_name;
    Shader vertex_shd;
    Shader fragment_shd;
    std::vector< Variable > attribute;
    std::vector< Variable > uniform;
    std::vector< Variable > uniform_block;

    Program() {
      program_name = nullptr;
    }
  };

  typedef std::vector< Program* > ProgramList;
  typedef ProgramList::iterator ProgramListIterator;
  typedef ProgramList::const_iterator ProgramListConstIterator;

private:
  ProgramList mProgramList;

public:
  ShaderObject();
  ~ShaderObject();

  ProgramList& getProgramList(void) {
    return mProgramList;
  }

  Program* createProgram(void) {
    Program* p_program = new Program;
    mProgramList.push_back(p_program);
    return p_program;
  }

  int writeToFile(FILE* output_h, const char* name_space);

  const char* getVarTypeString(GLenum type);
};

#endif
