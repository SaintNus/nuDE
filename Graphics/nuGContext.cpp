/*!
 * \file nuGContext.cpp
 * \brief Graphic context.
 * \author Nus
 * \date 2011/10/30 21:49
 */

#include "nuGContext.h"

nuGContext::nuGContext(nuGContextBuffer& ctx_buffer)
    : mFrameID(0),
      mBuffer(ctx_buffer)
{
  // None...
}

nuGContext::~nuGContext()
{
  // None...
}

void nuGContext::clear(ui32 clear_bit, const nuColor& color, f32 depth)
{

}
