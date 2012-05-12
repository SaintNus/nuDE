/*!
 * \file nuDEAppDelegate.mm
 * \brief Application delegate for nuDE.
 * \author Nus
 * \date 2011/09/11 17:49
 */

#import "nuDEInclude.h"

#import "nuDEAppDelegate.h"
#import "nuDEMain.h"

@implementation nuDEAppDelegate

- (const nuTypeInfo&) appMain
{
  return nuDEMain::TypeInfo();
}

- (void) applicationDidFinishLaunching: (NSNotification*) notification
{
  [super applicationDidFinishLaunching: notification];
  NU_TRACE("Application launched.\n");
}

@end
