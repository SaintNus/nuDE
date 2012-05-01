/*!
 * \file nuGSetupContext.cpp
 * \brief Graphic setup context.
 * \author Nus
 * \date 2012/05/01 20:32
 */

#include "nuGSetupContext.h"

nuGSetupContext::nuGSetupContext()
    : mFrameID(0)
{

}

nuGSetupContext::~nuGSetupContext()
{

}

void nuGSetupContext::beginSetup(i64 frame_id)
{
  mFrameID = frame_id;
}

void nuGSetupContext::endSetup(void)
{

}
