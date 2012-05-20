/*!
 * \file nuMachAppDelegate.mm
 * \brief Application delegate for MacOSX
 * \author Nus
 * \date 2011/09/14 23:44
 */

#import "ApplicationsInclude.h"

#import "nuMachAppDelegate.h"
#import "nuApplication.h"

static nuMachAppDelegate* gpAppDelegate = nil;

@interface nuMachAppDelegate(Private)

- (void) appThreadProcedure: (id) sender;

@end

@implementation nuMachAppDelegate

@synthesize window;
@synthesize view;

+ (id) instance
{
  return gpAppDelegate;
}

- (NSString*) windowTitle
{
  return @"nuDE";
}

- (const nuTypeInfo&) appMain
{
  return nuAppMain::TypeInfo();
}

- (void) applicationDidFinishLaunching: (NSNotification*) notification
{
  NU_TRACE("Application launching!\n");
  gpAppDelegate = self;
  window = [[nuMachWindow alloc] initWithName: [self windowTitle]];
  view = [[nuMachView alloc] initWithWindow: window];
  appThread = [[NSThread alloc] initWithTarget: self
                                selector: @selector(appThreadProcedure:)
                                object: nil];
  if(window && view && appThread) {
    [window center];
    [window makeKeyAndOrderFront: self];

    nuSingleton< nuApplication >::createInstance();
    nuApplication::instance()->initialize([self appMain]);

    CGLContextObj ctx = [view lockContext];
    nuResHandle res = nuApplication::resourceManager().createResource("res://Resources/Shader.shlst");
    nude::ShaderList shader_list(res.cast< nuShaderList >());
    nuApplication::renderGL().initialize(shader_list);
    CGLFlushDrawable(ctx);
    [view unlockContext: ctx];

    [appThread start];
    [view startDraw];
  }
}

- (void) applicationWillTerminate: (NSNotification*) notification
{
  NU_TRACE("Application terminating!\n");
  [view stopDraw];

  CGLContextObj ctx = [view lockContext];
  nuApplication::renderGL().terminate();
  [view unlockContext: ctx];

  [view release];
  view = nil;
  [window release];
  window = nil;
  [appThread release];

  nuApplication::instance()->terminate();
  nuSingleton< nuApplication >::deleteInstance();

  NU_TRACE("--- Bye ---\n");
}

- (void) appThreadProcedure: (id) sender
{
  NU_TRACE("Running application thread.\n");
  nuApplication::instance()->run();
}

@end
