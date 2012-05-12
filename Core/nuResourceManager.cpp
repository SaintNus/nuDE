/*!
 * \file nuResourceManager.cpp
 * \brief Resource manager.
 * \author Nus
 * \date 2012/02/04 22:49
 */

#include "nuCRC.h"
#include "nuResourceManager.h"

IMPLEMENT_TYPE_INFO(nuResourceManager, nuObject);

nuResourceManager::nuResourceManager()
    : mAttribute(0),
      mDataStatus(EMPTY),
      mLoadTaskNum(0)
{
  if(nuResource::TypeInfo().getChild())
    buildResourceTable(*nuResource::TypeInfo().getChild());

  mResourceList.reserve(256);

  nuFunction func = static_cast< nuFunction >(&nuResourceManager::resourceLoaderProc);
  mResourceLoader.setName("Loader thread");
  mResourceLoader.dispatchThread(this, func, nullptr);

  while(mState == IDLE)
    nuThread::usleep(1000);
}

nuResourceManager::~nuResourceManager()
{
  exit();
  mResourceLoader.join();

  {
    ResourceMapIterator it;
    for(it = mResourceMap.begin(); it != mResourceMap.end(); ++it) {
      nuResource* p_res = it->second;
      NU_TRACE("Resource leak: %s\n", p_res->mPath);
      delete p_res;
      it->second = nullptr;
    }
    NU_ASSERT(mResourceMap.size() == 0, "Resource is leaking.\n");
    mResourceMap.clear();
  }
}

void nuResourceManager::resourceLoaderProc(void* param)
{
  const ui32 expand_list = 256;
  ui32 list_num = 0;
  nuResource** load_list = nullptr;

  NU_TRACE("Resource loader started.\n");
  mDataCondition.lock();

  while(!mExit) {
    while(mDataStatus == EMPTY && mLoadTaskNum == 0) {
      mState = WAIT;
      mDataCondition.wait();
      if(mExit)
        break;
    }

    if(mExit)
      break;

    mState = BUSY;

    ui32 num;
    {
      nuMutex::Autolock lock(mResourceListMutex);

      num = static_cast< ui32 >(mResourceList.size());
      if(num > list_num) {
        if(load_list)
          nude::Dealloc(load_list);
        ui32 exp_num = ((num / expand_list) + 1) * expand_list;
        load_list = static_cast< nuResource** >(nude::Alloc(sizeof(nuResource*) * exp_num));
        NU_ASSERT_C(load_list);
      }

      for(ui32 ui = 0; ui < num; ui++) {
        load_list[ui] = mResourceList[ui];
      }

      mResourceList.clear();
    }

    for(ui32 ui = 0; ui < num; ui++) {
      load(*load_list[ui]);
      load_list[ui]->decRefCount();
      nuAtomic::decBarrier(&mLoadTaskNum);
    }

    if(mLoadTaskNum == 0)
      setDataStatus(EMPTY);
  }

  mDataCondition.unlock();
  mState = IDLE;
  NU_TRACE("Resource loader exited.\n");
}

void nuResourceManager::registerResource(nuResource* p_res)
{
  NU_ASSERT(p_res->mCRC == 0, "Invalid CRC.\n");

  mResourceMap.insert(std::pair< ui32, nuResource* >(p_res->mCRC, p_res));
}

void nuResourceManager::unregisterResource(nuResource* p_res)
{
  if(mExit)
    return;

  ui32 crc = p_res->mCRC;
  NU_ASSERT(crc != 0, "Invalid CRC.\n");

  {
    nuMutex::Autolock lock(mResourceMapMutex);
    mResourceMap.erase(crc);
  }
}

void nuResourceManager::registerLoadList(nuResource* p_res)
{
  nuMutex::Autolock lock(mResourceListMutex);
  mResourceList.push_back(p_res->incRefCount());
  nuAtomic::inc(&mLoadTaskNum);
  setDataStatus(HAS_DATA);
}

