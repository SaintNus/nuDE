/*!
 * \file nuRenderGL.cpp
 * \brief Renderer.
 * \author Nus
 * \date 2011/10/30 17:40
 */

#include "GraphicsInclude.h"
#include "nuDrawString.h"
#include "nuRenderGL.h"

IMPLEMENT_TYPE_INFO(nuRenderGL, nuObject);

nuRenderGL::nuRenderGL()
    : mFrameID(0),
      mLock(INIT_PHASE),
      mpNextTagList(nullptr),
      mpCurrentTagList(nullptr),
      mDefaultVertexArray(0),
      mCurrentVertexArray(0)
{
  mResourceManager.setRenderGL(*this);
}

nuRenderGL::~nuRenderGL()
{
  nuDrawString::terminate();
}

void nuRenderGL::initialize(nude::ShaderList& shader_list)
{
  mResourceManager.setShaderList(shader_list);  
  mCapabilities.initialize();
  nuDrawString::initialize(*this);

  CHECK_GL_ERROR(glGenVertexArrays(1, &mDefaultVertexArray));

  CHECK_GL_ERROR(glBindVertexArray(mDefaultVertexArray));
  CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));
  CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

  for(ui32 ui = 0; ui < nuVertexArray::MAX_VERTEX_ATTRIBUTE; ui++)
    CHECK_GL_ERROR(glDisableVertexAttribArray(static_cast< GLuint >(ui)));

  CHECK_GL_ERROR(glEnable(GL_PRIMITIVE_RESTART));
  CHECK_GL_ERROR(glPrimitiveRestartIndex(0xffffffff));

  mRenderContext.clear_color = nuColor(0);
  mRenderContext.depth_value = 1.0f;
  mRenderContext.stencil_test = nuGContext::StencilTest();

  mRenderContext.viewport.origin_x = static_cast< GLint >(mViewport.origin().x());
  mRenderContext.viewport.origin_y = static_cast< GLint >(mViewport.origin().y());
  mRenderContext.viewport.width = static_cast< GLsizei >(mViewport.size().w());
  mRenderContext.viewport.height = static_cast< GLsizei >(mViewport.size().h());

  mRenderContext.scissor = nuGContext::Scissor();
  mRenderContext.depth_test = nuGContext::DepthTest();
  mRenderContext.blending = nuGContext::Blending();

  mRenderContext.rasterizer.culling = false;
  mRenderContext.rasterizer.line_smooth = false;
  mRenderContext.rasterizer.line_width = 1.0f;
  mRenderContext.rasterizer.cull_face = nude::CULL_BACK;
  mRenderContext.rasterizer.front_face = nude::FRONT_COUNTER_CLOCKWISE;

  mRenderContext.current_array_buffer = 0;
  mRenderContext.current_element_buffer = 0;
  mRenderContext.current_vertex_array = mDefaultVertexArray;
  mRenderContext.restart_index = 0xffffffff;

  initializeStates();
}

void nuRenderGL::terminate(void)
{
  mLock.lockWhenCondition(SETUP_PHASE);
  mLock.unlockWithCondition(EXECUTE_PHASE);

  if(mDefaultVertexArray)
    CHECK_GL_ERROR(glDeleteVertexArrays(1, &mDefaultVertexArray));
}

