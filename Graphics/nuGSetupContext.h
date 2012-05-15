/*!
 * \file nuGSetupContext.h
 * \brief Graphic setup context.
 * \author Nus
 * \date 2012/05/01 20:32
 */

#ifndef __NUGSETUPCONTEXT_H__
#define __NUGSETUPCONTEXT_H__

class nuGSetupContext
{
  i64 mFrameID;

public:
  nuGSetupContext();
  ~nuGSetupContext();

  void beginSetup(i64 frame_id);
  void endSetup(void);

  void map(class nuVertexBuffer& vertex_buffer);
  void map(class nuElementBuffer& element_buffer);

};

#endif
