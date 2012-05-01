/*!
 * \file nuEntity.h
 * \brief Entity class.
 * \author Nus
 * \date 2012/05/01 19:26
 */

#ifndef __NUENTITY_H__
#define __NUENTITY_H__

class nuEntity : public nuObject
{
  DECLARE_TYPE_INFO;
  friend class nuEntityManager;

public:
  nuEntity();
  virtual ~nuEntity() = 0;

};

#endif
