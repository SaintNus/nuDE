/*!
 * \file nuResource.h
 * \brief Resource.
 * \author Nus
 * \date 2012/01/29 20:13
 */

#ifndef __NURESOURCE_H__
#define __NURESOURCE_H__

#include "nuObject.h"

class nuResource : public nuObject
{
  DECLARE_TYPE_INFO;

public:
  nuResource() {}
  ~nuResource() {}
};

#endif
