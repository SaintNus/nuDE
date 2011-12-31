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
  bool displaLinkStarted;
}

@property (nonatomic, readonly) bool displayLinkStarted;

//! \brief Initialize with NSWindow.
- (id) initWithWindow: (NSWindow*) window;
//! \brief Stop draw procedure.
- (void) startDraw;
//! \brief Stop draw procedure.
- (void) stopDraw;

- (void) lockContext;
- (void) unlockContext;

@end
