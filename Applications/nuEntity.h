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

  union {
    ui32 mAttribute;
    struct {
      ui32 mRegistered: 1;
      ui32 mReserved: 31;
    };
  };

  nuEntity* mpPrev;
  nuEntity* mpNext;

protected:
  nuEntity();
  virtual ~nuEntity();

  virtual void setup(nuGSetupContext& setup) = 0;
  virtual void update(void) = 0;
  virtual void draw(nuGContext& context) = 0;

};

#endif
