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
}

//! \brief Initialize with NSWindow.
- (id) initWithWindow: (NSWindow*) window;

@end
