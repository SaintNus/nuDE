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
  nuAppMain();
  virtual ~nuAppMain();

  i32 main(void);

};

#endif