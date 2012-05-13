/*!
 * \file nuGResManager.cpp
 * \brief GL resource manager.
 * \author Nus
 * \date 2012/02/05 19:09
 */

#include "GraphicsInclude.h"

#include "nuGResManager.h"
#include "nuVertexBuffer.h"
#include "nuElementBuffer.h"

using namespace nude;

IMPLEMENT_TYPE_INFO(nuGResManager, nuObject);

nuGResManager::nuGResManager()
    : mpUpdateTable(nullptr),
      mUpdateTableNum(EXPANDABLE_TABLE_NUM)
{
  nuShaderList shd;

  mpUpdateTable = new nuGResource* [mUpdateTableNum];
  NU_ASSERT(mpUpdateTable != nullptr, "Cannot create update table.\n");
}

nuGResManager::~nuGResManager()
{
  deleteResources(mStaticResource, mStaticResMutex, 0x7fffffffffffffffL);
  deleteResources(mDynamicResource, mDynamicResMutex, 0x7fffffffffffffffL);

  NU_ASSERT(mStaticResource.size() == 0, "Static GL resource is leaking.\n");
  NU_ASSERT(mDynamicResource.size() == 0, "Dynamic GL resource is leaking.\n");

  if(mpUpdateTable) {
    delete[] mpUpdateTable;
    mpUpdateTable = nullptr;
  }
}

VertexArray nuGResManager::createVertexArray(void)
{
  nuVertexArray* p_va = new nuVertexArray();
  registerResource(*p_va);
  return VertexArray(p_va);
}

VertexBuffer nuGResManager::createVertexBuffer(size_t size, nuGResource::RESOURCE_USAGE usage)
{
  nuVertexBuffer* p_vb = new nuVertexBuffer(size, usage);
  registerResource(*p_vb);
  return VertexBuffer(p_vb);
}

ElementBuffer nuGResManager::createElementBuffer(nuElementBuffer::ELEMENT_TYPE type,
                                                 ui32 size,
                                                 nuGResource::RESOURCE_USAGE usage)
{
  nuElementBuffer* p_eb = new nuElementBuffer(type, size, usage);
  registerResource(*p_eb);
  return ElementBuffer(p_eb);
}

void nuGResManager::updateStaticResource(i64 frame_id)
{
  deleteResources(mStaticResource, mStaticResMutex, frame_id);
  updateResources(mStaticResource, mStaticResMutex);
}

void nuGResManager::updateDynamicResource(i64 frame_id)
{
  deleteResources(mDynamicResource, mDynamicResMutex, frame_id);
  updateResources(mDynamicResource, mDynamicResMutex);
}

void nuGResManager::deleteResources(ResList& resource_list, nuMutex& mutex, i64 frame_id)
{
  nuMutex::Autolock auto_mutex(mutex);
  ResListIterator it = resource_list.begin();
  while(it != resource_list.end()) {
    nuGResource* p_res = *it;
    if(p_res->mRefCount == 1 && p_res->mFrameID < frame_id) {
      bool deleted = p_res->decRefCount();
      if(deleted)
        it = resource_list.erase(it);
    } else {
      ++it;
    }
  }
}

void nuGResManager::updateResources(ResList& resource_list, nuMutex& mutex)
{
  if(resource_list.size() > mUpdateTableNum) {
    ui32 size = static_cast< ui32 >(resource_list.size()) / EXPANDABLE_TABLE_NUM;

    size++;
    size *= EXPANDABLE_TABLE_NUM;
    mUpdateTableNum = size;
    
    delete[] mpUpdateTable;
    mpUpdateTable = new nuGResource* [mUpdateTableNum];

    NU_ASSERT(mpUpdateTable != nullptr, "Cannot create update table.\n");
  }

  ui32 num = 0;

  {
    nuMutex::Autolock auto_mutex(mutex);
    ResListConstIterator it;
    for(it = resource_list.begin(); it != resource_list.end(); ++it) {
      if((*it)->isUpdated()) {
        mpUpdateTable[num] = *it;
        num++;
      }
    }
  }

  for(ui32 ui = 0; ui < num; ui++) {
    mpUpdateTable[ui]->update();
    mpUpdateTable[ui]->setUpdate(false);
  }
}

void nuGResManager::initializeShaderList(nuResourceManager& resource_mgr, ccstr shader_list)
{
  if(mShaderList.isValid())
    return;

  nuResHandle res = resource_mgr.createResource(shader_list);
  mShaderList = res.cast< nuShaderList >();
}
