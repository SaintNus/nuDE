/*!
 * \file Utility.h
 * \brief Utility functions.
 * \author Nus
 * \date 2011/12/29 23:37
 */

#include "Utility.h"
#include "Settings.h"
#include "StringTable.h"
#include "ShaderList.h"
#include "ShaderObject.h"
#include "GlObject.h"

extern "C" int yyget_lineno(void);

int yy_iserror = 0;
FILE* OutputHeaderFile = nullptr;
FILE* OutputSourceFile = nullptr;
ShaderObject ShaderObj;

extern "C"
{
  int ProcessFile(FILE* inoutfile, const char* file_name, std::vector< int >& incl_list);
  int ParseIncludeFile(unsigned int line_no, const char* line);
  int WritePreprocessor(FILE* inout_file, const PreProcessorList* p_plist);
  int BuildGLSLProgram(ShaderObject::Program* p_program, FILE* vsh_file, FILE* fsh_file);
}

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

int BuildList(const char* intermediate_dir)
{
  int ret = ErrorNone;
  GlObject* p_gl = new GlObject();

  // Build all program.
  for(unsigned int ui = 0; ui < ShaderList::instance()->getProgramSize(); ui++) {
    int err = ErrorNone;
    const Program* p_prog = ShaderList::instance()->getProgram(ui);
    FILE* vsh_file = nullptr;
    FILE* fsh_file = nullptr;
    ShaderObject::Program* p_soprog = new ShaderObject::Program;

    ShaderObj.getProgramList().push_back(p_soprog);
    p_soprog->program_name = p_prog->name();

    if(p_prog->getVertexShader()) {
      FILE* immed_file;
      Shader* p_vsh = p_prog->getVertexShader();
      const char* vsh_fname = p_vsh->fileName();
      char immed_fname[PATH_MAX];

      if(vsh_fname[0] == '/')
        snprintf(immed_fname, PATH_MAX, "%s%s", intermediate_dir, vsh_fname);
      else
        snprintf(immed_fname, PATH_MAX, "%s/%s", intermediate_dir, vsh_fname);

      if(MakePath(immed_fname)) {
        fprintf(stderr, "Cannot create intermediate file \"%s\".\n", immed_fname);
        err = ErrorFileSystem;
      } else {
        immed_file = fopen(immed_fname, "wb");
        if(!immed_file) {
          fprintf(stderr, "Error: Cannot open immediate file \"%s\".\n", immed_fname);
          err = ErrorIO;
        } else {
          std::vector< int > include_list;

          p_soprog->vertex_shd.file_name = vsh_fname;
          {
            int str = StringTable::addString(0, immed_fname);
            p_soprog->vertex_shd.immed_name = StringTable::getString(str)->string();
          }

          const char* ver = "#version 150\n\n";
          fwrite(ver, sizeof(char), strlen(ver), immed_file);

          // Global preprocessor.
          const PreProcessorList* p_glist = ShaderList::instance()->getGlobalPreproc();
          if(WritePreprocessor(immed_file, p_glist)) {
            fprintf(stderr, "Error: Cannot write to immediate file \"%s\".\n", immed_fname);
            err = ErrorIO;
          }

          // Vertex preprocessor.
          p_glist = p_vsh->getPreproc();
          if(WritePreprocessor(immed_file, p_glist)) {
            fprintf(stderr, "Error: Cannot write to immediate file \"%s\".\n", immed_fname);
            err = ErrorIO;
          }

          // Process vertex shader.
          if(ProcessFile(immed_file, vsh_fname, include_list)) {
            fprintf(stderr, "Error: Error writing immediate file \"%s\".\n", immed_fname);
            err = ErrorIO;
          } else {
            fclose(immed_file);
            vsh_file = fopen(immed_fname, "rb");
            if(!vsh_file) {
              fprintf(stderr, "Error: Error reading immediate file \"%s\".\n", immed_fname);
              err = ErrorIO;
            }
          }
        }
      }
    }

    if(p_prog->getFragmentShader()) {
      FILE* immed_file;
      Shader* p_fsh = p_prog->getFragmentShader();
      const char* fsh_fname = p_fsh->fileName();
      char immed_fname[PATH_MAX];

      if(fsh_fname[0] == '/')
        snprintf(immed_fname, PATH_MAX, "%s%s", intermediate_dir, fsh_fname);
      else
        snprintf(immed_fname, PATH_MAX, "%s/%s", intermediate_dir, fsh_fname);

      if(MakePath(immed_fname)) {
        fprintf(stderr, "Cannot create intermediate file \"%s\".\n", immed_fname);
        err = ErrorFileSystem;
      } else {
        immed_file = fopen(immed_fname, "wb");
        if(!immed_file) {
          fprintf(stderr, "Error: Cannot open immediate file \"%s\".\n", immed_fname);
          err = ErrorIO;
        } else {
          std::vector< int > include_list;

          p_soprog->fragment_shd.file_name = fsh_fname;
          {
            int str = StringTable::addString(0, immed_fname);
            p_soprog->fragment_shd.immed_name = StringTable::getString(str)->string();
          }

          const char* ver = "#version 150\n\n";
          fwrite(ver, sizeof(char), strlen(ver), immed_file);

          // Global preprocessor.
          const PreProcessorList* p_glist = ShaderList::instance()->getGlobalPreproc();
          if(WritePreprocessor(immed_file, p_glist)) {
            fprintf(stderr, "Error: Cannot write to immediate file \"%s\".\n", immed_fname);
            err = ErrorIO;
          }

          // Fragment preprocessor.
          p_glist = p_fsh->getPreproc();
          if(WritePreprocessor(immed_file, p_glist)) {
            fprintf(stderr, "Error: Cannot write to immediate file \"%s\".\n", immed_fname);
            err = ErrorIO;
          }

          // Process Fragment shader.
          if(ProcessFile(immed_file, fsh_fname, include_list)) {
            fprintf(stderr, "Error: Error writing immediate file \"%s\".\n", immed_fname);
            err = ErrorIO;
          } else {
            fclose(immed_file);
            fsh_file = fopen(immed_fname, "rb");
            if(!fsh_file) {
              fprintf(stderr, "Error: Error reading immediate file \"%s\".\n", immed_fname);
              err = ErrorIO;
            }
          }
        }
      }
    }

    if(err != ErrorNone)
      continue;

    BuildGLSLProgram(p_soprog, vsh_file, fsh_file);

    fclose(vsh_file);
    fclose(fsh_file);
  }

  ret = ShaderObj.writeToFile(OutputHeaderFile, OutputSourceFile);
  if(ret) {
    const char* err_str[] = {
      "",
      "header",
      "source",
    };
    fprintf(stderr, "Cannot write output %s file.\n", err_str[ret]);
    ret = ErrorIO;
  } else {
    ret = ErrorNone;
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
        return ErrorFileSystem;
    } else if(!S_ISDIR(st.st_mode)) {
      return ErrorIO;
    }

    *p_ch = '/';
    p_ch++;
    p_ch = strchr(p_ch, '/');
  }

  return ErrorNone;
}

