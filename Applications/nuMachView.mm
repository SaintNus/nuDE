/*!
 * \file nuMacView.mm
 * \brief View class for MacOSX.
 * \author Nus
 * \date 2011/09/15 23:27
 */

#import "nuApplication.h"
#import "nuMachView.h"

static CVReturn DisplayLinkCallback(CVDisplayLinkRef display_link,
                                    const CVTimeStamp* now,
                                    const CVTimeStamp* output_time,
                                    CVOptionFlags flags_in,
                                    CVOptionFlags* flags_out,
                                    void* dl_context);

@interface nuMachView (Private)

- (CVReturn) drawFrameForTime: (const CVTimeStamp*) output_time;
- (void) drawFrame;

@end

@implementation nuMachView

@synthesize displayLinkStarted;

// Initialize with NSWindow.
- (id) initWithWindow: (NSWindow*) window
{
  NSOpenGLPixelFormatAttribute attributes[] = {
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFAColorSize, 32,
    NSOpenGLPFAAlphaSize, 8,
    NSOpenGLPFADepthSize, 24,
    NSOpenGLPFAStencilSize, 8,
    NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
    0,
  };
  
  NSOpenGLPixelFormat* pf = [[NSOpenGLPixelFormat alloc] initWithAttributes: attributes];
  if(!pf) {
    NU_TRACE("No appropriate pixel format available.\n");
    return nil;
  }
  
  NSRect content_rect = [window contentRectForFrameRect: [window frame]];
  self = [super initWithFrame: content_rect pixelFormat: pf];
  if(self) {
    [window setContentView: self];
    [pf release];
    pf = nil;
  }
  return self;
}

- (void) prepareOpenGL
{
  GLint swapInt = 1;

  [[self openGLContext] makeCurrentContext];
  [[self openGLContext] setValues: &swapInt forParameter: NSOpenGLCPSwapInterval];

  CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
  
  CVDisplayLinkSetOutputCallback(displayLink, &DisplayLinkCallback, self);
  
  CGLContextObj ctx = static_cast< CGLContextObj >([[self openGLContext] CGLContextObj]);
  CGLPixelFormatObj pf = static_cast< CGLPixelFormatObj >([[self pixelFormat] CGLPixelFormatObj]);
  CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, ctx, pf);

  displayLinkStarted = false;
}

- (CVReturn) drawFrameForTime: (const CVTimeStamp*) output_time
{
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

  [self lockContext];
  [self drawFrame];
  [self unlockContext];

  [pool release];
  return kCVReturnSuccess;
}

- (void) reshape
{ 
  [super reshape];
  #if !NDEBUG
  {
    NSSize frame_size = [self frame].size;
    NU_TRACE("Resizing to %.1f x %.1f\n", frame_size.width, frame_size.height);
  }
#endif
}

- (void) dealloc
{
  [self stopDraw];
  CVDisplayLinkRelease(displayLink);
  [super dealloc];
}

- (void) drawFrame
{
  NU_ASSERT_C(nuApplication::appMain());
  if(nuApplication::appMain()->getState() == nuAppMain::RUNNING)
    nuApplication::renderGL().render();
}

// Stop draw procedure.
- (void) startDraw
{
  if(!displayLinkStarted) {
    displayLinkStarted = true;
    CVDisplayLinkStart(displayLink);
  }
}

// Stop draw procedure.
- (void) stopDraw
{
  if(displayLinkStarted) {
    CVDisplayLinkStop(displayLink);
    displayLinkStarted = false;
  }
}

- (void) lockContext
{
  CGLContextObj ctx = static_cast< CGLContextObj >([[self openGLContext] CGLContextObj]);
  [[self openGLContext] makeCurrentContext];
  CGLLockContext(ctx);
}

- (void) unlockContext
{
  CGLContextObj ctx = static_cast< CGLContextObj >([[self openGLContext] CGLContextObj]);
  CGLFlushDrawable(ctx);
  CGLUnlockContext(ctx);
}

@end

CVReturn DisplayLinkCallback(CVDisplayLinkRef display_link,
                             const CVTimeStamp* now,
                             const CVTimeStamp* output_time,
                             CVOptionFlags flags_in,
                             CVOptionFlags* flags_out,
                             void* dl_context)
{
  return [(nuMachView*) dl_context drawFrameForTime: output_time];
}
