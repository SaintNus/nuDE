/*!
 * \file nuResourceManager.h
 * \brief Resource manager.
 * \author Nus
 * \date 2012/02/04 22:49
 */

#ifndef __NURESOURCEMANAGER_H__
#define __NURESOURCEMANAGER_H__

#include "nuResource.h"

class nuResourceManager : public nuObject
{
  DECLARE_TYPE_INFO;
  friend class nuResource;

  enum CONDITION {
    EMPTY = 0,
    HAS_DATA,
  };

  enum LOADER_STATE {
    IDLE = 0,
    WAIT,
    BUSY,
  };

  typedef std::map< ui32, nuResource* > ResourceMap;
  typedef ResourceMap::iterator ResourceMapIterator;
  typedef ResourceMap::const_iterator ResourceMapConstIterator;

  typedef std::vector< nuResource* > ResourceList;
  typedef ResourceList::iterator ResourceListIterator;
  typedef ResourceList::const_iterator ResourceListConstIterator;

  typedef std::map< ui32, const nuTypeInfo* > TypeInfoMap;
  typedef TypeInfoMap::iterator TypeInfoMapIterator;
  typedef TypeInfoMap::const_iterator TypeInfoMapConstIterator;

  ResourceList mResourceList;
  nuMutex mResourceListMutex;

  ResourceMap mResourceMap;
  nuMutex mResourceMapMutex;

  nuThread mResourceLoader;
  nuCondition mDataCondition;

  volatile i32 mDataStatus;
  volatile i32 mLoadTaskNum;

  TypeInfoMap mTypeInfoMap;

  union {
    ui32 mAttribute;
    struct {
      ui32 mState: 3;
      ui32 mExit: 1;
      ui32 mReserved: 28;
    };
  };

  void resourceLoaderProc(void* param);

  void registerResource(nuResource* p_res);
  void unregisterResource(nuResource* p_res);

  void registerLoadList(nuResource* p_res);

  void setDataStatus(CONDITION condition) {
    i32 curr = mDataStatus;
    i32 res = condition;
    while(curr != res) {
      bool ret = nuAtomic::casBarrier(curr, res, &mDataStatus);
      if(ret) {
        if(res == HAS_DATA && mState == WAIT) {
          mDataCondition.lock();
          mDataCondition.signal();
          mDataCondition.unlock();
        }
      } else {
        curr = mDataStatus;
      }
    }
  }

  void exit(void) {
    mExit = 1;
    while(mState != IDLE) {
      mDataCondition.signal();
      nuThread::usleep(1000);
    }
  }

  void buildResourceTable(const nuTypeInfo& type_info);

public:
  nuResourceManager();
  ~nuResourceManager();

  nuResHandle createResource(ccstr resource_path);
  nuResHandle createResourceSync(ccstr resource_path);

};

#endif
