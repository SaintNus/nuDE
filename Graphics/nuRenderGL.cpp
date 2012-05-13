/*!
 * \file nuRenderGL.cpp
 * \brief Renderer.
 * \author Nus
 * \date 2011/10/30 17:40
 */

#include "GraphicsInclude.h"
#include "nuRenderGL.h"

IMPLEMENT_TYPE_INFO(nuRenderGL, nuObject);

struct Vertex {
  f32 pos[3];
  f32 color[4];
};

static GLuint vsh_id = 0;
static GLuint fsh_id = 0;
static GLuint prog_id = 0;

nuRenderGL::nuRenderGL()
    : mFrameID(0),
      mLock(INIT_PHASE),
      mpNextTagList(nullptr),
      mpCurrentTagList(nullptr)
{
  // None...
}

nuRenderGL::~nuRenderGL()
{
  // None...
}

void nuRenderGL::initialize(nuResourceManager& resource_mgr, ccstr shader_list)
{
  mProgramManager.initializeResource(resource_mgr, shader_list);

  nuFile vsh(nude::FATTR_READ, "res://Resources/Shader/processed/Debug.vsh");
  nuFile fsh(nude::FATTR_READ, "res://Resources/Shader/processed/Debug.fsh");
  void* vsh_buffer = nude::Alloc(vsh.getSize());
  void* fsh_buffer = nude::Alloc(fsh.getSize());

  GLint vsh_len = static_cast< GLint >(vsh.getSize());
  GLint fsh_len = static_cast< GLint >(fsh.getSize());

  GLint status;

  CHECK_GL_ERROR(vsh_id = glCreateShader(GL_VERTEX_SHADER));
  CHECK_GL_ERROR(fsh_id = glCreateShader(GL_FRAGMENT_SHADER));

  vsh.read(vsh_buffer, vsh.getSize());
  CHECK_GL_ERROR(glShaderSource(vsh_id, 1, (const GLchar**) &vsh_buffer, &vsh_len));
  CHECK_GL_ERROR(glCompileShader(vsh_id));

  CHECK_GL_ERROR(glGetShaderiv(vsh_id, GL_COMPILE_STATUS, &status));
  if(status == 0) {
    GLint log_len;
    CHECK_GL_ERROR(glGetShaderiv(vsh_id, GL_INFO_LOG_LENGTH, &log_len));
    if(log_len > 0) {
      GLchar* p_log = static_cast< GLchar* >(nude::Alloc(log_len));
      GLsizei len;
      CHECK_GL_ERROR(glGetShaderInfoLog(vsh_id, log_len, &len, p_log));
      NU_TRACE("%s\n", p_log);
      nude::Dealloc(p_log);
    }
    NU_TRACE("Error compiling vertex shader.\n");
  }

  fsh.read(fsh_buffer, fsh.getSize());
  CHECK_GL_ERROR(glShaderSource(fsh_id, 1, (const GLchar**) &fsh_buffer, &fsh_len));
  CHECK_GL_ERROR(glCompileShader(fsh_id));

  CHECK_GL_ERROR(glGetShaderiv(fsh_id, GL_COMPILE_STATUS, &status));
  if(status == 0) {
    GLint log_len;
    CHECK_GL_ERROR(glGetShaderiv(fsh_id, GL_INFO_LOG_LENGTH, &log_len));
    if(log_len > 0) {
      GLchar* p_log = static_cast< GLchar* >(nude::Alloc(log_len));
      GLsizei len;
      CHECK_GL_ERROR(glGetShaderInfoLog(fsh_id, log_len, &len, p_log));
      NU_TRACE("%s\n", p_log);
      nude::Dealloc(p_log);
    }
    NU_TRACE("Error compiling fragment shader.\n");
  }

  CHECK_GL_ERROR(prog_id = glCreateProgram());

  CHECK_GL_ERROR(glAttachShader(prog_id, vsh_id));
  CHECK_GL_ERROR(glAttachShader(prog_id, fsh_id));

  CHECK_GL_ERROR(glBindAttribLocation(prog_id, 0, "inPosition"));
  CHECK_GL_ERROR(glBindAttribLocation(prog_id, 1, "inColor"));

  CHECK_GL_ERROR(glLinkProgram(prog_id));

  CHECK_GL_ERROR(glGetProgramiv(prog_id, GL_LINK_STATUS, &status));
  if(status == 0) {
    GLint log_len;
    CHECK_GL_ERROR(glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &log_len));
    if(log_len > 0) {
      GLchar* p_log = static_cast< GLchar* >(nude::Alloc(log_len));
      GLsizei len;
      CHECK_GL_ERROR(glGetProgramInfoLog(prog_id, log_len, &len, p_log));
      NU_TRACE("%s\n", p_log);
      nude::Dealloc(p_log);
    }
    NU_TRACE("Error linking program.\n");
  }

  CHECK_GL_ERROR(glValidateProgram(prog_id));
  CHECK_GL_ERROR(glGetProgramiv(prog_id, GL_VALIDATE_STATUS, &status));
  if(status == 0) {
    GLint log_len;
    CHECK_GL_ERROR(glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &log_len));
    if(log_len > 0) {
      GLchar* p_log = static_cast< GLchar* >(nude::Alloc(log_len));
      GLsizei len;
      CHECK_GL_ERROR(glGetProgramInfoLog(prog_id, log_len, &len, p_log));
      NU_TRACE("%s\n", p_log);
      nude::Dealloc(p_log);
    }
    NU_TRACE("Error validating program.\n");
  }

  nude::Dealloc(vsh_buffer);
  nude::Dealloc(fsh_buffer);
  CHECK_GL_ERROR(glEnable(GL_PRIMITIVE_RESTART));

  // Initialize render context.
  mRenderContext.clear_color = nuColor(0);
  CHECK_GL_ERROR(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));

  mRenderContext.depth_value = 1.0f;
  CHECK_GL_ERROR(glClearDepth(1.0f));

  mRenderContext.p_vertex_array = nullptr;
  mRenderContext.p_vertex_buffer = nullptr;
  mRenderContext.p_element_buffer = nullptr;

  CHECK_GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void nuRenderGL::terminate(void)
{
  mLock.lockWhenCondition(SETUP_PHASE);
  mLock.unlockWithCondition(EXECUTE_PHASE);

  CHECK_GL_ERROR(glDeleteShader(vsh_id));
  CHECK_GL_ERROR(glDeleteShader(fsh_id));
  CHECK_GL_ERROR(glDeleteProgram(prog_id));

  mProgramManager.terminateResource();
}

