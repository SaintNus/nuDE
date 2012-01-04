/*!
 * \file Utility.h
 * \brief Utility functions.
 * \author Nus
 * \date 2011/12/29 23:37
 */

#include "Utility.h"
#include "StringTable.h"
#include "ShaderList.h"
#include "GlObject.h"

extern "C" int yyget_lineno(void);

int yy_iserror = 0;
FILE* OutputHeaderFile = nullptr;
FILE* OutputSourceFile = nullptr;

static int ProcessFile(const char* file_name);

void yyerror(const char* err_str)
{
  fprintf(stderr, "Error at line-%d: %s\n", yyget_lineno(), err_str);
  yy_iserror = 1;
#if defined(DEBUG)
  assert(yy_iserror == 0);
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

int yyget_iserror(void)
{
  return yy_iserror;
}

int BuildList(void)
{
  int ret = 0;
  GlObject* p_gl = new GlObject();

  // Build all program.
  {
    for(unsigned int ui = 0; ui < ShaderList::instance()->getProgramSize(); ui++) {
      int err = 0;
      const Program* p_prog = ShaderList::instance()->getProgram(ui);
      if(p_prog->getVertexShader()) {
        Shader* p_vsh = p_prog->getVertexShader();
        void* file_buffer = nullptr;
        size_t file_sz = 0;
        FILE* vs_file = fopen(p_vsh->fileName(), "rb");
        if(vs_file) {
          fseek(vs_file, 0, SEEK_END);
          file_sz = ftell(vs_file);
          rewind(vs_file);
          file_buffer = malloc(file_sz);
          fread(file_buffer, 1, file_sz, vs_file);
          free(file_buffer);
          fclose(vs_file);
        } else {
          fprintf(stderr, "Error: Cannot open vertex shader \"%s\".\n", p_vsh->fileName());
          err = -5;
        }
      }
      if(p_prog->getFragmentShader()) {
        Shader* p_fsh = p_prog->getFragmentShader();
        void* file_buffer = nullptr;
        size_t file_sz = 0;
        FILE* fs_file = fopen(p_fsh->fileName(), "rb");
        if(fs_file) {
          fseek(fs_file, 0, SEEK_END);
          file_sz = ftell(fs_file);
          rewind(fs_file);
          file_buffer = malloc(file_sz);
          fread(file_buffer, 1, file_sz, fs_file);
          free(file_buffer);
          fclose(fs_file);
        } else {
          fprintf(stderr, "Error: Cannot open fragment shader \"%s\".\n", p_fsh->fileName());
          err = -5;
        }
      }

      if(err != 0)
        continue;

      // Build shader here...
    }
  }

  delete p_gl;
  return ret;
}

FILE* OutputHeader(void)
{
  return OutputHeaderFile;
}

FILE* OutputSource(void)
{
  return OutputSourceFile;
}

void SetOutputHeader(FILE* p_file)
{
  OutputHeaderFile = p_file;
}

void SetOutputSource(FILE* p_file)
{
  OutputSourceFile = p_file;
}

int MakePath(const char* path)
{
  char buffer[PATH_MAX];
  char* p_ch;

  strncpy(buffer, path, PATH_MAX);

  p_ch = strrchr(buffer, '/');
  if(p_ch)
    p_ch[1] = '\0';

  if(buffer[0] == '/')
    p_ch = strchr(&buffer[1], '/');
  else
    p_ch = strchr(buffer, '/');

  while(p_ch) {
    struct stat st;
    *p_ch = '\0';

    if(stat(buffer, &st) != 0) {
      if(mkdir(buffer, 0755) != 0) 
        return -1;
    } else if(!S_ISDIR(st.st_mode)) {
      return -2;
    }

    *p_ch = '/';
    p_ch++;
    p_ch = strchr(p_ch, '/');
  }

  return 0;
}

int ProcessFile(const char* file_name)
{
  
  return 0;
}
