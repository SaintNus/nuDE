/*!
 * \file nuDrawString.mm
 * \brief String drawer.
 * \author Nus
 * \date 2012/05/27 21:41
 */

#import "GraphicsInclude.h"
#import "nuDrawString.h"

@implementation nuDrawString

@synthesize antiAlias;
@synthesize margin;

- (id) initWithFrame: (const NSRect&) frame
{
  self = [super init];

  if(self) {
    bounds = frame;
    string = nil;
    color = nil;

    image = [[NSImage alloc] initWithSize: bounds.size];

    textureID = 0;
    antiAlias = YES;
    margin = NSMakeSize(4, 2);
    textureSize = NSMakeSize(0.0f, 0.0f);
  }

  return self;
}

- (void) dealloc
{
  [self terminate];

  [image release];
  [string release];
  [color release];

  [super dealloc];
}

- (void) update
{
  if(!updateString)
    return;

  BOOL first = NO;

  if(textureID == 0) {
    CHECK_GL_ERROR(glGenTextures(1, &textureID));

    CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_RECTANGLE, textureID));

    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    first = YES;
  }

	[image lockFocus];
	[[NSGraphicsContext currentContext] setShouldAntialias: antiAlias];

  [color set];
  [string drawAtPoint: NSMakePoint(margin.width, margin.height)];

  NSBitmapImageRep* bitmap = [[NSBitmapImageRep alloc] initWithFocusedViewRect: bounds];
  [image unlockFocus];

  if(!first) {
    CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_RECTANGLE, textureID));
  }

  NSSize size = NSMakeSize([bitmap pixelsWide], [bitmap pixelsHigh]);
  if(NSEqualSizes(size, textureSize)) {
    GLsizei width = static_cast< GLsizei >(textureSize.width);
    GLsizei height = static_cast< GLsizei >(textureSize.height);
    GLint row_size = width * ([bitmap hasAlpha] ? 4 : 3);
    GLint alignment = row_size % 4 == 0 ? 4 : 1;

    CHECK_GL_ERROR(glPixelStorei(GL_UNPACK_ALIGNMENT, alignment));
    CHECK_GL_ERROR(glTexSubImage2D(GL_TEXTURE_RECTANGLE,
                                   0,
                                   0,
                                   0,
                                   width,
                                   height,
                                   [bitmap hasAlpha] ? GL_RGBA : GL_RGB,
                                   GL_UNSIGNED_BYTE,
                                   [bitmap bitmapData]));
  } else {
    GLsizei width = static_cast< GLsizei >(size.width);
    GLsizei height = static_cast< GLsizei >(size.height);
    GLint row_size = width * ([bitmap hasAlpha] ? 4 : 3);
    GLint alignment = row_size % 4 == 0 ? 4 : 1;

    CHECK_GL_ERROR(glPixelStorei(GL_UNPACK_ALIGNMENT, alignment));
    CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_RECTANGLE,
                                0,
                                [bitmap hasAlpha] ? GL_RGBA8 : GL_RGB8,
                                width,
                                height,
                                0,
                                [bitmap hasAlpha] ? GL_RGBA : GL_RGB,
                                GL_UNSIGNED_BYTE,
                                [bitmap bitmapData]));
    textureSize = size;
  }

  [bitmap release];

  updateString = NO;
}

- (void) drawAtPoint: (const NSPoint&) point context: (nuGContext&) graphicContext
{
  if(textureID == 0)
    return;
}

- (void) terminate
{
  if(textureID != 0) {
    CHECK_GL_ERROR(glDeleteTextures(1, &textureID));
    textureID = 0;
  }
}

- (void) setString: (NSAttributedString*) attributedString color: (NSColor*) textColor
{
  updateString = YES;
}

@end