bool nuRenderGL::render(void)
{
  typedef nuGContext::DrawCmd< i32 > DummyCmd;

  mLock.unlockWithCondition(EXECUTE_PHASE);

  if(mpCurrentTagList) {
    if(mpCurrentTagList->mTagNum > 0) {
      nuGContext::Tag* p_tag = mpCurrentTagList->mpTagList;

      mRenderContext.reset();
      mRenderContext.current_array_buffer = 0;
      mRenderContext.current_element_buffer = 0;
      mRenderContext.current_vertex_array = mDefaultVertexArray;

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

      nuDrawString::draw(mRenderContext);

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
  mResourceManager.updateStreamResource(mFrameID);

  nuDrawString::updateResource();

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
  if(p_draw->data.p_vertex_array && p_draw->data.p_vertex_array->getHandle() == 0)
    return;
  if(p_draw->data.p_vertex_buffer->getHandle() == 0)
    return;
  if(p_draw->data.p_element_buffer->getHandle() == 0)
    return;

  setViewport(context, *p_draw->p_viewport);
  setFragmentOps(context, p_draw->data.fragment_ops);
  setRasterizer(context, *p_draw->data.p_rasterizer);

  setShaderProgram(context, p_draw->data.program_object);

  setTexture(p_draw->data.p_texture, p_draw->data.texture_num);

  if(context.p_vertex_array != p_draw->data.p_vertex_array)
    context.p_vertex_array = p_draw->data.p_vertex_array;

  if(context.p_vertex_buffer != p_draw->data.p_vertex_buffer)
    context.p_vertex_buffer = p_draw->data.p_vertex_buffer;

  if(context.p_vertex_array) {
    if(context.p_vertex_array->getVertexHandle() != context.p_vertex_buffer->getHandle()) {
      context.p_vertex_array->reset(context.p_vertex_buffer->getHandle());
    } else {
      if(context.current_vertex_array != context.p_vertex_array->getHandle()) {
        CHECK_GL_ERROR(glBindVertexArray(context.p_vertex_array->getHandle()));
      }
      if(context.current_array_buffer != context.p_vertex_buffer->getHandle()) {
        CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, context.p_vertex_buffer->getHandle()));
      }
    }
    context.current_vertex_array = context.p_vertex_array->getHandle();
    context.current_array_buffer = context.p_vertex_buffer->getHandle();
  } else {
    ui32 count = mCurrentVertexArray < p_draw->data.immediate_num ? p_draw->data.immediate_num : mCurrentVertexArray;
    NU_ASSERT_C(count < nuVertexArray::MAX_VERTEX_ATTRIBUTE);

    if(context.current_vertex_array != mDefaultVertexArray) {
      context.current_vertex_array = mDefaultVertexArray;
      CHECK_GL_ERROR(glBindVertexArray(context.current_vertex_array));
    }

    if(context.current_array_buffer != context.p_vertex_buffer->getHandle()) {
      CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, context.p_vertex_buffer->getHandle()));
      context.current_array_buffer = context.p_vertex_buffer->getHandle();
    }

    for(ui32 ui = 0; ui < count; ui++) {
      if(ui < p_draw->data.immediate_num) {
        if(ui < mCurrentVertexArray) {
          if(mVertexArray[ui] != p_draw->data.p_immediate_array[ui]) {
            mVertexArray[ui] = p_draw->data.p_immediate_array[ui];
            nuVertexArray::Array& va = mVertexArray[ui];
            CHECK_GL_ERROR(glVertexAttribPointer(static_cast< GLuint>(ui),
                                                 static_cast< GLint >(va.size),
                                                 nuVertexArray::getAttributeType(va.type),
                                                 va.normalized ? GL_TRUE : GL_FALSE,
                                                 static_cast< GLsizei >(va.stride),
                                                 reinterpret_cast< GLvoid* >(va.offset)));
          }
        } else {
          CHECK_GL_ERROR(glEnableVertexAttribArray(static_cast< GLuint >(ui)));
          nuVertexArray::Array& va = p_draw->data.p_immediate_array[ui];
          CHECK_GL_ERROR(glVertexAttribPointer(static_cast< GLuint>(ui),
                                               static_cast< GLint >(va.size),
                                               nuVertexArray::getAttributeType(va.type),
                                               va.normalized ? GL_TRUE : GL_FALSE,
                                               static_cast< GLsizei >(va.stride),
                                               reinterpret_cast< GLvoid* >(va.offset)));
        }
      } else {
        CHECK_GL_ERROR(glDisableVertexAttribArray(static_cast< GLuint >(ui)));
      }
    }
    mCurrentVertexArray = p_draw->data.immediate_num;
  }

  if(context.p_element_buffer != p_draw->data.p_element_buffer) {
    const GLuint restart_idx[] = {
      0xffff,
      0xffffffff,
    };
    GLuint dr_idx = restart_idx[p_draw->data.p_element_buffer->getElementType()];
    if(context.restart_index != dr_idx) {
      context.restart_index = dr_idx;
      CHECK_GL_ERROR(glPrimitiveRestartIndex(context.restart_index));
    }

    context.p_element_buffer = p_draw->data.p_element_buffer;
    if(context.current_element_buffer != context.p_element_buffer->getHandle()) {
      context.current_element_buffer = context.p_element_buffer->getHandle();
      CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.current_element_buffer));
    }
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
  if(context.scissor.enable != fragment_ops.p_scissor->enable) {
    if(fragment_ops.p_scissor->enable) {
      CHECK_GL_ERROR(glEnable(GL_SCISSOR_TEST));
    } else {
      CHECK_GL_ERROR(glDisable(GL_SCISSOR_TEST));
    }
    context.scissor.enable = fragment_ops.p_scissor->enable;
  }

  if(fragment_ops.p_scissor->left != context.scissor.left ||
     fragment_ops.p_scissor->bottom != context.scissor.bottom ||
     fragment_ops.p_scissor->width != context.scissor.width ||
     fragment_ops.p_scissor->height != context.scissor.height) {
    CHECK_GL_ERROR(glScissor(fragment_ops.p_scissor->left,
                             fragment_ops.p_scissor->bottom,
                             fragment_ops.p_scissor->width,
                             fragment_ops.p_scissor->height));

    context.scissor.left = fragment_ops.p_scissor->left;
    context.scissor.bottom = fragment_ops.p_scissor->bottom;
    context.scissor.width = fragment_ops.p_scissor->width;
    context.scissor.height = fragment_ops.p_scissor->height;
  }

  if(context.depth_test.enable != fragment_ops.p_depth_test->enable) {
    if(fragment_ops.p_depth_test->enable) {
      CHECK_GL_ERROR(glEnable(GL_DEPTH_TEST));
    } else {
      CHECK_GL_ERROR(glDisable(GL_DEPTH_TEST));
    }
    context.depth_test.enable = fragment_ops.p_depth_test->enable;
  }

  if(context.depth_test.function != fragment_ops.p_depth_test->function) {
    CHECK_GL_ERROR(glDepthFunc(fragment_ops.p_depth_test->function));
    context.depth_test.function = fragment_ops.p_depth_test->function;
  }

  if(context.stencil_test.enable != fragment_ops.p_stencil_test->enable) {
    if(fragment_ops.p_stencil_test->enable) {
      CHECK_GL_ERROR(glEnable(GL_STENCIL_TEST));
    } else {
      CHECK_GL_ERROR(glDisable(GL_STENCIL_TEST));
    }
    context.stencil_test.enable = fragment_ops.p_stencil_test->enable;
  }

  if(context.stencil_test.function != fragment_ops.p_stencil_test->function ||
     context.stencil_test.reference != fragment_ops.p_stencil_test->reference ||
     context.stencil_test.mask != fragment_ops.p_stencil_test->mask) {
    CHECK_GL_ERROR(glStencilFunc(fragment_ops.p_stencil_test->function,
                                 fragment_ops.p_stencil_test->reference,
                                 fragment_ops.p_stencil_test->mask));
    context.stencil_test.function = fragment_ops.p_stencil_test->function;
    context.stencil_test.reference = fragment_ops.p_stencil_test->reference;
    context.stencil_test.mask = fragment_ops.p_stencil_test->mask;
  }

  if(context.stencil_test.stencil_fail != fragment_ops.p_stencil_test->stencil_fail ||
     context.stencil_test.depth_fail != fragment_ops.p_stencil_test->depth_fail ||
     context.stencil_test.depth_pass != fragment_ops.p_stencil_test->depth_pass) {
    CHECK_GL_ERROR(glStencilOp(fragment_ops.p_stencil_test->stencil_fail,
                               fragment_ops.p_stencil_test->depth_fail,
                               fragment_ops.p_stencil_test->depth_pass));
    context.stencil_test.stencil_fail = fragment_ops.p_stencil_test->stencil_fail;
    context.stencil_test.depth_fail = fragment_ops.p_stencil_test->depth_fail;
    context.stencil_test.depth_pass = fragment_ops.p_stencil_test->depth_pass;
  }

  if(context.blending.enable != fragment_ops.p_blending->enable) {
    if(fragment_ops.p_blending->enable) {
      CHECK_GL_ERROR(glEnable(GL_BLEND));
    } else {
      CHECK_GL_ERROR(glDisable(GL_BLEND));
    }
    context.blending.enable = fragment_ops.p_blending->enable;
  }

  if(context.blending.equation != fragment_ops.p_blending->equation) {
    CHECK_GL_ERROR(glBlendEquation(fragment_ops.p_blending->equation));
    context.blending.equation = fragment_ops.p_blending->equation;
  }

  if(context.blending.source != fragment_ops.p_blending->source ||
     context.blending.destination != fragment_ops.p_blending->destination) {
    CHECK_GL_ERROR(glBlendFunc(fragment_ops.p_blending->source,
                               fragment_ops.p_blending->destination));
    context.blending.source = fragment_ops.p_blending->source;
    context.blending.destination = fragment_ops.p_blending->destination;
  }

  if(context.blending.color != fragment_ops.p_blending->color) {
    nuColor color(fragment_ops.p_blending->color);
    CHECK_GL_ERROR(glBlendColor(color.fr(), color.fg(), color.fb(), color.fa()));
    context.blending.color = fragment_ops.p_blending->color;
  }
}

