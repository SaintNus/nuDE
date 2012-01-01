/*!
 * \file ShaderList.cpp
 * \brief Shader list compiler.
 * \author Nus
 * \date 2011/11/30 20:30
 */

#include "ShaderList.h"

using namespace std;

ShaderList* ShaderList::mpInstance = nullptr;
PreProcessorList* ShaderList::mpPreprocScratch = nullptr;
Program* ShaderList::mpProgramScratch = nullptr;

ShaderList::ShaderList()
    : mpGlobalPreproc(nullptr)
{
  // None...
}

ShaderList::~ShaderList()
{
  if(mpGlobalPreproc) {
    PreProcessorList::iterator it;
    for(it = mpGlobalPreproc->begin(); it < mpGlobalPreproc->end(); it++) {
      delete *it;
      *it = nullptr;
    }
    delete mpGlobalPreproc;
  }
  
  if(mProgram.size() > 0) {
    vector< Program* >::iterator it;
    for(it = mProgram.begin(); it < mProgram.end(); it++) {
      delete *it;
      *it = nullptr;
    }
    mProgram.clear();
  }
}

void ShaderList::addGlobalPreproc(PreProcessorList* p_list)
{
  if(mpGlobalPreproc) {
    PreProcessorList::iterator it;
    for(it = p_list->begin(); it < p_list->end(); it++) {
      mpGlobalPreproc->push_back(*it);
      *it = nullptr;
    }
    delete p_list;
  } else {
    mpGlobalPreproc = p_list;
  }
}

void ShaderList::addProgram(Program* p_program)
{
  mProgram.push_back(p_program);
}

Shader::~Shader()
{
  if(mpPreproc) {
    PreProcessorList::iterator it;
    for(it = mpPreproc->begin(); it < mpPreproc->end(); it++) {
      delete *it;
      *it = nullptr;
    }
    delete mpPreproc;
  }
}

void Shader::addPreproc(PreProcessorList* p_list)
{
  if(mpPreproc) {
    PreProcessorList::iterator it;
    for(it = p_list->begin(); it < p_list->end(); it++) {
      mpPreproc->push_back(*it);
      *it = nullptr;
    }
    delete p_list;
  } else {
    mpPreproc = p_list;
  }
}

Program::~Program()
{
  if(mpVertexShader)
    delete mpVertexShader;
  if(mpFragmentShader)
    delete mpFragmentShader;
  if(mpPreproc) {
    PreProcessorList::iterator it;
    for(it = mpPreproc->begin(); it < mpPreproc->end(); it++) {
      delete *it;
      *it = nullptr;
    }
    delete mpPreproc;
  }
}

void Program::addPreproc(PreProcessorList* p_list)
{
  if(mpPreproc) {
    PreProcessorList::iterator it;
    for(it = p_list->begin(); it < p_list->end(); it++) {
      mpPreproc->push_back(*it);
      *it = nullptr;
    }
    delete p_list;
  } else {
    mpPreproc = p_list;
  }
}