bool nuRenderGL::render(void)
{
  typedef nuGContext::DrawCmd< i32 > DummyCmd;

  mLock.unlockWithCondition(EXECUTE_PHASE);

  if(mpCurrentTagList) {
    if(mpCurrentTagList->mTagNum > 0) {
      nuGContext::Tag* p_tag = mpCurrentTagList->mpTagList;

      mRenderContext.p_vertex_array = nullptr;
      mRenderContext.p_vertex_buffer = nullptr;
      mRenderContext.p_element_buffer = nullptr;

      for(ui32 ui = 0; ui < mpCurrentTagList->mTagNum; ui++) {
        DummyCmd* p_dummy = static_cast< DummyCmd* >(p_tag[ui].mpCommand);

        switch(p_dummy->type) {
        case nuGContext::CLEAR:
          executeClear(mRenderContext, p_tag[ui].mpCommand);
          break;
        case nuGContext::DRAW_ELEMENTS:
          CHECK_GL_ERROR(glUseProgram(prog_id));
          executeDrawElements(mRenderContext, p_tag[ui].mpCommand);
          break;
        default:
          NU_ASSERT(false, "Logical error.\n");
        }
      }
      
      return true;
    }
  }

  return false;
}

i64 nuRenderGL::updateGraphicResources(void)
{
  mFrameID++;
  mResourceManager.updateStaticResource(mFrameID);
  mResourceManager.updateDynamicResource(mFrameID);
  return mFrameID;
}

