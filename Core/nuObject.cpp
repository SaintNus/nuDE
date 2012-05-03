/*!
 * \file nuObject.cpp
 * \brief Base class.
 * \author Nus
 * \date 2011/09/11 23:54
 */

#include "nuObject.h"

nuObject::PrivateTypeInfo::PrivateTypeInfo() : nuTypeInfo("nuObject", nullptr) {}
nuObject::PrivateTypeInfo::~PrivateTypeInfo() {}
size_t nuObject::PrivateTypeInfo::getSize(void) const { return sizeof(nuObject); }
nuTypeInfo::Type nuObject::PrivateTypeInfo::getType(void) const { return nuTypeInfo::ROOT; }
nuObject* nuObject::PrivateTypeInfo::createInstance(void) const { return nullptr; }
nuObject::PrivateTypeInfo nuObject::mTypeInfo;
const nuTypeInfo& nuObject::TypeInfo(void) { return mTypeInfo; }

nuObject::~nuObject()
{
  // None...
}
