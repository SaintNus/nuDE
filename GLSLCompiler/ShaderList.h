/*!
 * \file ShaderList.h
 * \brief Shader list compiler.
 * \author Nus
 * \date 2011/11/30 20:30
 */

#ifndef __SHADERLIST_H__
#define __SHADERLIST_H__

#include <vector>

class PreProcessor
{
private:
  const char* mDefinition;
  const char* mValue;
  unsigned int mLineNo;
  PreProcessor();

public:
  PreProcessor(unsigned int line_no, const char* definition, const char* value = nullptr)
      : mDefinition(definition),
        mValue(value),
        mLineNo(line_no)
  {
    // None...
  }

  ~PreProcessor() {
    // None...
  }

  const char* definition(void) const {
    return mDefinition;
  }

  const char* value(void) const {
    return mValue;
  }

  unsigned int lineNo(void) const {
    return mLineNo;
  }

};

typedef std::vector< PreProcessor* > PreProcessorList;

class Shader
{
private:
  const char* mFileName;
  PreProcessorList* mpPreproc;
  unsigned int mLineNo;

public:
  Shader()
      : mFileName(nullptr),
        mpPreproc(nullptr),
        mLineNo(0)
  {
    // None...
  }
  ~Shader();

  void setFileName(unsigned int line_no, const char* file_name) {
    mLineNo = line_no;
    mFileName = file_name;
  }
  const char* fileName(void) const {
    return mFileName;
  }
  unsigned int lineNo(void) const {
    return mLineNo;
  }

  void addPreproc(PreProcessorList* p_list);
  PreProcessorList* getPreproc(void) const {
    return mpPreproc;
  }
};

class Program
{
private:
  const char* mName;
  Shader* mpVertexShader;
  Shader* mpFragmentShader;
  PreProcessorList* mpPreproc;
  unsigned int mLineNo;

public:
  Program()
      : mpVertexShader(nullptr),
        mpFragmentShader(nullptr),
        mpPreproc(nullptr),
        mName(nullptr),
        mLineNo(0)
  {
    // None...
  }
  ~Program();
  void setName(unsigned int line_no, const char* name) {
    mLineNo = line_no;
    mName = name;
  }
  unsigned int lineNo(void) const {
    return mLineNo;
  }
  const char* name(void) const {
    return mName;
  }

  Shader* createVertexShader(void) {
    if(!mpVertexShader)
      mpVertexShader = new Shader;
    return mpVertexShader;
  }
  Shader* getVertexShader(void) const {
    return mpVertexShader;
  }

  Shader* createFragmentShader(void) {
    if(!mpFragmentShader)
      mpFragmentShader = new Shader;
    return mpFragmentShader;
  }
  Shader* getFragmentShader(void) const {
    return mpFragmentShader;
  }

  PreProcessorList* getPreproc(void) const {
    return mpPreproc;
  }
  void addPreproc(PreProcessorList* p_list);

};

class ShaderList
{
private:
  static ShaderList* mpInstance;
  static PreProcessorList* mpPreprocScratch;
  static Program* mpProgramScratch;

  PreProcessorList* mpGlobalPreproc;
  std::vector< Program* > mProgram;

  ShaderList();
  ~ShaderList();

public:
  static ShaderList* createInstance(void) {
    if(!mpInstance)
      mpInstance = new ShaderList;
    return mpInstance;
  }
  static void deleteInstance(void) {
    delete mpInstance;
  }
  static ShaderList* instance(void) {
    return mpInstance;
  }

  static PreProcessorList* getPreprocScratch(void) {
    return mpPreprocScratch;
  }
  static void setPreprocScratch(PreProcessorList* p_list) {
    mpPreprocScratch = p_list;
  }
  static void addPreprocScratch(PreProcessor* p_preproc) {
    if(!mpPreprocScratch) {
      mpPreprocScratch = new PreProcessorList;
    }
    mpPreprocScratch->push_back(p_preproc);
  }

  void addGlobalPreproc(PreProcessorList* p_list);

  PreProcessorList* getGlobalPreproc(void) const {
    return mpGlobalPreproc;
  }

  static Program* getProgramScratch(void) {
    if(!mpProgramScratch)
      mpProgramScratch = new Program;
    return mpProgramScratch;
  }

  static void setProgramScratch(Program* p_program) {
    mpProgramScratch = p_program;
  }

  void addProgram(Program* p_program);

  Program* getProgram(unsigned int index) const {
    if(index >= mProgram.size())
      return nullptr;
    return mProgram[index];
  }

  size_t getProgramSize(void) const {
    return mProgram.size();
  }

};

#endif
