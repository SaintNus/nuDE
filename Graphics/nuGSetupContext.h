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
  const nuRect& mViewport;
  bool mResizeView;

  nuGSetupContext();

public:
  nuGSetupContext(i64 frame_id, const nuRect& rect, bool resize);
  ~nuGSetupContext();

  i64 getFrameID(void) const {
    return mFrameID;
  }

  bool isResized(void) const {
    return mResizeView;
  }

  const nuRect& getViewport(void) const {
    return mViewport;
  }

};

#endif
