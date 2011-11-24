/*!
 * \file nuAppMain.h
 * \brief Application main class.
 * \author Nus
 * \date 2011/11/03 22:13
 */

#ifndef __NUAPPMAIN_H__
#define __NUAPPMAIN_H__

/*!
 * \class nuAppMain
 * \brief Application main class.
 */
class nuAppMain : public nuObject
{
  DECLARE_TYPE_INFO;

public:
  enum STATE {
    UNINITIALIZED = 0,
    READY,
    RUNNING,
    TERMINATING,
  };

  nuAppMain();
  virtual ~nuAppMain();

  virtual void initialize(void);
  virtual i32 main(void);
  virtual void terminate(void);

  nuRenderGL& getRenderGL(void) const {
    return *mpRenderGL;
  }
  STATE getState(void) const {
    return mState;
  }

protected:
  virtual nuRenderGL* createRenderGL(void) const;

  STATE mState;
  nuRenderGL* mpRenderGL;

};

#endif
