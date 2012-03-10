/*!
 * \file nuMachAppDelegate.mm
 * \brief Application delegate for MacOSX
 * \author Nus
 * \date 2011/09/14 23:44
 */

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
    INST(nuApplication)->initialize([self appMain]);

    nuApplication::renderGL().initTest();

    [appThread start];
    [view startDraw];
  }
}

- (void) applicationWillTerminate: (NSNotification*) notification
{
  NU_TRACE("Application terminating!\n");
  [view stopDraw];

  [view lockContext];
  nuApplication::renderGL().termTest();
  [view unlockContext];

  [view release];
  view = nil;
  [window release];
  window = nil;
  [appThread release];
  nuSingleton< nuApplication >::deleteInstance();
  NU_TRACE("--- Bye ---\n");
}

- (void) appThreadProcedure: (id) sender
{
  NU_TRACE("Running application thread.\n");
  INST(nuApplication)->run();
}

@end
