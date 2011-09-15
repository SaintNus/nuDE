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

- (void) applicationDidFinishLaunching: (NSNotification*) notification
{
  gpAppDelegate = self;
}

@end