void nuRenderGL::setRasterizer(RenderContext& context, nuGContext::Rasterizer& rasterizer)
{
  if(context.rasterizer.line_smooth != rasterizer.line_smooth) {
    context.rasterizer.line_smooth = rasterizer.line_smooth;
    if(context.rasterizer.line_smooth) {
      CHECK_GL_ERROR(glEnable(GL_LINE_SMOOTH));
    } else {
      CHECK_GL_ERROR(glDisable(GL_LINE_SMOOTH));
    }
  }

  if(context.rasterizer.line_width != rasterizer.line_width) {
    context.rasterizer.line_width = rasterizer.line_width;
    CHECK_GL_ERROR(glLineWidth(context.rasterizer.line_width));
  }

  if(context.rasterizer.culling != rasterizer.culling) {
    context.rasterizer.culling = rasterizer.culling;
    if(context.rasterizer.culling) {
      CHECK_GL_ERROR(glEnable(GL_CULL_FACE));
    } else {
      CHECK_GL_ERROR(glDisable(GL_CULL_FACE));
    }
  }

  if(context.rasterizer.cull_face != rasterizer.cull_face) {
    context.rasterizer.cull_face = rasterizer.cull_face;
    CHECK_GL_ERROR(glCullFace(context.rasterizer.cull_face));
  }

  if(context.rasterizer.front_face != rasterizer.front_face) {
    context.rasterizer.front_face = rasterizer.front_face;
    CHECK_GL_ERROR(glFrontFace(context.rasterizer.front_face));
  }
}