nuResHandle nuResourceManager::createResource(ccstr resource_path)
{
  ui32 crc = nuCRC::calculate(resource_path);
  nuResource* p_res = nullptr;

  {
    nuMutex::Autolock lock(mResourceMapMutex);
    ResourceMap::iterator it = mResourceMap.find(crc);
    if(it != mResourceMap.end())
      return nuResHandle(it->second);

    ccstr ptr = strrchr(resource_path, '.');
    if(ptr) {
      ptr++;
      ui32 ext_crc = nuCRC::calculate(ptr);
      TypeInfoMapConstIterator itt = mTypeInfoMap.find(ext_crc);
      if(itt != mTypeInfoMap.end()) {
        p_res = static_cast< nuResource* >(itt->second->createInstance());
        if(p_res) {
          p_res->setPath(resource_path, crc);
          p_res->mpResourceManager = this;
#if NDEBUG || !DEBUG
          mResourceMap.insert(std::pair< ui32, nuResource* >(crc, p_res));
#else
          std::pair< ResourceMapIterator, bool > res;
          res = mResourceMap.insert(std::pair< ui32, nuResource* >(crc, p_res));
          NU_ASSERT(res.second, "Cannot register resource!\n");
#endif
        }
      }
    }
  }

  if(p_res) {
    nuResHandle ret(p_res);
    registerLoadList(p_res);
    return ret;
  }

  NU_ASSERT(false, "Cannot create resource (%s).\n", resource_path);
  return nuResHandle();
}

nuResHandle nuResourceManager::createResourceSync(ccstr resource_path)
{
  ui32 crc = nuCRC::calculate(resource_path);
  nuResource* p_res = nullptr;

  {
    nuMutex::Autolock lock(mResourceMapMutex);
    ResourceMap::iterator it = mResourceMap.find(crc);
    if(it != mResourceMap.end())
      return nuResHandle(it->second);

    ccstr ptr = strrchr(resource_path, '.');
    if(ptr) {
      ptr++;
      ui32 ext_crc = nuCRC::calculate(ptr);
      TypeInfoMapConstIterator itt = mTypeInfoMap.find(ext_crc);
      if(itt != mTypeInfoMap.end()) {
        p_res = static_cast< nuResource* >(itt->second->createInstance());
        if(p_res) {
          p_res->setPath(resource_path, crc);
          p_res->mpResourceManager = this;
#if NDEBUG || !DEBUG
          mResourceMap.insert(std::pair< ui32, nuResource* >(crc, p_res));
#else
          std::pair< ResourceMapIterator, bool > res;
          res = mResourceMap.insert(std::pair< ui32, nuResource* >(crc, p_res));
          NU_ASSERT(res.second, "Cannot register resource!\n");
#endif
        }
      }
    }
  }

  if(p_res) {
    nuResHandle ret(p_res);
    load(*p_res);
    return ret;
  }

  NU_ASSERT(false, "Cannot create resource (%s).\n", resource_path);
  return nuResHandle();
}

void nuResourceManager::buildResourceTable(const nuTypeInfo& type_info)
{
  nuResource* p_res = static_cast< nuResource* >(type_info.createInstance());
  if(p_res) {
    ui32 crc = nuCRC::calculate(p_res->getExt());
#if NDEBUG || !DEBUG
    mTypeInfoMap.insert(std::pair< ui32, const nuTypeInfo* >(crc, &type_info));
#else
    std::pair< TypeInfoMapIterator, bool > ret;
    ret = mTypeInfoMap.insert(std::pair< ui32, const nuTypeInfo* >(crc, &type_info));
    NU_ASSERT(ret.second, "Invalid resource CRC.\n");
#endif
    NU_TRACE("Registering resource: %s (%s)\n", type_info.getName(), p_res->getExt());
    delete p_res;
  }

  if(type_info.getChild())
    buildResourceTable(*type_info.getChild());

  if(type_info.getSibling())
    buildResourceTable(*type_info.getSibling());
}

void nuResourceManager::load(nuResource& res)
{
  nuStream stream;
  if(stream.create(res.mPath, nude::FATTR_READ) == nude::SERROR_NONE) {
    NU_TRACE("Loading: %s\n", res.mPath);
    nuResource::ERROR_CODE ret = res.setup(stream);
    res.mErrorCode = ret;
    if(ret == nuResource::ERROR_NONE) {
      res.mInitialized = 1;
    } else {
      NU_TRACE("Error loading %s: %s.\n", res.mPath, res.getErrorStr(ret));
    }
  } else {
    res.mErrorCode = nuResource::ERROR_NOT_FOUND;
    NU_TRACE("Resource load error: %s.\n", res.getErrorStr(nuResource::ERROR_NOT_FOUND));
  }
}
