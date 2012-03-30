/*!
 * \file nuResourceManager.h
 * \brief Resource manager.
 * \author Nus
 * \date 2012/02/04 22:49
 */

#ifndef __NURESOURCEMANAGER_H__
#define __NURESOURCEMANAGER_H__

#include "nuObject.h"
#include "nuSingleton.h"

class nuResourceManager : public nuObject
{
  DECLARE_TYPE_INFO;
  DECLARE_SINGLETON(nuResourceManager);

private:
  nuResourceManager() {}
  ~nuResourceManager() {}

};

#endif
