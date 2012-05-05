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
- (void) drawFrameWithContext: (CGLContextObj) ctx timeStamp: (const CVTimeStamp*) time_stamp;

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
    videoTime = 0;
  }
  [pf release];
  pf = nil;
  return self;
}

- (void) prepareOpenGL
{
  GLint swapInt = 0;

  [[self openGLContext] makeCurrentContext];
  [[self openGLContext] setValues: &swapInt forParameter: NSOpenGLCPSwapInterval];

  CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
  
  CVDisplayLinkSetOutputCallback(displayLink, &DisplayLinkCallback, self);
  
  CGLContextObj ctx = static_cast< CGLContextObj >([[self openGLContext] CGLContextObj]);
  CGLPixelFormatObj pf = static_cast< CGLPixelFormatObj >([[self pixelFormat] CGLPixelFormatObj]);

  {
    GLint sync = 1;
    CGLSetParameter(ctx, kCGLCPSwapInterval, &sync);
  }

  CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, ctx, pf);

  displayLinkStarted = false;
}

- (CVReturn) drawFrameForTime: (const CVTimeStamp*) output_time
{
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

  CGLContextObj ctx = [self lockContext];
  [self drawFrameWithContext: ctx timeStamp: output_time];
  [self unlockContext: ctx];

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

- (void) drawFrameWithContext: (CGLContextObj) ctx timeStamp: (const CVTimeStamp*) time_stamp
{
  NU_ASSERT_C(nuApplication::appMain());
  if(nuApplication::appMain()->getState() == nuAppMain::RUNNING) {
    if(nuApplication::renderGL().isCommandSubmitted()) {
      const f64 freq_60 = 1.0 / 60.0;
      const f64 min_ftime = 0.5;
      const f64 max_ftime = 5.0;

      i64 frame_id = nuApplication::renderGL().updateGraphicResources();

      i64 v_time = time_stamp->videoTime;
      i64 delta_time = v_time - videoTime;

      f64 refresh_freq = 1.0 / static_cast< f64 >(time_stamp->videoRefreshPeriod);
      f64 frame_scale = time_stamp->rateScalar * static_cast< f64 >(time_stamp->videoTimeScale) * refresh_freq * freq_60;
      f64 frame_time = static_cast< f64 >(delta_time) * refresh_freq;
      
      videoTime = v_time;

      if(frame_time < min_ftime)
        frame_time = min_ftime;
      else if(frame_time > max_ftime)
        frame_time = max_ftime;

      nuApplication::instance()->setFrameTime(static_cast< f32 >(frame_time * frame_scale));
      // NU_TRACE("Frame time: %f\n", nuApplication::instance()->getFrameTime());
      nuApplication::entityManager().setupEntity(frame_id);      

      if(nuApplication::renderGL().render())
        CGLFlushDrawable(ctx);
    }
  }
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

- (CGLContextObj) lockContext
{
  CGLContextObj ctx = static_cast< CGLContextObj >([[self openGLContext] CGLContextObj]);
  [[self openGLContext] makeCurrentContext];
  CGLLockContext(ctx);
  return ctx;
}

- (void) unlockContext: (CGLContextObj) ctx
{
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
