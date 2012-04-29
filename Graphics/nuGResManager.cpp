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
    : mpUpdateTable(nullptr),
      mUpdateTableNum(EXPANDABLE_TABLE_NUM)
{
  mpUpdateTable = new nuGResource* [mUpdateTableNum];
  NU_ASSERT(mpUpdateTable != nullptr, "Cannot create update table.\n");
}

nuGResManager::~nuGResManager()
{
  NU_ASSERT(mStaticResource.size() == 0, "GL static resource is leaking.\n");
  NU_ASSERT(mDynamicResource.size() == 0, "GL dynamic resource is leaking.\n");

  if(mpUpdateTable) {
    delete[] mpUpdateTable;
    mpUpdateTable = nullptr;
  }
}

nuGResHandle nuGResManager::createVertexBuffer(size_t size, nuGResource::RESOURCE_USAGE usage)
{
  return nuGResHandle(new nuVertexBuffer(0, usage));
}

nuGResHandle nuGResManager::createElementBuffer(size_t size, nuGResource::RESOURCE_USAGE usage)
{
  return nuGResHandle(new nuElementBuffer(nuElementBuffer::UNSIGNED_INT_16, 0, usage));
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
  }
}
