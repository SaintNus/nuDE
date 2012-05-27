/*!
 * \file nuDrawString.h
 * \brief String drawer.
 * \author Nus
 * \date 2012/05/27 21:41
 */

#import "nuGContext.h"

@interface nuDrawString : NSObject
{
  NSRect bounds;
  NSMutableAttributedString* string;
  NSImage* image;
  GLuint textureID;
  BOOL updateString;

  BOOL antiAlias;
	NSSize margin;
  NSColor* color;
  NSSize textureSize;
}

@property (nonatomic, getter = isAntiAlias) BOOL antiAlias;
@property (nonatomic) NSSize margin;

- (id) initWithFrame: (const NSRect&) frame;
- (void) update;
- (void) drawAtPoint: (const NSPoint&) point context: (nuGContext&) graphicContext;
- (void) terminate;

- (void) setString: (NSAttributedString*) attributedString color: (NSColor*) textColor;

@end
