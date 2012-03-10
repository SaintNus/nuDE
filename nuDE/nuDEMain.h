/*!
 * \file nuDEMain.h
 * \brief Main loop.
 * \author Nus
 * \date 2012/03/11 00:28
 */

#ifndef __NUDEMAIN_H__
#define __NUDEMAIN_H__

class nuDEMain : public nuAppMain
{
  DECLARE_TYPE_INFO;

  void update(void);

public:
  nuDEMain();
  ~nuDEMain();

  void initialize(void);
  void terminate(void);

};

#endif
