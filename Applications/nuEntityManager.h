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

  void updateEntity(void* param);

public:
  class EntityTable {
    friend class nuEntityManager;
    friend class Iterator;

    static const ui32 EXPAND_ENTRY_NUM = 1024;

    ui32 mCapacity;
    ui32 mEntryNum;
    nuEntity** mpEntityTable;

    void reserve(ui32 entity_num) {
      if(mCapacity < entity_num) {
        ui32 num = entity_num / EntityTable::EXPAND_ENTRY_NUM;
        num = (num + 1) * EntityTable::EXPAND_ENTRY_NUM;
        if(mpEntityTable) {
          delete[] mpEntityTable;
          mpEntityTable = nullptr;
          mCapacity = 0;
        }
        mpEntityTable = new nuEntity* [num];
        if(mpEntityTable) {
          mCapacity = num;
        }
      }
      mEntryNum = 0;
    }

  public:
    class Iterator {
      volatile i32 mCurrentIndex;
      const EntityTable* mpEntityTable;

    public:
      Iterator()
          : mCurrentIndex(0),
            mpEntityTable(nullptr)
      {
        // None...
      }
      ~Iterator() {}

      ui32 initialize(const EntityTable& entity_table) {
        mCurrentIndex = 0;
        mpEntityTable = &entity_table;
        return mpEntityTable->getEntryNum();
      }

      ui32 get(nuEntity*** p_entity, ui32 num) {
        i32 curr = mCurrentIndex;
        i32 entry_num = num;
        i32 res = curr + entry_num;

        if(res > static_cast< i32 >(mpEntityTable->getEntryNum())) {
          entry_num = mpEntityTable->getEntryNum() - curr;
          res = curr + entry_num;
        }

        while(curr < static_cast< i32 >(mpEntityTable->getEntryNum())) {
          bool ret = nuAtomic::casBarrier(curr, res, &mCurrentIndex);
          if(!ret) {
            curr = mCurrentIndex;
            entry_num = num;
            res = curr + entry_num;
            if(res > static_cast< i32 >(mpEntityTable->getEntryNum())) {
              entry_num = mpEntityTable->getEntryNum() - curr;
              res = curr + entry_num;
            }
          } else {
            *p_entity = &(mpEntityTable->mpEntityTable[curr]);
            return entry_num;
          }
        }

        return 0;
      }
    };

    EntityTable()
        : mCapacity(0),
          mEntryNum(0),
          mpEntityTable(nullptr)
    {
      // None...
    }

    ~EntityTable() {
      if(mpEntityTable) {
        delete[] mpEntityTable;
        mpEntityTable = nullptr;
      }
    }

    ui32 getEntryNum(void) const {
      return mEntryNum;
    }

    nuEntity* operator [] (ui32 index) {
      if(index < mEntryNum)
        return mpEntityTable[index];
      return nullptr;
    }

  };

  nuEntityManager();
  ~nuEntityManager();

  ui32 getEntityNum(void) const {
    return mEntityNum;
  }

  void setupEntity(i64 frame_id);
  void createUpdateList(nuTaskSet& update_set, EntityTable& table);
  void drawEntity(nuGContext& context, nuEntity& entity);

};

#endif