int ProcessFile(FILE* inout_file, const char* file_name, std::vector< int >& incl_list)
{
  FILE* in_file;
  int ret = ErrorNone;

  in_file = fopen(file_name, "r");
  if(in_file) {
    const size_t buffer_sz = 2048;
    void* buffer = malloc(buffer_sz);
    size_t line_sz = 0;
    unsigned int line_no = 0;
    char* p_ch = static_cast< char* >(buffer);

    while(fread(p_ch, sizeof(char), 1, in_file) != 0 && line_sz < buffer_sz) {
      bool do_parse = false;
      line_sz++;

      if(*p_ch == 0x0a) {
        *p_ch = 0x00;
        do_parse = true;
      } else if(*p_ch == 0x0d) {
        *p_ch = 0x00;
        p_ch++;
        fread(p_ch, sizeof(char), 1, in_file);
        if(*p_ch != 0x0a)
          fseek(in_file, -1, SEEK_CUR);
        do_parse = true;
      }

      if(do_parse) {
        line_no++;
        int str_num = ParseIncludeFile(line_no, static_cast< const char* >(buffer));
        if(str_num >= 0) {
          bool found = false;
          std::vector< int >::iterator it;
          for(it = incl_list.begin(); it != incl_list.end(); it++) {
            if(strcmp(StringTable::getString(str_num)->string(), StringTable::getString(*it)->string()) == 0) {
              found = true;
              break;
            }
          }
          if(!found) {
            incl_list.push_back(str_num);
            ret = ProcessFile(inout_file, StringTable::getString(str_num)->string(), incl_list);
            if(ret) {
              fprintf(stderr,
                      "Error: Invalid processing include file at \"%s:%d\".\n",
                      StringTable::getString(str_num)->string(),
                      StringTable::getString(str_num)->lineNo());
            }
          }
        } else {
          char* p_temp = static_cast< char* >(buffer);
          p_temp[line_sz - 1] = '\n';
          if(fwrite(p_temp, sizeof(char), line_sz, inout_file) != line_sz) {
            fprintf(stderr, "Error: Cannot write to file.\n");
            ret = -6;
            break;
          }
        }
        line_sz = 0;
        p_ch = static_cast< char* >(buffer);
      } else {
        p_ch++;
      }
    }

    if(line_sz == buffer_sz) {
      fprintf(stderr, "Error: Not enough line buffer.\n");
      ret = ErrorMemory;
    }

    free(buffer);
  } else {
    fprintf(stderr, "Error: Cannot open \"%s\".\n", file_name);
    ret = ErrorIO;
  }

  return ret;
}

