/*!
 * \file nuShaderList.cpp
 * \brief Shader list resource.
 * \author Nus
 * \date 2012/05/05 23:12
 */

#include "GraphicsInclude.h"
#include "nuShaderList.h"

IMPLEMENT_TYPE_INFO(nuShaderList, nuResource);

nuShaderList::nuShaderList()
{
  for(ui32 ui = 0; ui < nude::ProgramList_Num; ui++) {
    mData[ui].vsh_data = nullptr;
    mData[ui].vsh_size = 0;
    mData[ui].vsh_id = 0;
    mData[ui].fsh_data = nullptr;
    mData[ui].fsh_size = 0;
    mData[ui].fsh_id = 0;
  }
}

nuShaderList::~nuShaderList()
{
  for(ui32 ui = 0; ui < nude::ProgramList_Num; ui++) {
    if(mData[ui].vsh_data)
      nude::Dealloc(mData[ui].vsh_data);
    if(mData[ui].vsh_id != 0)
      CHECK_GL_ERROR(glDeleteShader(mData[ui].vsh_id));
    if(mData[ui].fsh_data)
      nude::Dealloc(mData[ui].fsh_data);
    if(mData[ui].fsh_id != 0)
      CHECK_GL_ERROR(glDeleteShader(mData[ui].fsh_id));
  }
}

nuResource::ERROR_CODE nuShaderList::setup(const nuStream &stream)
{
  if(stream.getSize() == 0)
    return ERROR_INVALID;

  nude::TempBuffer<> temp(stream.getSize());
  if(!temp.getBuffer())
    return ERROR_INSUFFICIENT_MEMORY;

  if(stream.read(temp.getBuffer(), temp.getSize()) != temp.getSize())
    return ERROR_READ_FAIL;

  c8 shader_path[PATH_MAX];
  c8* file_name = shader_path;
  const c8* p_curr = static_cast< c8* >(temp.getBuffer());
  size_t remain = temp.getSize();
  size_t path_cmd_len = strlen("SHADER_PATH");
  bool end = false;
  for(ui32 ui = 0; ui < static_cast< ui32 >(temp.getSize()) && !end; ui++) {
    switch(p_curr[ui]) {
    case 0x0a:
    case 0x0d:
    case ' ':
    case '\t':
      break;
    case '#':
      do {
        ui++;
        remain--;
      } while(p_curr[ui] != 0x0a && p_curr[ui] != 0x0d);
      break;
    default:
      if(remain > path_cmd_len && strncmp(&p_curr[ui], "SHADER_PATH", path_cmd_len) == 0) {
        ui += path_cmd_len;
        remain -= path_cmd_len;
        c8* pc = shader_path;
        for(ui32 uj = ui; uj < static_cast< ui32 >(temp.getSize()) && !end; uj++) {
          switch(p_curr[uj]) {
          case ' ':
          case '\t':
          case '=':
            break;
          case '"':
            uj++;
            remain--;
            while(p_curr[uj] != '"' && remain > 0) {
              *pc = p_curr[uj];
              pc++;
              uj++;
              remain--;
            }
            if(pc != shader_path && pc[-1] != '/') {
              *pc = '/';
              pc++;
            }
            *pc = 0x00;
            NU_TRACE("Shader path: %s\n", shader_path);
            file_name = pc;
            end = true;
            break;
          default:
            return ERROR_INVALID;
          }
          remain--;
        }
      } else {
        return ERROR_INVALID;
      }
    }
  }

  size_t max_fname = PATH_MAX - strlen(shader_path);

  for(ui32 ui = 0; ui < nude::ProgramList_Num; ui++) {
    const nude::Program& prog = nude::GLSLPrograms[ui];
    Data& data = mData[ui];

    data.attribute = 0;

    if(prog.vsh_file) {
      snprintf(file_name, max_fname, "%s", prog.vsh_file);
      file_name[max_fname - 1] = 0x00;
      NU_TRACE("Reading VSH: %s\n", shader_path);

      nuFile vsh(nude::FATTR_READ, shader_path);
      if(vsh.isOpened()) {
        data.vsh_size = vsh.getSize();
        data.vsh_data = nude::Alloc(data.vsh_size);
        vsh.read(data.vsh_data, data.vsh_size);
      }
    } else {
      data.vsh_data = nullptr;
      data.vsh_size = 0;
    }

    if(prog.fsh_file) {
      snprintf(file_name, max_fname, "%s", prog.fsh_file);
      file_name[max_fname - 1] = 0x00;
      NU_TRACE("Reading FSH: %s\n", shader_path);

      nuFile fsh(nude::FATTR_READ, shader_path);
      if(fsh.isOpened()) {
        data.fsh_size = fsh.getSize();
        data.fsh_data = nude::Alloc(data.fsh_size);
        fsh.read(data.fsh_data, data.fsh_size);
      }
    } else {
      data.fsh_data = nullptr;
      data.fsh_size = 0;
    }
  }

  return ERROR_NONE;
}

void nuShaderList::compileData(Data& data)
{
  data.vsh_id = compileShader(GL_VERTEX_SHADER,
                              (const GLchar**) &data.vsh_data,
                              static_cast< GLint >(data.vsh_size));
  data.fsh_id = compileShader(GL_FRAGMENT_SHADER,
                              (const GLchar**) &data.fsh_data,
                              static_cast< GLint >(data.fsh_size));
  data.initialized = 1;
}

GLuint nuShaderList::compileShader(GLenum shader, const GLchar** sh_data, GLint sh_size)
{
  GLuint ret;
  CHECK_GL_ERROR(ret = glCreateShader(shader));
  CHECK_GL_ERROR(glShaderSource(ret, 1, sh_data, &sh_size));
  CHECK_GL_ERROR(glCompileShader(ret));

  GLint status;
  CHECK_GL_ERROR(glGetShaderiv(ret, GL_COMPILE_STATUS, &status));

  if(status == 0) {
    GLint log_len;
    CHECK_GL_ERROR(glGetShaderiv(ret, GL_INFO_LOG_LENGTH, &log_len));
    if(log_len > 0) {
      nude::TempBuffer<> temp(log_len);
      GLchar* p_log = static_cast< GLchar* >(temp.getBuffer());
      GLsizei len;
      CHECK_GL_ERROR(glGetShaderInfoLog(ret, log_len, &len, p_log));
      NU_TRACE("%s\n", p_log);
    }
    NU_TRACE("Error compiling shader.\n");
    CHECK_GL_ERROR(glDeleteShader(ret));
    ret = 0;
  }

  return ret;
}
