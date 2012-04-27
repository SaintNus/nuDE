/*!
 * \file nuGResManager.cpp
 * \brief GL resource manager.
 * \author Nus
 * \date 2012/02/05 19:09
 */

#include "nuGResManager.h"
#include "nuVertexBuffer.h"
#include "nuElementBuffer.h"

IMPLEMENT_TYPE_INFO(nuGResManager, nuObject);

nuGResManager::nuGResManager()
{

}

nuGResManager::~nuGResManager()
{

}

nuGResHandle nuGResManager::createVertexBuffer(size_t size, nuGResource::RESOURCE_USAGE usage)
{
  return nuGResHandle(new nuVertexBuffer(0, usage));
}

nuGResHandle nuGResManager::createElementBuffer(size_t size, nuGResource::RESOURCE_USAGE usage)
{
  return nuGResHandle(new nuElementBuffer(nuElementBuffer::UNSIGNED_INT_16, 0, usage));
}

void nuGResManager::updateStaticResource(void)
{

}

void nuGResManager::updateDynamicResource(void)
{

}