int ParseIncludeFile(unsigned int line_no, const char* line)
{
  char* p_ch = strstr(line, "#include");
  if(p_ch) {
    char include_path[PATH_MAX];
    p_ch += strlen("#include");
    while(*p_ch != '"' && *p_ch != '<') {
      if(*p_ch == 0x00) {
        return ErrorException;
        break;
      }
      p_ch++;
    }
    p_ch++;
    while(*p_ch == ' ') {
      if(*p_ch == 0x00)
        return ErrorException;
      p_ch++;
    }
    for(unsigned int ui = 0; ui < PATH_MAX; ui++) {
      include_path[ui] = p_ch[ui];
      if(include_path[ui] == '"' || include_path[ui] == '>') {
        if(ui == 0)
          return ErrorException;
        include_path[ui] = 0x00;
        p_ch = &include_path[ui - 1];
        break;
      } else if(*p_ch == 0x00) {
        return ErrorException;
      }
    }
    while(*p_ch == ' ') {
      p_ch--;
      if(*p_ch != ' ') {
        p_ch[1] = 0x00;
        break;
      }
    }
    return StringTable::addString(line_no, include_path);
  }
  return -1;
}

int WritePreprocessor(FILE* inout_file, const PreProcessorList* p_plist)
{
  PreProcessorList::const_iterator it;
  for(it = p_plist->begin(); it < p_plist->end(); it++) {
    const PreProcessor* p_preproc = *it;
    if(p_preproc->value()) {
      char buffer[512];
      snprintf(buffer, 512, "#define %s %s\n",
               p_preproc->definition(),
               p_preproc->value());
      if(fwrite(buffer, sizeof(char), strlen(buffer), inout_file) == 0)
        return 1;
    } else {
      char buffer[512];
      snprintf(buffer, 512, "#define %s\n", p_preproc->definition());
      if(fwrite(buffer, sizeof(char), strlen(buffer), inout_file) == 0)
        return ErrorIO;
    }
  }
  return ErrorNone;
}

