/*!
 * \file nuRenderGL.cpp
 * \brief Renderer.
 * \author Nus
 * \date 2011/10/30 17:40
 */

#include "nuRenderGL.h"

IMPLEMENT_TYPE_INFO(nuRenderGL, nuObject);

struct Vertex {
  f32 pos[3];
  f32 color[4];
};

static GLuint vsh_id = 0;
static GLuint fsh_id = 0;
static GLuint prog_id = 0;
static nude::VertexBuffer vtx_buffer;
static nude::ElementBuffer idx_buffer;
static GLint vertex_loc = 0;
static GLint color_loc = 0;

nuRenderGL::nuRenderGL()
    : mFrameID(0),
      mLock(INIT_PHASE)
{

}

nuRenderGL::~nuRenderGL()
{

}

void nuRenderGL::initialize(void)
{
  nuFile vsh(nude::FATTR_READ, "res://Resources/Shader/processed/Debug.vsh");
  nuFile fsh(nude::FATTR_READ, "res://Resources/Shader/processed/Debug.fsh");
  void* vsh_buffer = nude::Alloc(vsh.getSize());
  void* fsh_buffer = nude::Alloc(fsh.getSize());

  GLint vsh_len = static_cast< GLint >(vsh.getSize());
  GLint fsh_len = static_cast< GLint >(fsh.getSize());

  GLint status;

  vsh_id = glCreateShader(GL_VERTEX_SHADER);
  fsh_id = glCreateShader(GL_FRAGMENT_SHADER);

  vsh.read(vsh_buffer, vsh.getSize());
  glShaderSource(vsh_id, 1, (const GLchar**) &vsh_buffer, &vsh_len);
  glCompileShader(vsh_id);

  glGetShaderiv(vsh_id, GL_COMPILE_STATUS, &status);
  if(status == 0) {
    GLint log_len;
    glGetShaderiv(vsh_id, GL_INFO_LOG_LENGTH, &log_len);
    if(log_len > 0) {
      GLchar* p_log = static_cast< GLchar* >(nude::Alloc(log_len));
      GLsizei len;
      glGetShaderInfoLog(vsh_id, log_len, &len, p_log);
      NU_TRACE("%s\n", p_log);
      nude::Dealloc(p_log);
    }
    NU_TRACE("Error compiling vertex shader.\n");
  }

  fsh.read(fsh_buffer, fsh.getSize());
  glShaderSource(fsh_id, 1, (const GLchar**) &fsh_buffer, &fsh_len);
  glCompileShader(fsh_id);

  glGetShaderiv(fsh_id, GL_COMPILE_STATUS, &status);
  if(status == 0) {
    GLint log_len;
    glGetShaderiv(fsh_id, GL_INFO_LOG_LENGTH, &log_len);
    if(log_len > 0) {
      GLchar* p_log = static_cast< GLchar* >(nude::Alloc(log_len));
      GLsizei len;
      glGetShaderInfoLog(fsh_id, log_len, &len, p_log);
      NU_TRACE("%s\n", p_log);
      nude::Dealloc(p_log);
    }
    NU_TRACE("Error compiling fragment shader.\n");
  }

  prog_id = glCreateProgram();
  glAttachShader(prog_id, vsh_id);
  glAttachShader(prog_id, fsh_id);
  glLinkProgram(prog_id);

  glGetProgramiv(prog_id, GL_LINK_STATUS, &status);
  if(status == 0) {
    GLint log_len;
    glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &log_len);
    if(log_len > 0) {
      GLchar* p_log = static_cast< GLchar* >(nude::Alloc(log_len));
      GLsizei len;
      glGetProgramInfoLog(prog_id, log_len, &len, p_log);
      NU_TRACE("%s\n", p_log);
      nude::Dealloc(p_log);
    }
    NU_TRACE("Error linking program.\n");
  }

  glValidateProgram(prog_id);
  glGetProgramiv(prog_id, GL_VALIDATE_STATUS, &status);
  if(status == 0) {
    GLint log_len;
    glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &log_len);
    if(log_len > 0) {
      GLchar* p_log = static_cast< GLchar* >(nude::Alloc(log_len));
      GLsizei len;
      glGetProgramInfoLog(prog_id, log_len, &len, p_log);
      NU_TRACE("%s\n", p_log);
      nude::Dealloc(p_log);
    }
    NU_TRACE("Error validating program.\n");
  }

  {
    GLint attrib, attrib_len;
    glGetProgramiv(prog_id, GL_ACTIVE_ATTRIBUTES, &attrib);
    glGetProgramiv(prog_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attrib_len);
    GLchar* p_attr = static_cast< GLchar* >(nude::Alloc(attrib_len));
    for(GLuint ui = 0; ui < static_cast< GLuint >(attrib); ui++) {
      GLsizei len;
      GLint size;
      GLenum type;
      glGetActiveAttrib(prog_id, ui, attrib_len, &len, &size, &type, p_attr);
      NU_TRACE("len: %d, size: %d, type: 0x%x, name: %s\n", len, size, type, p_attr);
      if(strcmp(p_attr, "inPosition") == 0)
        vertex_loc = glGetAttribLocation(prog_id, p_attr);
      else if(strcmp(p_attr, "inColor") == 0)
        color_loc = glGetAttribLocation(prog_id, p_attr);
    }
    nude::Dealloc(p_attr);
  }

  nude::Dealloc(vsh_buffer);
  nude::Dealloc(fsh_buffer);

  {
    ui16 idx[3] = { 0, 1, 2 };
    Vertex vtx[3] = {
      {
        { 0.0f, 0.6f, 0.0f },
        { 1.0f, 0.85f, 0.35f, 1.0f }
      },
      {
        { -0.2f, -0.3f, 0.0f },
        { 1.0f, 0.85f, 0.35f, 1.0f }
      },
      {
        { 0.2f, -0.3f, 0.0f },
        { 1.0f, 0.85f, 0.35f, 1.0f }
      },
    };

    {
      vtx_buffer = createVertexBuffer(sizeof(vtx), nuGResource::STATIC_RESOURCE);
      void* p_buffer = vtx_buffer->getBuffer();
      memcpy(p_buffer, vtx, sizeof(vtx));
      vtx_buffer->commit(sizeof(vtx));

      vtx_buffer->beginArrayDeclaration(sizeof(Vertex));
      {
        vtx_buffer->declare(nuVertexBuffer::VertexArray(vertex_loc,
                                                        3,
                                                        nuVertexBuffer::FLOAT,
                                                        false,
                                                        0));
        vtx_buffer->declare(nuVertexBuffer::VertexArray(color_loc,
                                                        4,
                                                        nuVertexBuffer::FLOAT,
                                                        false,
                                                        sizeof(f32) * 3));
      }
      vtx_buffer->endArrayDeclaration();
    }

    {
      idx_buffer = createElementBuffer(nuElementBuffer::UNSIGNED_INT_16,
                                       sizeof(idx) / sizeof(ui16),
                                       nuGResource::nuGResource::STATIC_RESOURCE);
      void* p_buffer = idx_buffer->getBuffer();
      memcpy(p_buffer, idx, sizeof(idx));
      idx_buffer->commit(sizeof(idx) / sizeof(ui16));
    }
  }

}

void nuRenderGL::terminate(void)
{
  mLock.lockWhenCondition(SETUP_PHASE);
  mLock.unlockWithCondition(EXECUTE_PHASE);

  glDeleteShader(vsh_id);
  glDeleteShader(fsh_id);
  glDeleteProgram(prog_id);

  vtx_buffer.release();
  idx_buffer.release();
}

i32 nuRenderGL::render(void)
{
  mLock.unlockWithCondition(EXECUTE_PHASE);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(prog_id);

  vtx_buffer->bind();
  idx_buffer->bind();

  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

  return 0;
}

i64 nuRenderGL::updateGraphicResources(void)
{
  mFrameID++;
  mResourceManager.updateStaticResource(mFrameID);
  mResourceManager.updateDynamicResource(mFrameID);
  return mFrameID;
}

void nuRenderGL::synchronize(void)
{
  mLock.unlockWithCondition(SETUP_PHASE);
  mLock.lockWhenCondition(EXECUTE_PHASE);
}

bool nuRenderGL::isCommandSubmitted(void)
{
  return mLock.trylockWhenCondition(SETUP_PHASE);
}