void nuRenderGL::setTexture(nuGContext::TextureEntity* p_textures, ui32 tex_num)
{
  for(ui32 ui = 0; ui < tex_num; ui++) {
    nuGContext::TextureEntity& texture = p_textures[ui];
    CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0 + ui));
    CHECK_GL_ERROR(glBindTexture(static_cast< GLenum >(texture.p_texture->mTextureType),
                                 texture.p_texture->mHandle));
    if(texture.p_parameter) {
      if(texture.p_texture->mWrapS != texture.p_parameter->wrap_s) {
        texture.p_texture->mWrapS = texture.p_parameter->wrap_s;
        CHECK_GL_ERROR(glTexParameteri(static_cast< GLenum >(texture.p_texture->mTextureType),
                                       GL_TEXTURE_WRAP_S,
                                       texture.p_texture->mWrapS));
      }

      if(texture.p_texture->mWrapT != texture.p_parameter->wrap_t) {
        texture.p_texture->mWrapT = texture.p_parameter->wrap_t;
        CHECK_GL_ERROR(glTexParameteri(static_cast< GLenum >(texture.p_texture->mTextureType),
                                       GL_TEXTURE_WRAP_T,
                                       texture.p_texture->mWrapT));
      }

      if(texture.p_texture->mWrapR != texture.p_parameter->wrap_r) {
        texture.p_texture->mWrapR = texture.p_parameter->wrap_r;
        CHECK_GL_ERROR(glTexParameteri(static_cast< GLenum >(texture.p_texture->mTextureType),
                                       GL_TEXTURE_WRAP_R,
                                       texture.p_texture->mWrapR));
      }

      if(texture.p_texture->mMinFilter != texture.p_parameter->min_filter) {
        texture.p_texture->mMinFilter = texture.p_parameter->min_filter;
        CHECK_GL_ERROR(glTexParameteri(static_cast< GLenum >(texture.p_texture->mTextureType),
                                       GL_TEXTURE_MIN_FILTER,
                                       texture.p_texture->mMinFilter));
      }

      if(texture.p_texture->mMagFilter != texture.p_parameter->mag_filter) {
        texture.p_texture->mMagFilter = texture.p_parameter->mag_filter;
        CHECK_GL_ERROR(glTexParameteri(static_cast< GLenum >(texture.p_texture->mTextureType),
                                       GL_TEXTURE_MAG_FILTER,
                                       texture.p_texture->mMagFilter));
      }
    }
  }
}

