/*!
 * \file nuDEAppDelegate.h
 * \brief Application delegate for nuDE.
 * \author Nus
 * \date 2011/09/11 17:49
 */

#import <Cocoa/Cocoa.h>

@interface nuDEAppDelegate : NSObject <NSApplicationDelegate>
{
  NSWindow* window;
}

@property (assign) IBOutlet NSWindow* window;

@end
