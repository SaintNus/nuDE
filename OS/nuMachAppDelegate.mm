/*!
 * \file nuMachAppDelegate.mm
 * \brief Application delegate for MacOSX
 * \author Nus
 * \date 2011/09/14 23:44
 */

#import "nuMachAppDelegate.h"

static nuMachAppDelegate* gpAppDelegate = nil;

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

- (void) applicationDidFinishLaunching: (NSNotification*) notification
{
  NU_TRACE("Application launching!\n");
  gpAppDelegate = self;
  window = [[nuMachWindow alloc] initWithName: [self windowTitle]];
  view = [[nuMachView alloc] initWithWindow: window];
  if(window && view) {
    [window center];
    [window makeKeyAndOrderFront: self];
  }
}

- (void) applicationWillTerminate:(NSNotification *)notification
{
  NU_TRACE("Application terminating!\n");
  [view release];
  view = nil;
  [window release];
  window = nil;
}

@end