void nuRenderGL::initializeStates(void)
{
  mRenderContext.reset();

  // Initialize render context.
  CHECK_GL_ERROR(glClearColor(mRenderContext.clear_color.fr(),
                              mRenderContext.clear_color.fg(),
                              mRenderContext.clear_color.fb(),
                              mRenderContext.clear_color.fa()));

  CHECK_GL_ERROR(glClearDepth(mRenderContext.depth_value));

  CHECK_GL_ERROR(glViewport(mRenderContext.viewport.origin_x,
                            mRenderContext.viewport.origin_y,
                            mRenderContext.viewport.width,
                            mRenderContext.viewport.height));

  if(mRenderContext.scissor.enable) {
    CHECK_GL_ERROR(glEnable(GL_SCISSOR_TEST));
  } else {
    CHECK_GL_ERROR(glDisable(GL_SCISSOR_TEST));
  }
  CHECK_GL_ERROR(glScissor(mRenderContext.scissor.left,
                           mRenderContext.scissor.bottom,
                           mRenderContext.scissor.width,
                           mRenderContext.scissor.height));

  if(mRenderContext.depth_test.enable) {
    CHECK_GL_ERROR(glEnable(GL_DEPTH_TEST));
  } else {
    CHECK_GL_ERROR(glDisable(GL_DEPTH_TEST));
  }
  CHECK_GL_ERROR(glDepthFunc(mRenderContext.depth_test.function));

  if(mRenderContext.stencil_test.enable) {
    CHECK_GL_ERROR(glEnable(GL_STENCIL_TEST));
  } else {
    CHECK_GL_ERROR(glDisable(GL_STENCIL_TEST));
  }
  CHECK_GL_ERROR(glStencilFunc(mRenderContext.stencil_test.function,
                               mRenderContext.stencil_test.reference,
                               mRenderContext.stencil_test.mask));
  CHECK_GL_ERROR(glStencilOp(mRenderContext.stencil_test.stencil_fail,
                             mRenderContext.stencil_test.depth_fail,
                             mRenderContext.stencil_test.depth_pass));

  if(mRenderContext.blending.enable) {
    CHECK_GL_ERROR(glEnable(GL_BLEND));
  } else {
    CHECK_GL_ERROR(glDisable(GL_BLEND));
  }
  CHECK_GL_ERROR(glBlendEquation(mRenderContext.blending.equation));
  CHECK_GL_ERROR(glBlendFunc(mRenderContext.blending.source, mRenderContext.blending.destination));
  nuColor color(mRenderContext.blending.color);
  CHECK_GL_ERROR(glBlendColor(color.fr(), color.fg(), color.fb(), color.fa()));

  if(mRenderContext.rasterizer.culling) {
    CHECK_GL_ERROR(glEnable(GL_CULL_FACE));
  } else {
    CHECK_GL_ERROR(glDisable(GL_CULL_FACE));
  }

  mRenderContext.rasterizer.line_smooth = false;
  if(mRenderContext.rasterizer.line_smooth) {
    CHECK_GL_ERROR(glEnable(GL_LINE_SMOOTH));
  } else {
    CHECK_GL_ERROR(glDisable(GL_LINE_SMOOTH));
  }

  mRenderContext.rasterizer.line_width = 1.0f;
  CHECK_GL_ERROR(glLineWidth(mRenderContext.rasterizer.line_width));
  mRenderContext.rasterizer.cull_face = nude::CULL_BACK;
  CHECK_GL_ERROR(glCullFace(mRenderContext.rasterizer.cull_face));
  mRenderContext.rasterizer.front_face = nude::FRONT_COUNTER_CLOCKWISE;
  CHECK_GL_ERROR(glFrontFace(mRenderContext.rasterizer.front_face));
}
