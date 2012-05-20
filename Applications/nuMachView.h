/*!
 * \file nuMachView.h
 * \brief View class for MacOSX.
 * \author Nus
 * \date 2011/09/15 23:27
 */

/*!
 * \class nuMachView
 * \brief View class.
 */
@interface nuMachView : NSOpenGLView
{
  CVDisplayLinkRef displayLink;
  bool displayLinkStarted;
  i64 videoTime;
}

@property (nonatomic, readonly, getter = isDisplayLinkStarted) bool displayLinkStarted;

//! \brief Initialize with NSWindow.
- (id) initWithWindow: (NSWindow*) window;
//! \brief Stop draw procedure.
- (void) startDraw;
//! \brief Stop draw procedure.
- (void) stopDraw;

- (CGLContextObj) lockContext;
- (void) unlockContext: (CGLContextObj) ctx;

@end
