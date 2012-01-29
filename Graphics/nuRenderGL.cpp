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
static GLuint vtx_id = 0;
static GLuint idx_id = 0;
static GLuint vao_id = 0;
static GLint vertex_loc = 0;
static GLint color_loc = 0;

nuRenderGL::nuRenderGL()
{

}

nuRenderGL::~nuRenderGL()
{

}

void nuRenderGL::initTest(void)
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
        { -0.2, -0.3, 0.0f },
        { 1.0f, 0.85f, 0.35f, 1.0f }
      },
      {
        { 0.2f, -0.3f, 0.0f },
        { 1.0f, 0.85f, 0.35f, 1.0f }
      },
    };

    glGenVertexArrays(1, &vao_id);
    glGenBuffers(1, &vtx_id);
    glGenBuffers(1, &idx_id);

    glBindVertexArray(vao_id);

    glBindBuffer(GL_ARRAY_BUFFER, vtx_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vtx), vtx, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertex_loc);
    glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast< GLvoid* >(0));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
    glEnableVertexAttribArray(color_loc);
    glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast< GLvoid* >(sizeof(f32) * 3));
  }

}

void nuRenderGL::termTest(void)
{
  glDeleteShader(vsh_id);
  glDeleteShader(fsh_id);
  glDeleteProgram(prog_id);
  glDeleteBuffers(1, &vtx_id);
  glDeleteBuffers(1, &idx_id);
  glDeleteVertexArrays(1, &vao_id);
}

i32 nuRenderGL::render(void)
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(prog_id);

  glBindVertexArray(vao_id);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

  return 0;
}
