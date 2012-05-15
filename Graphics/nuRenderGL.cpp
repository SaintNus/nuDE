/*!
 * \file nuRenderGL.cpp
 * \brief Renderer.
 * \author Nus
 * \date 2011/10/30 17:40
 */

#include "GraphicsInclude.h"
#include "nuRenderGL.h"

IMPLEMENT_TYPE_INFO(nuRenderGL, nuObject);

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
  mResourceManager.initializeShaderList(resource_mgr, shader_list);

  // Initialize render context.
  mRenderContext.clear_color = nuColor(0);
  CHECK_GL_ERROR(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
  CHECK_GL_ERROR(glEnable(GL_PRIMITIVE_RESTART));

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
}

bool nuRenderGL::render(void)
{
  typedef nuGContext::DrawCmd< i32 > DummyCmd;

  mLock.unlockWithCondition(EXECUTE_PHASE);

  if(mpCurrentTagList) {
    if(mpCurrentTagList->mTagNum > 0) {
      nuGContext::Tag* p_tag = mpCurrentTagList->mpTagList;

      mRenderContext.p_shader_program = nullptr;
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

  if(p_draw->data.program_object.p_shader_program->getHandle() == 0)
    return;
  if(p_draw->data.p_vertex_array->getHandle() == 0)
    return;
  if(p_draw->data.p_vertex_buffer->getHandle() == 0)
    return;
  if(p_draw->data.p_element_buffer->getHandle() == 0)
    return;

  setShaderProgram(context, p_draw->data.program_object);

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

void nuRenderGL::setShaderProgram(RenderContext& context, nuGContext::ProgramObject& program)
{
  if(context.p_shader_program == program.p_shader_program)
    return;

  context.p_shader_program = program.p_shader_program;
  CHECK_GL_ERROR(glUseProgram(context.p_shader_program->getHandle()));
  for(ui32 ui = 0; ui < program.uniform_num; ui++) {
    nuGContext::UniformValue& uniform = program.p_value[ui];
    switch(uniform.type) {
    case nuGContext::UniformValue::FLOAT_1:
      CHECK_GL_ERROR(glUniform1f(uniform.location, *static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::FLOAT_2:
      {
        f32* val = static_cast< f32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform2f(uniform.location, val[0], val[1]));
      }
      break;
    case nuGContext::UniformValue::FLOAT_3:
      {
        f32* val = static_cast< f32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform3f(uniform.location, val[0], val[1], val[2]));
      }
      break;
    case nuGContext::UniformValue::FLOAT_4:
      {
        f32* val = static_cast< f32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform4f(uniform.location, val[0], val[1], val[2], val[3]));
      }
      break;
    case nuGContext::UniformValue::INT_1:
      CHECK_GL_ERROR(glUniform1i(uniform.location, *static_cast< i32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::INT_2:
      {
        i32* val = static_cast< i32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform2i(uniform.location, val[0], val[1]));
      }
      break;
    case nuGContext::UniformValue::INT_3:
      {
        i32* val = static_cast< i32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform3i(uniform.location, val[0], val[1], val[2]));
      }
      break;
    case nuGContext::UniformValue::INT_4:
      {
        i32* val = static_cast< i32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform4i(uniform.location, val[0], val[1], val[2], val[3]));
      }
      break;
    case nuGContext::UniformValue::UINT_1:
      CHECK_GL_ERROR(glUniform1ui(uniform.location, *static_cast< ui32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::UINT_2:
      {
        ui32* val = static_cast< ui32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform2ui(uniform.location, val[0], val[1]));
      }
      break;
    case nuGContext::UniformValue::UINT_3:
      {
        ui32* val = static_cast< ui32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform3ui(uniform.location, val[0], val[1], val[2]));
      }
      break;
    case nuGContext::UniformValue::UINT_4:
      {
        ui32* val = static_cast< ui32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform4ui(uniform.location, val[0], val[1], val[2], val[3]));
      }
      break;
    case nuGContext::UniformValue::FLOAT_1_V:
      CHECK_GL_ERROR(glUniform1fv(uniform.location, uniform.count, static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::FLOAT_2_V:
      CHECK_GL_ERROR(glUniform2fv(uniform.location, uniform.count, static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::FLOAT_3_V:
      CHECK_GL_ERROR(glUniform3fv(uniform.location, uniform.count, static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::FLOAT_4_V:
      CHECK_GL_ERROR(glUniform4fv(uniform.location, uniform.count, static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::INT_1_V:
      CHECK_GL_ERROR(glUniform1iv(uniform.location, uniform.count, static_cast< i32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::INT_2_V:
      CHECK_GL_ERROR(glUniform2iv(uniform.location, uniform.count, static_cast< i32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::INT_3_V:
      CHECK_GL_ERROR(glUniform3iv(uniform.location, uniform.count, static_cast< i32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::INT_4_V:
      CHECK_GL_ERROR(glUniform4iv(uniform.location, uniform.count, static_cast< i32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::UINT_1_V:
      CHECK_GL_ERROR(glUniform1uiv(uniform.location, uniform.count, static_cast< ui32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::UINT_2_V:
      CHECK_GL_ERROR(glUniform2uiv(uniform.location, uniform.count, static_cast< ui32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::UINT_3_V:
      CHECK_GL_ERROR(glUniform3uiv(uniform.location, uniform.count, static_cast< ui32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::UINT_4_V:
      CHECK_GL_ERROR(glUniform4uiv(uniform.location, uniform.count, static_cast< ui32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_2_V:
      CHECK_GL_ERROR(glUniformMatrix2fv(uniform.location,
                                        uniform.count,
                                        uniform.transpose ? GL_TRUE : GL_FALSE,
                                        static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_3_V:
      CHECK_GL_ERROR(glUniformMatrix3fv(uniform.location,
                                        uniform.count,
                                        uniform.transpose ? GL_TRUE : GL_FALSE,
                                        static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_4_V:
      CHECK_GL_ERROR(glUniformMatrix4fv(uniform.location,
                                        uniform.count,
                                        uniform.transpose ? GL_TRUE : GL_FALSE,
                                        static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_2x3_V:
      CHECK_GL_ERROR(glUniformMatrix2x3fv(uniform.location,
                                          uniform.count,
                                          uniform.transpose ? GL_TRUE : GL_FALSE,
                                          static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_3x2_V:
      CHECK_GL_ERROR(glUniformMatrix3x2fv(uniform.location,
                                          uniform.count,
                                          uniform.transpose ? GL_TRUE : GL_FALSE,
                                          static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_2x4_V:
      CHECK_GL_ERROR(glUniformMatrix2x4fv(uniform.location,
                                          uniform.count,
                                          uniform.transpose ? GL_TRUE : GL_FALSE,
                                          static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_4x2_V:
      CHECK_GL_ERROR(glUniformMatrix4x2fv(uniform.location,
                                          uniform.count,
                                          uniform.transpose ? GL_TRUE : GL_FALSE,
                                          static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_3x4_V:
      CHECK_GL_ERROR(glUniformMatrix3x4fv(uniform.location,
                                          uniform.count,
                                          uniform.transpose ? GL_TRUE : GL_FALSE,
                                          static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_4x3_V:
      CHECK_GL_ERROR(glUniformMatrix4x3fv(uniform.location,
                                          uniform.count,
                                          uniform.transpose ? GL_TRUE : GL_FALSE,
                                          static_cast< f32* >(uniform.p_data)));
      break;
    default:
      NU_ASSERT_C(false);
    }
  }
}
