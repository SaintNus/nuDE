/*!
 * \file nuTypeInfo.cpp
 * \brief Type information.
 * \author Nus
 * \date 2010/04/15 23:40
 */

#include "nuDebug.h"
#include "nuCRC.h"
#include "nuTypeInfo.h"



//! \brief Manager.
nuTypeInfo::TypeInfoMgr nuTypeInfo::mManager;



// Constructor.
nuTypeInfo::nuTypeInfo(ccstr class_name, nuTypeInfo* type_info)
    : mName(class_name)
{
  mID = nuCRC::calculate(class_name);
  if(type_info) {
    nuTypeInfo* temp = type_info->mpSubClass;
    type_info->mpSubClass = this;
    mpNextClass = temp;
    mpSuperClass = type_info;
  }
  registerTypeInfo();
}



// Search for type info.
const nuTypeInfo* nuTypeInfo::TypeInfoMgr::searchByName(ccstr class_name)
{
  ui32 c_id = nuCRC::calculate(class_name);
  ui32 h_addr = hashAddr(c_id);
  nuTypeInfo* ret = NULL;
  nuTypeInfo* p_curr = mTable[h_addr];
  while(p_curr) {
    if(p_curr->mID == c_id) {
      ret = p_curr;
      break;
    }
    p_curr = p_curr->mpNextTypeInfo;
  }
  return ret;
}

// Register type info.
void nuTypeInfo::TypeInfoMgr::registerTypeInfo(nuTypeInfo* type_info)
{
  ui32 h_addr = hashAddr(type_info->mID);
  nuTypeInfo* p_curr = mTable[h_addr];

  if(type_info->mpSuperClass == NULL) {
    // Add to root.
    nuTypeInfo* temp = mpRoot;
    type_info->mpNextClass = temp;
    mpRoot = type_info;
  }

  // Register to hash table.
  while(p_curr) {
    NU_ASSERT(p_curr->mID != type_info->mID, "Critical: Class has same CRC!\n");
    if(p_curr->mpNextTypeInfo == NULL) {
      p_curr->mpNextTypeInfo = type_info;
      return;
    }
    p_curr = p_curr->mpNextTypeInfo;
  }
  mTable[h_addr] = type_info;
}

#if !NDEBUG

// Print all type infos.
void nuTypeInfo::listAll(void)
{
  if(!mManager.getRoot())
    return;
  NU_TRACE("Listing type info...\n");
  printTree(mManager.getRoot(), 0);
}

// Print all type infos.
void nuTypeInfo::printTree(const nuTypeInfo* type_info, ui32 lv)
{
  if(lv == 0)
    NU_TRACE("Root:");
  for(ui32 ui = 0; ui < lv; ui++)
    NU_TRACE("-");
  NU_TRACE(" %s\n", type_info->getName());
  if(type_info->mpSubClass)
    printTree(type_info->mpSubClass, lv + 1);
  if(type_info->mpNextClass)
    printTree(type_info->mpNextClass, lv);
}

#endif

// Is derived from.
bool nuTypeInfo::isDerivedFrom(ccstr parent_name) const
{
  ui32 id = nuCRC::calculate(parent_name);
  return internalIsDerivedFrom(id);
}

// Is derived from.
bool nuTypeInfo::isDerivedFrom(const nuTypeInfo& parent) const
{
  return internalIsDerivedFrom(parent.mID);
}