int BuildGLSLProgram(ShaderObject::Program* p_program, FILE* vsh_file, FILE* fsh_file)
{
  if(vsh_file == nullptr || fsh_file == nullptr)
    return ErrorIO;

  GLuint vsh_id = 0;
  GLuint fsh_id = 0;

  // Vertex shader.
  {
    fseek(vsh_file, 0, SEEK_END);
    size_t vsh_sz = ftell(vsh_file);
    void* p_buffer = malloc(vsh_sz);
    rewind(vsh_file);
    vsh_sz = fread(p_buffer, sizeof(char), vsh_sz, vsh_file);
    if(vsh_sz)
      vsh_id = GlObject::compileShader(GL_VERTEX_SHADER, p_buffer, vsh_sz);
    free(p_buffer);
  }

  // Fragment shader.
  {
    fseek(fsh_file, 0, SEEK_END);
    size_t fsh_sz = ftell(fsh_file);
    void* p_buffer = malloc(fsh_sz);
    rewind(fsh_file);
    fsh_sz = fread(p_buffer, sizeof(char), fsh_sz, fsh_file);
    if(fsh_sz)
      fsh_id = GlObject::compileShader(GL_FRAGMENT_SHADER, p_buffer, fsh_sz);
    free(p_buffer);
  }

  if(vsh_id == 0 || fsh_id == 0) {
    GlObject::deleteShader(vsh_id);
    GlObject::deleteShader(fsh_id);
    return ErrorShaderCompile;
  }

  // Link program.
  GLuint prog_id = GlObject::linkProgram(vsh_id, fsh_id);
  if(!prog_id) {
    GlObject::deleteShader(vsh_id);
    GlObject::deleteShader(fsh_id);
    return ErrorProgramLink;
  }

  // Get active attributes.
  {
    GLint attrib, attrib_len;

    glGetProgramiv(prog_id, GL_ACTIVE_ATTRIBUTES, &attrib);
    glGetProgramiv(prog_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attrib_len);

    GLchar* p_attr = static_cast< GLchar* >(malloc(attrib_len));

    for(GLuint ui = 0; ui < static_cast< GLuint >(attrib); ui++) {
      GLsizei len;
      GLint size;
      GLenum type;
      glGetActiveAttrib(prog_id, ui, attrib_len, &len, &size, &type, p_attr);
      int str_idx = StringTable::addString(0, p_attr);
      if(str_idx < 0) {
        fprintf(stderr, "Not enough memory for attribute \"%s\".\n", p_attr);
        continue;
      }

      {
        ShaderObject::Variable var;
        var.name = StringTable::getString(str_idx)->string();
        var.size = size;
        var.type = type;
        p_program->attribute.push_back(var);
      }
    }

    free(p_attr);
  }

  // Get active uniforms.
  {
    GLint uniform, uniform_len;

    glGetProgramiv(prog_id, GL_ACTIVE_UNIFORMS, &uniform);
    glGetProgramiv(prog_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniform_len);

    GLchar* p_uniform = static_cast< GLchar* >(malloc(uniform_len));

    for(GLuint ui = 0; ui < static_cast< GLuint >(uniform); ui++) {
      GLsizei len;
      GLint size;
      GLenum type;
      glGetActiveUniform(prog_id, ui, uniform_len, &len, &size, &type, p_uniform);
      for(GLchar* pc = p_uniform; *pc != 0x00; pc++) {
        if(*pc == '.')
          *pc = '_';
      }
      int str_idx = StringTable::addString(0, p_uniform);
      if(str_idx < 0) {
        fprintf(stderr, "Not enough memory for uniform \"%s\".\n", p_uniform);
        continue;
      }

      {
        ShaderObject::Variable var;
        var.name = StringTable::getString(str_idx)->string();
        var.size = size;
        var.type = type;
        p_program->uniform.push_back(var);
      }
    }

    free(p_uniform);
  }

  // Get active uniform blocks.
  {
    GLint uniform, uniform_len;

    glGetProgramiv(prog_id, GL_ACTIVE_UNIFORM_BLOCKS, &uniform);
    glGetProgramiv(prog_id, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &uniform_len);

    GLchar* p_uniform = static_cast< GLchar* >(malloc(uniform_len));

    for(GLuint ui = 0; ui < static_cast< GLuint >(uniform); ui++) {
      GLsizei len;
      glGetActiveUniformBlockName(prog_id, ui, uniform_len, &len, p_uniform);
      for(GLchar* pc = p_uniform; *pc != 0x00; pc++) {
        if(*pc == '.')
          *pc = '_';
      }
      int str_idx = StringTable::addString(0, p_uniform);
      if(str_idx < 0) {
        fprintf(stderr, "Not enough memory for uniform block \"%s\".\n", p_uniform);
        continue;
      }

      {
        ShaderObject::Variable var;
        var.name = StringTable::getString(str_idx)->string();
        var.size = 0;
        var.type = 0;
        p_program->uniform_block.push_back(var);
      }
    }

    free(p_uniform);
  }

  GlObject::deleteProgram(prog_id);
  GlObject::deleteShader(vsh_id);
  GlObject::deleteShader(fsh_id);

  return ErrorNone;
}
