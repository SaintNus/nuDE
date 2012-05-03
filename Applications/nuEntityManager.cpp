/*!
 * \file nuEntityManager.cpp
 * \brief Entity manager.
 * \author Nus
 * \date 2012/05/01 19:30
 */

#include "nuEntity.h"
#include "nuEntityManager.h"

nuEntityManager::nuEntityManager()
    : mpList(nullptr),
      mEntityNum(0),
      mAttribute(0)
{
  // None...
}

nuEntityManager::~nuEntityManager()
{
  NU_ASSERT(mEntityNum == 0, "Leaking entity object.\n");

  nuEntity* ptr = mpList;
  mExited = 1;

  while(ptr) {
    nuEntity* p_next = ptr->mpNext;
    delete ptr;
    ptr = p_next;
  }
}

void nuEntityManager::registerEntity(nuEntity& entity)
{
  if(mExited)
    return;

  NU_ASSERT(entity.mRegistered == 0, "Entity has already registered.\n");

  nuMutex::Autolock lock(mListMutex);

  entity.mpNext = mpList;
  if(mpList)
    mpList->mpPrev = &entity;
  mpList = &entity;
  mEntityNum++;

  entity.mRegistered = 1;
}

void nuEntityManager::unregisterEntity(nuEntity& entity)
{
  if(mExited)
    return;

  NU_ASSERT(entity.mRegistered == 1, "Entity is not registered.\n");

  nuMutex::Autolock lock(mListMutex);

  if(mpList == &entity) {
    mpList = entity.mpNext;
    if(entity.mpNext)
      entity.mpNext->mpPrev = nullptr;
  } else {
    if(entity.mpNext)
      entity.mpNext->mpPrev = entity.mpPrev;
    if(entity.mpPrev)
      entity.mpPrev->mpNext = entity.mpNext;
  }

  mEntityNum--;

  entity.mRegistered = 0;
}

void nuEntityManager::setupEntity(i64 frame_id)
{
  nuGSetupContext setup_ctx;
  setup_ctx.beginSetup(frame_id + 1);
  {
    nuMutex::Autolock lock(mListMutex);
    nuEntity* ptr = mpList;

    while(ptr) {
      ptr->setup(setup_ctx);
      ptr = ptr->mpNext;
    }
  }
  setup_ctx.endSetup();
}
