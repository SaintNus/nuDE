/*!
 * \file nuGResManager.h
 * \brief GL resource manager.
 * \author Nus
 * \date 2012/02/05 19:09
 */

#ifndef __NUGRESMANAGER_H__
#define __NUGRESMANAGER_H__

#include "nuGResource.h"
#include "nuVertexArray.h"
#include "nuVertexBuffer.h"
#include "nuElementBuffer.h"
#include "nuUniformBuffer.h"
#include "nuShaderList.h"
#include "nuShaderProgram.h"

class nuGResManager : public nuObject
{
  DECLARE_TYPE_INFO;

  static const ui32 EXPANDABLE_TABLE_NUM = 512;

  typedef std::list< nuGResource* > ResList;
  typedef ResList::iterator ResListIterator;
  typedef ResList::const_iterator ResListConstIterator;

  nuMutex mStaticResMutex;
  ResList mStaticResource;

  nuMutex mDynamicResMutex;
  ResList mDynamicResource;

  nuGResource** mpUpdateTable;
  ui32 mUpdateTableNum;

  nude::ShaderList mShaderList;

  void deleteResources(ResList& resource_list, nuMutex& mutex, i64 frame_id);
  void updateResources(ResList& resource_list, nuMutex& mutex);

  void registerResource(nuGResource& resource) {
    resource.mpGResManager = this;
    switch (resource.getUsage()) {
    case nuGResource::STATIC_RESOURCE:
      {
        nuMutex::Autolock mutex(mStaticResMutex);
        mStaticResource.push_back(&resource);
      }
      break;
    case nuGResource::DYNAMIC_RESOURCE:
      {
        nuMutex::Autolock mutex(mDynamicResMutex);
        mDynamicResource.push_back(&resource);
      }
      break;
    default:
      NU_ASSERT_C(false);
      break;
    }
  }

public:
  nuGResManager();
  ~nuGResManager();

  void setShaderList(nude::ShaderList& shader_list);
  const nude::ShaderList& getShaderList(void) const {
    return mShaderList;
  }

  nude::VertexArray createVertexArray(void);
  nude::VertexBuffer createVertexBuffer(size_t size, nuGResource::RESOURCE_USAGE usage);
  nude::ElementBuffer createElementBuffer(nuElementBuffer::ELEMENT_TYPE type,
                                          ui32 size,
                                          nuGResource::RESOURCE_USAGE usage);
  nude::ShaderProgram createShaderProgram(nude::ProgramList program);
  nude::UniformBuffer createUniformBuffer(nude::ProgramList program_id, ui32 ubo_id);

  void updateStaticResource(i64 frame_id);
  void updateDynamicResource(i64 frame_id);

};

#endif
