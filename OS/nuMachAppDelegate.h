/*!
 * \file nuMachAppDelegate.h
 * \brief Application delegate for MacOSX
 * \author Nus
 * \date 2011/09/14 23:44
 */

#import "nuMachWindow.h"
#import "nuMachView.h"

@interface nuMachAppDelegate : NSObject< NSApplicationDelegate >
{
  nuMachWindow* window;
  nuMachView* view;
  NSThread* appThread;
}

@property (readonly) nuMachWindow* window;
@property (readonly) nuMachView* view;

+ (id) instance;
- (NSString*) windowTitle;

- (const nuTypeInfo&) appMain;

@end
