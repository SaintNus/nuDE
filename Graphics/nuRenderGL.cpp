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

void nuRenderGL::initialize(nude::ShaderList& shader_list)
{
  mCapabilities.initialize();
  mResourceManager.setShaderList(shader_list);

  // Initialize render context.
  mRenderContext.clear_color = nuColor(0);
  CHECK_GL_ERROR(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
  CHECK_GL_ERROR(glEnable(GL_PRIMITIVE_RESTART));

  mRenderContext.depth_value = 1.0f;
  CHECK_GL_ERROR(glClearDepth(1.0f));

  mRenderContext.reset();

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

      mRenderContext.reset();

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

  setViewport(context, *p_clear->p_viewport);

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

  setViewport(context, *p_draw->p_viewport);
  setFragmentOps(context, p_draw->data.fragment_ops);

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
  if(context.p_shader_program != program.p_shader_program) {
    context.p_shader_program = program.p_shader_program;
    CHECK_GL_ERROR(glUseProgram(context.p_shader_program->getHandle()));
  }

  if(program.p_value && context.p_shader_program->mpRenderedUniformValue != program.p_value)
    setUniformValue(context, program);

  if(program.p_block && context.p_shader_program->mpRenderedUniformBlock != program.p_block)
    setUniformBlock(context, program);
}

void nuRenderGL::setUniformValue(RenderContext& context, nuGContext::ProgramObject& program)
{
  context.p_shader_program->setRenderedUniformValue(program.p_value);

  for(ui32 ui = 0; ui < context.p_shader_program->getUniformNum(); ui++) {
    nuGContext::UniformValue& uniform = program.p_value[ui];

    if(!uniform.p_data)
      continue;

    GLuint location = context.p_shader_program->getUniformLocation(ui);

    switch(uniform.type) {
    case nuGContext::UniformValue::FLOAT_1:
      CHECK_GL_ERROR(glUniform1f(location, *static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::FLOAT_2:
      {
        f32* val = static_cast< f32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform2f(location, val[0], val[1]));
      }
      break;
    case nuGContext::UniformValue::FLOAT_3:
      {
        f32* val = static_cast< f32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform3f(location, val[0], val[1], val[2]));
      }
      break;
    case nuGContext::UniformValue::FLOAT_4:
      {
        f32* val = static_cast< f32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform4f(location, val[0], val[1], val[2], val[3]));
      }
      break;
    case nuGContext::UniformValue::INT_1:
      CHECK_GL_ERROR(glUniform1i(location, *static_cast< i32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::INT_2:
      {
        i32* val = static_cast< i32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform2i(location, val[0], val[1]));
      }
      break;
    case nuGContext::UniformValue::INT_3:
      {
        i32* val = static_cast< i32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform3i(location, val[0], val[1], val[2]));
      }
      break;
    case nuGContext::UniformValue::INT_4:
      {
        i32* val = static_cast< i32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform4i(location, val[0], val[1], val[2], val[3]));
      }
      break;
    case nuGContext::UniformValue::UINT_1:
      CHECK_GL_ERROR(glUniform1ui(location, *static_cast< ui32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::UINT_2:
      {
        ui32* val = static_cast< ui32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform2ui(location, val[0], val[1]));
      }
      break;
    case nuGContext::UniformValue::UINT_3:
      {
        ui32* val = static_cast< ui32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform3ui(location, val[0], val[1], val[2]));
      }
      break;
    case nuGContext::UniformValue::UINT_4:
      {
        ui32* val = static_cast< ui32* >(uniform.p_data);
        CHECK_GL_ERROR(glUniform4ui(location, val[0], val[1], val[2], val[3]));
      }
      break;
    case nuGContext::UniformValue::FLOAT_1_V:
      CHECK_GL_ERROR(glUniform1fv(location, uniform.count, static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::FLOAT_2_V:
      CHECK_GL_ERROR(glUniform2fv(location, uniform.count, static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::FLOAT_3_V:
      CHECK_GL_ERROR(glUniform3fv(location, uniform.count, static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::FLOAT_4_V:
      CHECK_GL_ERROR(glUniform4fv(location, uniform.count, static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::INT_1_V:
      CHECK_GL_ERROR(glUniform1iv(location, uniform.count, static_cast< i32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::INT_2_V:
      CHECK_GL_ERROR(glUniform2iv(location, uniform.count, static_cast< i32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::INT_3_V:
      CHECK_GL_ERROR(glUniform3iv(location, uniform.count, static_cast< i32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::INT_4_V:
      CHECK_GL_ERROR(glUniform4iv(location, uniform.count, static_cast< i32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::UINT_1_V:
      CHECK_GL_ERROR(glUniform1uiv(location, uniform.count, static_cast< ui32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::UINT_2_V:
      CHECK_GL_ERROR(glUniform2uiv(location, uniform.count, static_cast< ui32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::UINT_3_V:
      CHECK_GL_ERROR(glUniform3uiv(location, uniform.count, static_cast< ui32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::UINT_4_V:
      CHECK_GL_ERROR(glUniform4uiv(location, uniform.count, static_cast< ui32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_2_V:
      CHECK_GL_ERROR(glUniformMatrix2fv(location,
                                        uniform.count,
                                        uniform.transpose ? GL_TRUE : GL_FALSE,
                                        static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_3_V:
      CHECK_GL_ERROR(glUniformMatrix3fv(location,
                                        uniform.count,
                                        uniform.transpose ? GL_TRUE : GL_FALSE,
                                        static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_4_V:
      CHECK_GL_ERROR(glUniformMatrix4fv(location,
                                        uniform.count,
                                        uniform.transpose ? GL_TRUE : GL_FALSE,
                                        static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_2x3_V:
      CHECK_GL_ERROR(glUniformMatrix2x3fv(location,
                                          uniform.count,
                                          uniform.transpose ? GL_TRUE : GL_FALSE,
                                          static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_3x2_V:
      CHECK_GL_ERROR(glUniformMatrix3x2fv(location,
                                          uniform.count,
                                          uniform.transpose ? GL_TRUE : GL_FALSE,
                                          static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_2x4_V:
      CHECK_GL_ERROR(glUniformMatrix2x4fv(location,
                                          uniform.count,
                                          uniform.transpose ? GL_TRUE : GL_FALSE,
                                          static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_4x2_V:
      CHECK_GL_ERROR(glUniformMatrix4x2fv(location,
                                          uniform.count,
                                          uniform.transpose ? GL_TRUE : GL_FALSE,
                                          static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_3x4_V:
      CHECK_GL_ERROR(glUniformMatrix3x4fv(location,
                                          uniform.count,
                                          uniform.transpose ? GL_TRUE : GL_FALSE,
                                          static_cast< f32* >(uniform.p_data)));
      break;
    case nuGContext::UniformValue::MATRIX_4x3_V:
      CHECK_GL_ERROR(glUniformMatrix4x3fv(location,
                                          uniform.count,
                                          uniform.transpose ? GL_TRUE : GL_FALSE,
                                          static_cast< f32* >(uniform.p_data)));
      break;
    default:
      NU_ASSERT_C(false);
    }
  }
}

void nuRenderGL::setUniformBlock(RenderContext& context, nuGContext::ProgramObject& program)
{
  context.p_shader_program->setRenderedUniformBlock(program.p_block);

  for(ui32 ui = 0; ui < context.p_shader_program->getUniformBlockNum(); ui++) {
    nuGContext::UniformBlock& ub = program.p_block[ui];

    if(ub.p_buffer && ub.p_buffer->getHandle() > 0) {
      CHECK_GL_ERROR(glUniformBlockBinding(context.p_shader_program->getHandle(),
                                           context.p_shader_program->getUniformBlockIndex(ui),
                                           ui));
      CHECK_GL_ERROR(glBindBufferBase(GL_UNIFORM_BUFFER, ui, ub.p_buffer->getHandle()));
    }
  }
}

void nuRenderGL::setViewport(RenderContext& context, const nuGContext::Viewport& viewport)
{
  if(viewport != context.viewport) {
    context.viewport = viewport;
    CHECK_GL_ERROR(glViewport(context.viewport.origin_x,
                              context.viewport.origin_y,
                              context.viewport.width,
                              context.viewport.height));
  }
}

void nuRenderGL::setFragmentOps(RenderContext& context, nuGContext::FragmentOps& fragment_ops)
{
  if(*fragment_ops.p_scissor != context.scissor) {
    context.scissor = *fragment_ops.p_scissor;

    if(context.scissor.enable)
      CHECK_GL_ERROR(glEnable(GL_SCISSOR_TEST));
    else
      CHECK_GL_ERROR(glDisable(GL_SCISSOR_TEST));

    CHECK_GL_ERROR(glScissor(context.scissor.left,
                             context.scissor.bottom,
                             context.scissor.width,
                             context.scissor.height));
  }

  if(*fragment_ops.p_depth_test != context.depth_test) {
    context.depth_test = *fragment_ops.p_depth_test;

    if(context.depth_test.enable)
      CHECK_GL_ERROR(glEnable(GL_DEPTH_TEST));
    else
      CHECK_GL_ERROR(glDisable(GL_DEPTH_TEST));

    CHECK_GL_ERROR(glDepthFunc(context.depth_test.function));
  }
}
