/*!
 * \file nuMachWindow.mm
 * \brief Window class for MacOSX.
 * \author Nus
 * \date 2011/09/14 23:53
 */

#import "nuMachWindow.h"

@implementation nuMachWindow

- (id) initWithName: (NSString*) name
{
  NSRect rect;
  NSUInteger style;

  rect.origin = CGPointMake(0.0f, 0.0f);
  rect.size = CGSizeMake(1280.0f, 720.0f);

  style = NSTitledWindowMask;
  style |= NSClosableWindowMask;
  style |= NSMiniaturizableWindowMask;
  style |= NSResizableWindowMask;

  self = [super initWithContentRect: rect
                styleMask: style
                backing: NSBackingStoreBuffered
                defer: YES];

  if(self) {
    [self setTitle: name ? name : @"nuDE"];
  }

  return self;
}

- (BOOL) windowShouldClose: (id) sender
{
  [NSApp terminate: self];
  return YES;
}

- (NSTimeInterval) animationResizeTime: (NSRect) window_frame
{
  return 0.0;
}

@end
