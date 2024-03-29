/*!
 * \file nuGSetupContext.cpp
 * \brief Graphic setup context.
 * \author Nus
 * \date 2012/05/01 20:32
 */

#include "GraphicsInclude.h"
#include "nuVertexBuffer.h"
#include "nuElementBuffer.h"
#include "nuShaderProgram.h"
#include "nuUniformBuffer.h"
#include "nuGSetupContext.h"

nuGSetupContext::nuGSetupContext(i64 frame_id, const nuRect& rect, bool resize)
    : mFrameID(frame_id),
      mViewport(rect),
      mResizeView(resize)
{
  // None...
}

nuGSetupContext::~nuGSetupContext()
{
  // None...
}
