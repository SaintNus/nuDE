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
  PreProcessor();

public:
  PreProcessor(const char* definition, const char* value = nullptr)
      : mDefinition(definition),
        mValue(value)
  {
    // None...
  }

  ~PreProcessor() {
    // None...
  }

  const char* defintion(void) const {
    return mDefinition;
  }

  const char* value(void) const {
    return mValue;
  }

};

typedef std::vector< PreProcessor* > PreProcessorList;

class ShaderList
{
private:
  static PreProcessorList* mpPreprocScratch;

public:
  ShaderList();
  ~ShaderList();

  static PreProcessorList* getPreprocScratch(void) {
    return mpPreprocScratch;
  }
  static void setPreProcScratch(PreProcessorList* p_list) {
    mpPreprocScratch = p_list;
  }
  static void addPreProcScratch(PreProcessor* p_preproc) {
    if(!mpPreprocScratch) {
      mpPreprocScratch = new PreProcessorList;
    }
    mpPreprocScratch->push_back(p_preproc);
  }
};

#endif
