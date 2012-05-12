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

}

nuProgramManager::ShaderList::~ShaderList()
{

}

nuResource::ERROR_CODE nuProgramManager::ShaderList::setup(const nuStream &stream)
{
  return ERROR_NONE;
}

nuProgramManager::nuProgramManager()
{

}

nuProgramManager::~nuProgramManager()
{

}
