/*!
 * \file nuEntityManager.h
 * \brief Entity manager.
 * \author Nus
 * \date 2012/05/01 19:30
 */

#ifndef __NUENTITYMANAGER_H__
#define __NUENTITYMANAGER_H__

class nuEntityManager : public nuObject
{
  DECLARE_TYPE_INFO;
  friend class nuEntity;

  union {
    ui32 mAttribute;
    struct {
      ui32 mExited: 1;
      ui32 mReserved: 31;
    };
  };

  nuMutex mListMutex;
  class nuEntity* mpList;
  ui32 mEntityNum;

  void registerEntity(class nuEntity& entity);
  void unregisterEntity(class nuEntity& entity);

public:
  nuEntityManager();
  ~nuEntityManager();

  ui32 getEntityNum(void) const {
    return mEntityNum;
  }

  void setupEntity(i64 frame_id);

};

#endif
