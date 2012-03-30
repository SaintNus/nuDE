/*!
 * \file nuGResManager.h
 * \brief GL resource manager.
 * \author Nus
 * \date 2012/02/05 19:09
 */

#ifndef __NUGRESMANAGER_H__
#define __NUGRESMANAGER_H__

#include "nuGResource.h"

class nuGResManager : public nuObject
{
  DECLARE_TYPE_INFO;

  typedef std::list< nuGResource* > ResList;
  typedef ResList::iterator ResListIterator;
  typedef ResList::const_iterator ResListConstIterator;

  nuMutex mStaticResMutex;
  ResList mStaticResource;

  nuMutex mDynamicResMutex;
  ResList mDynamicResource;

public:
  nuGResManager();
  ~nuGResManager();

  nuGResHandle createVertexBuffer(size_t size, nuGResource::RESOURCE_USAGE usage);
  nuGResHandle createElementBuffer(size_t size, nuGResource::RESOURCE_USAGE usage);

  void updateStaticResource(void);
  void updateDynamicResource(void);

};

#endif