i64 nuRenderGL::synchronize(void)
{
  mLock.unlockWithCondition(SETUP_PHASE);
  mLock.lockWhenCondition(EXECUTE_PHASE);
  return mFrameID;
}

bool nuRenderGL::isCommandSubmitted(void)
{
  if(mLock.trylockWhenCondition(SETUP_PHASE)) {
    mpCurrentTagList = mpNextTagList;
    mpNextTagList = nullptr;
    return true;
  }
  return false;
}

void nuRenderGL::executeClear(RenderContext& context, void* clear_cmd)
{
  nuGContext::ClearCmd* p_clear = static_cast< nuGContext::ClearCmd* >(clear_cmd);
  nuColor clear_color(p_clear->data.clear_color);

  if(context.clear_color != clear_color)
    CHECK_GL_ERROR(glClearColor(clear_color.fr(), clear_color.fg(), clear_color.fb(), clear_color.fa()));
  
  if(fabsf(p_clear->data.depth_value - context.depth_value) > 0.000001f)
    CHECK_GL_ERROR(glClearDepth(p_clear->data.depth_value));

  CHECK_GL_ERROR(glClear(p_clear->data.clear_bit));
}

void nuRenderGL::executeDrawElements(RenderContext& context, void* draw_cmd)
{
  nuGContext::DrawElementsCmd* p_draw = static_cast< nuGContext::DrawElementsCmd* >(draw_cmd);

  if(p_draw->data.p_vertex_array->getHandle() == 0)
    return;
  if(p_draw->data.p_vertex_buffer->getHandle() == 0)
    return;
  if(p_draw->data.p_element_buffer->getHandle() == 0)
    return;

  if(context.p_vertex_array != p_draw->data.p_vertex_array)
    context.p_vertex_array = p_draw->data.p_vertex_array;

  if(context.p_vertex_buffer != p_draw->data.p_vertex_buffer)
    context.p_vertex_buffer = p_draw->data.p_vertex_buffer;

  if(context.p_vertex_array->getVertexHandle() != context.p_vertex_buffer->getHandle())
    context.p_vertex_array->reset(context.p_vertex_buffer->getHandle());
  else
    CHECK_GL_ERROR(glBindVertexArray(context.p_vertex_array->getHandle()));

  if(context.p_element_buffer != p_draw->data.p_element_buffer) {
    const GLuint restart_idx[] = {
      0xffff,
      0xffffffff,
    };
    if(context.p_element_buffer) {
      nuElementBuffer::ELEMENT_TYPE prev_type = context.p_element_buffer->getElementType();
      if(prev_type != p_draw->data.p_element_buffer->getElementType())
        CHECK_GL_ERROR(glPrimitiveRestartIndex(restart_idx[p_draw->data.p_element_buffer->getElementType()]));
    } else {
      CHECK_GL_ERROR(glPrimitiveRestartIndex(restart_idx[p_draw->data.p_element_buffer->getElementType()]));
    }

    context.p_element_buffer = p_draw->data.p_element_buffer;
    CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.p_element_buffer->getHandle()));
  }

  const GLenum primitive_mode[] = {
    GL_POINTS,

    GL_LINE_STRIP,
    GL_LINE_LOOP,
    GL_LINES,

    GL_TRIANGLE_STRIP,
    GL_TRIANGLE_FAN,
    GL_TRIANGLES,

    GL_LINES_ADJACENCY,
    GL_LINE_STRIP_ADJACENCY,
    GL_TRIANGLES_ADJACENCY,
    GL_TRIANGLE_STRIP_ADJACENCY,
  };

  const GLenum element_type[] = {
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_INT,
  };

  CHECK_GL_ERROR(glDrawElements(primitive_mode[p_draw->data.primitive_mode],
                                p_draw->data.element_num,
                                element_type[context.p_element_buffer->getElementType()],
                                0));
}
