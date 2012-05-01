/*!
 * \file nuEntity.cpp
 * \brief Entity class.
 * \author Nus
 * \date 2012/05/01 19:26
 */

#include "nuApplication.h"
#include "nuEntityManager.h"
#include "nuEntity.h"

IMPLEMENT_TYPE_INFO_ABSTRACT(nuEntity, nuObject);

nuEntity::nuEntity()
    : mAttribute(0),
      mpPrev(nullptr),
      mpNext(nullptr)
{
  nuApplication::entityManager().registerEntity(*this);
}

nuEntity::~nuEntity()
{
  nuApplication::entityManager().unregisterEntity(*this);
}
