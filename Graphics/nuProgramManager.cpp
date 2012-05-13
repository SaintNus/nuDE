/*!
 * \file nuProgramManager.cpp
 * \brief GLSL program manager.
 * \author Nus
 * \date 2012/05/05 23:12
 */

#include "GraphicsInclude.h"
#include "nuProgramManager.h"

IMPLEMENT_TYPE_INFO(nuProgramManager::ShaderList, nuResource);
IMPLEMENT_TYPE_INFO(nuProgramManager, nuObject);

nuProgramManager::ShaderList::ShaderList()
{
  for(ui32 ui = 0; ui < nude::ProgramList_Num; ui++) {
    if(mData[ui].vsh_data)
      mData[ui].vsh_data = nullptr;
    if(mData[ui].fsh_data)
      mData[ui].fsh_data = nullptr;
  }
}

nuProgramManager::ShaderList::~ShaderList()
{
  for(ui32 ui = 0; ui < nude::ProgramList_Num; ui++) {
    if(mData[ui].vsh_data)
      nude::Dealloc(mData[ui].vsh_data);
    if(mData[ui].fsh_data)
      nude::Dealloc(mData[ui].fsh_data);
  }
}

nuResource::ERROR_CODE nuProgramManager::ShaderList::setup(const nuStream &stream)
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
            if(pc[-1] != '/') {
              *pc = '/';
              pc++;
            }
            *pc = 0x00;
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

nuProgramManager::nuProgramManager()
{

}

nuProgramManager::~nuProgramManager()
{

}

void nuProgramManager::initializeResource(nuResourceManager& resource_mgr, ccstr file_path)
{
  if(mShdListHandle.isValid())
    return;

  nuResHandle res = resource_mgr.createResource(file_path);
  mShdListHandle = res.cast< nuProgramManager::ShaderList >();
}

void nuProgramManager::terminateResource(void)
{
  for(ui32 ui = 0; ui < nude::ProgramList_Num; ui++) {
    ShaderProgram& prog = mShaderProgram[ui];
    if(prog.mProgID) {
      CHECK_GL_ERROR(glDeleteProgram(prog.mProgID));
      prog.mProgID = 0;
    }
    if(prog.mVshID) {
      CHECK_GL_ERROR(glDeleteShader(prog.mVshID));
      prog.mVshID = 0;
    }
    if(prog.mFshID) {
      CHECK_GL_ERROR(glDeleteShader(prog.mFshID));
      prog.mFshID = 0;
    }
  }
}

