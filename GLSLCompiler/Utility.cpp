/*!
 * \file Utility.h
 * \brief Utility functions.
 * \author Nus
 * \date 2011/12/29 23:37
 */

#include "Utility.h"
#include "StringTable.h"
#include "ShaderList.h"

extern "C" int yyget_lineno(void);

void yyerror(const char* err_str)
{
  fprintf(stderr, "Error at line-%d: %s\n", yyget_lineno(), err_str);
#if defined(DEBUG)
  assert(false);
#endif
}

int yyaddstr(const char* string)
{
  return StringTable::addString(yyget_lineno(), string);
}

void DumpList(void)
{
  // Dump global processor.
  PreProcessorList* p_list = ShaderList::instance()->getGlobalPreproc();
  if(p_list) {
    fprintf(stdout, "Global preprocessor:\n");
    PreProcessorList::const_iterator it;
    for(it = p_list->begin(); it < p_list->end(); it++) {
      const PreProcessor* p_preproc = *it;
      if(p_preproc->value())
        fprintf(stdout, "  %s = %s (line-%d)\n",
                p_preproc->definition(),
                p_preproc->value(),
                p_preproc->lineNo());
      else
        fprintf(stdout, "  %s (line-%d)\n", p_preproc->definition(), p_preproc->lineNo());
    }

    // Dump program.
    if(ShaderList::instance()->getProgramSize() > 0) {
      for(unsigned int ui = 0; ui < ShaderList::instance()->getProgramSize(); ui++) {
        Program* p_prog = ShaderList::instance()->getProgram(ui);
        fprintf(stdout, "Program: %s (line-%d)\n", p_prog->name(), p_prog->lineNo());

        // Dump local preprocessor.
        if(p_prog->getPreproc()) {
          PreProcessorList::const_iterator it;
          fprintf(stdout, "  Local preprocessor:\n");
          for(it = p_prog->getPreproc()->begin(); it < p_prog->getPreproc()->end(); it++) {
            const PreProcessor* p_preproc = *it;
            if(p_preproc->value())
              fprintf(stdout, "    %s = %s (line-%d)\n",
                      p_preproc->definition(),
                      p_preproc->value(),
                      p_preproc->lineNo());
            else
              fprintf(stdout, "    %s (line-%d)\n", p_preproc->definition(), p_preproc->lineNo());
          }
        }

        // Dump vertex shader.
        if(p_prog->getVertexShader()) {
          Shader* p_shd = p_prog->getVertexShader();
          fprintf(stdout, "  Vertex shader: \"%s\" (line-%d)\n",
                  p_shd->fileName(),
                  p_shd->lineNo());
          if(p_shd->getPreproc()) {
            PreProcessorList::const_iterator it;
            fprintf(stdout, "    VS preprocessor:\n");
            for(it = p_shd->getPreproc()->begin(); it < p_shd->getPreproc()->end(); it++) {
              const PreProcessor* p_preproc = *it;
              if(p_preproc->value())
                fprintf(stdout, "      %s = %s (line-%d)\n",
                        p_preproc->definition(),
                        p_preproc->value(),
                        p_preproc->lineNo());
              else
                fprintf(stdout, "      %s (line-%d)\n", p_preproc->definition(), p_preproc->lineNo());
            }
          }
        } else {
          fprintf(stdout, "  No vertex shader available.\n");
        }

        // Dump fragment shader.
        if(p_prog->getFragmentShader()) {
          Shader* p_shd = p_prog->getFragmentShader();
          fprintf(stdout, "  Fragment shader: \"%s\" (line-%d)\n",
                  p_shd->fileName(),
                  p_shd->lineNo());
          if(p_shd->getPreproc()) {
            PreProcessorList::const_iterator it;
            fprintf(stdout, "    FS preprocessor:\n");
            for(it = p_shd->getPreproc()->begin(); it < p_shd->getPreproc()->end(); it++) {
              const PreProcessor* p_preproc = *it;
              if(p_preproc->value())
                fprintf(stdout, "      %s = %s (line-%d)\n",
                        p_preproc->definition(),
                        p_preproc->value(),
                        p_preproc->lineNo());
              else
                fprintf(stdout, "      %s (line-%d)\n", p_preproc->definition(), p_preproc->lineNo());
            }
          }
        } else {
          fprintf(stdout, "  No fragment shader available.\n");
        }
      }
    } else {
      fprintf(stdout, "No program defined.\n");
    }
  }
}