void nuProgramManager::update(i64 frame_id)
{
  for(ui32 ui = 0; ui < nude::ProgramList_Num; ui++) {
    ShaderProgram& prog = mShaderProgram[ui];
    const nude::Program& glsl_prog = nude::GLSLPrograms[ui];

    if(prog.mFrameID < frame_id) {
      if(prog.mRefCount == 0) {
        CHECK_GL_ERROR(glDeleteProgram(prog.mProgID));
        prog.mProgID = 0;

        if(prog.mpUniform) {
          for(GLuint uj = 0; uj < glsl_prog.uniform_num; uj++) {
            prog.mpUniform[uj].location = 0;
            prog.mpUniform[uj].data = nullptr;
          }
        }

        if(prog.mpUniformBlock) {
          for(GLuint uj = 0; uj < glsl_prog.uniform_block_num; uj++) {
            prog.mpUniformBlock[uj].location = 0;
            prog.mpUniformBlock[uj].binding = 0;
          }
        }
      }
    } else {
      if(!prog.mVshID) {
        const ShaderList::Data& data = mShdListHandle->getData(ui);
        prog.mVshID = compileShader(GL_VERTEX_SHADER,
                                    static_cast< const GLchar** >(data.vsh_data),
                                    static_cast< GLint >(data.vsh_size));
        if(!prog.mVshID)
          continue;
      }

      if(!prog.mFshID) {
        const ShaderList::Data& data = mShdListHandle->getData(ui);
        prog.mFshID = compileShader(GL_FRAGMENT_SHADER,
                                    static_cast< const GLchar** >(data.fsh_data),
                                    static_cast< GLint >(data.fsh_size));
        if(!prog.mFshID)
          continue;
      }

      CHECK_GL_ERROR(prog.mProgID = glCreateProgram());

      CHECK_GL_ERROR(glAttachShader(prog.mProgID, prog.mVshID));
      CHECK_GL_ERROR(glAttachShader(prog.mProgID, prog.mFshID));

      for(GLuint uj = 0; uj < glsl_prog.attribute_num; ui++)
        CHECK_GL_ERROR(glBindAttribLocation(prog.mProgID, uj, glsl_prog.attributes[uj].name));

      CHECK_GL_ERROR(glLinkProgram(prog.mProgID));

      GLint status;
      CHECK_GL_ERROR(glGetProgramiv(prog.mProgID, GL_LINK_STATUS, &status));
      if(status == 0) {
        GLint log_len;
        CHECK_GL_ERROR(glGetProgramiv(prog.mProgID, GL_INFO_LOG_LENGTH, &log_len));
        if(log_len > 0) {
          nude::TempBuffer<> temp(log_len);
          GLchar* p_log = static_cast< GLchar* >(temp.getBuffer());
          GLsizei len;
          CHECK_GL_ERROR(glGetProgramInfoLog(prog.mProgID, log_len, &len, p_log));
          NU_TRACE("%s\n", p_log);
        }
        NU_TRACE("Error linking program.\n");
        continue;
      }

      CHECK_GL_ERROR(glValidateProgram(prog.mProgID));
      CHECK_GL_ERROR(glGetProgramiv(prog.mProgID, GL_VALIDATE_STATUS, &status));
      if(status == 0) {
        GLint log_len;
        CHECK_GL_ERROR(glGetProgramiv(prog.mProgID, GL_INFO_LOG_LENGTH, &log_len));
        if(log_len > 0) {
          nude::TempBuffer<> temp(log_len);
          GLchar* p_log = static_cast< GLchar* >(temp.getBuffer());
          GLsizei len;
          CHECK_GL_ERROR(glGetProgramInfoLog(prog.mProgID, log_len, &len, p_log));
          NU_TRACE("%s\n", p_log);
        }
        NU_TRACE("Error validating program.\n");
        continue;
      }

      if(glsl_prog.uniform_num > 0) {
        prog.createUniform(glsl_prog.uniform_num);
        for(GLuint uj = 0; uj < glsl_prog.uniform_num; uj++) {
          CHECK_GL_ERROR(prog.mpUniform[uj].location = glGetUniformLocation(prog.mProgID,
                                                                            glsl_prog.uniforms[uj].name));
          prog.mpUniform[uj].data = nullptr;
        }
      }

      if(glsl_prog.uniform_block_num > 0) {
        prog.createUniformBlock(glsl_prog.uniform_block_num);
        for(GLuint uj = 0; uj < glsl_prog.uniform_block_num; uj++) {
          CHECK_GL_ERROR(prog.mpUniformBlock[uj].location = glGetUniformBlockIndex(prog.mProgID,
                                                                                   glsl_prog.uniform_blocks[uj].name));
          prog.mpUniformBlock[uj].binding = 0;
        }
      }
    }
  }
}

GLuint nuProgramManager::compileShader(GLenum shader, const GLchar** p_data, GLint size)
{
  GLuint ret;
  CHECK_GL_ERROR(ret = glCreateShader(shader));
  const GLchar** sh_data = static_cast< const GLchar** >(p_data);
  GLint sh_size = size;
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
    NU_TRACE("Error compiling vertex shader.\n");
    CHECK_GL_ERROR(glDeleteShader(ret));
    ret = 0;
  }

  return ret;
}
