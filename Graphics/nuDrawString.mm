/*!
 * \file nuDrawString.mm
 * \brief String drawer.
 * \author Nus
 * \date 2012/05/27 21:41
 */

#include "GraphicsInclude.h"
#include "nuShaderInfo.h"
#include "nuGraphics.h"
#include "nuShaderProgram.h"
#include "nuDrawString.h"

#define FONT_NAME @"Osaka"

@interface _MachDrawString : NSObject
{
  NSMutableString* string;
  NSDictionary* attributes;
  NSImage* image;

  GLuint textureID;
  GLsizei textureWidth;
  GLsizei textureHeight;

  BOOL antiAlias;
  NSRect bounds;
	NSSize margin;
  f32 fontSize;
  BOOL updateString;

  NSRect currentRect;
}

@property (nonatomic, readonly, getter = isAntiAlias) BOOL antiAlias;
@property (nonatomic) NSSize margin;
@property (nonatomic) f32 fontSize;
@property (nonatomic, readonly) NSRect currentRect;
@property (nonatomic, readonly) GLuint textureID;
@property (nonatomic, readonly) NSMutableString* string;

- (id) initWithFrame: (const NSRect&) frame fontSize: (f32) size;
- (id) initWithString: (const c8 *) cString fontSize: (f32) size;
- (id) initWithString: (const c8*) cString withFrame: (const NSRect&) frame fontSize: (f32) size;
- (id) initWithString: (const c8 *) cString withMargin: (const NSSize&) marginSize fontSize: (f32) size;
- (void) update;
- (void) drawAtPoint: (const NSPoint&) point;
- (void) terminate;

- (void) setCString: (const c8*) cString;
- (void) setAntiAlias: (BOOL) enable;

- (void) initializeAttribute;

@end

@implementation _MachDrawString

@synthesize antiAlias;
@synthesize margin;
@synthesize fontSize;
@synthesize currentRect;
@synthesize textureID;
@synthesize string;

- (void) initializeAttribute
{
  string = nil;
  attributes = nil;
  image = nil;

  textureID = 0;
  antiAlias = YES;
  textureWidth = 0;
  textureHeight = 0;

  currentRect = NSMakeRect(0.0f, 0.0f, 0.0f, 0.0f);
}

- (id) initWithFrame: (const NSRect&) frame fontSize: (f32) size
{
  self = [super init];
  if(self) {
    [self initializeAttribute];
    bounds = frame;
    image = [[NSImage alloc] initWithSize: bounds.size];

    margin = NSMakeSize(0.0f, 0.0f);
    fontSize = size;
    updateString = NO;
  }

  return self;
}

- (id) initWithString: (const c8 *) cString fontSize: (f32) size
{
  self = [super init];
  if(self) {
    [self initializeAttribute];

    fontSize = size;
    NSFont* font = [NSFont fontWithName: FONT_NAME size: fontSize];
    string = [[NSMutableString alloc] initWithCString:cString encoding: NSUTF8StringEncoding];
    attributes = [[NSDictionary alloc] initWithObjectsAndKeys: font, NSFontAttributeName, nil];

    NSSize str_size = [string sizeWithAttributes: attributes];
    bounds = NSMakeRect(0.0f, 0.0f, str_size.width, str_size.height);
    image = [[NSImage alloc] initWithSize: bounds.size];

    margin = NSMakeSize(0.0f, 0.0f);
    updateString = YES;
  }

  return self;
}

- (id) initWithString: (const c8*) cString withFrame: (const NSRect&) frame fontSize: (f32) size
{
  self = [super init];
  if(self) {
    [self initializeAttribute];

    fontSize = size;
    NSFont* font = [NSFont fontWithName: FONT_NAME size: fontSize];
    string = [[NSMutableString alloc] initWithCString:cString encoding: NSUTF8StringEncoding];
    attributes = [[NSDictionary alloc] initWithObjectsAndKeys: font, NSFontAttributeName, nil];

    bounds = frame;
    image = [[NSImage alloc] initWithSize: bounds.size];

    margin = NSMakeSize(0.0f, 0.0f);
    updateString = YES;
  }

  return self;
}

- (id) initWithString: (const c8 *) cString withMargin: (const NSSize&) marginSize fontSize: (f32) size
{
  self = [super init];
  if(self) {
    [self initializeAttribute];

    fontSize = size;
    NSFont* font = [NSFont fontWithName: FONT_NAME size: fontSize];
    string = [[NSMutableString alloc] initWithCString:cString encoding: NSUTF8StringEncoding];
    attributes = [[NSDictionary alloc] initWithObjectsAndKeys: font, NSFontAttributeName, nil];

    NSSize str_size = [string sizeWithAttributes: attributes];
    margin = marginSize;
    bounds = NSMakeRect(0.0f, 0.0f, str_size.width + margin.width, str_size.height + margin.height);
    image = [[NSImage alloc] initWithSize: bounds.size];
    updateString = YES;
  }

  return self;
}

- (void) dealloc
{
  [self terminate];

  [image release];
  [string release];
  [attributes release];

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
    first = YES;
  }

	[image lockFocus];
  [image drawInRect: bounds fromRect: bounds operation: NSCompositeClear fraction: 1.0f];
	[[NSGraphicsContext currentContext] setShouldAntialias: antiAlias];

  [[NSColor blackColor] set];
  [string drawAtPoint: NSMakePoint(margin.width, margin.height) withAttributes: attributes];

  NSBitmapImageRep* bitmap = [[NSBitmapImageRep alloc] initWithFocusedViewRect: bounds];
  [image unlockFocus];

  if(!first) {
    CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_RECTANGLE, textureID));
  }

  GLsizei width = static_cast< GLsizei >([bitmap pixelsWide]);
  GLsizei height = static_cast< GLsizei >([bitmap pixelsHigh]);
  if(width == textureWidth && height == textureHeight) {
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
    textureWidth = width;
    textureHeight = height;

    currentRect.size.width = width;
    currentRect.size.height = height;
  }

  [bitmap release];

  updateString = NO;
}

- (void) drawAtPoint: (const NSPoint&) point
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

- (void) setCString: (const c8*) cString;
{
  if(!cString)
    return;

  if(!string) {
    NSFont* font = [NSFont fontWithName: FONT_NAME size: fontSize];
    string = [[NSMutableString alloc] initWithCString:cString encoding: NSUTF8StringEncoding];
    attributes = [[NSDictionary alloc] initWithObjectsAndKeys: font, NSFontAttributeName, nil];
    updateString = YES;
  } else if(strcmp(cString, [string UTF8String]) != 0) {
    NSString* temp_string = [NSString stringWithCString: cString encoding:NSUTF8StringEncoding];
    [string setString: temp_string];
    updateString = YES;
  }
}

- (void) setAntiAlias: (BOOL) enable
{
  if(antiAlias != enable) {
    antiAlias = enable;
    updateString = YES;
  }
}

@end

class _DrawStringResource
{
  static const ui32 EXPAND_ARRAY = 256;

  typedef std::list< nuDrawString* > DrawStringList;
  typedef DrawStringList::iterator DrawStringListIterator;
  typedef DrawStringList::const_iterator DrawStringListConstIterator;

  nuRenderGL& mRenderGL;
  GLuint mVertexID;
  GLuint mElementID;
  GLuint mArrayID;

  nude::ShaderProgram mShaderProgram;

  DrawStringList mDrawStringList;
  std::vector< nuDrawString* > mDrawStringArray;
  nuMutex mMutex;

public:
  _DrawStringResource(nuRenderGL& render_gl)
      : mVertexID(0),
        mElementID(0),
        mArrayID(0),
        mRenderGL(render_gl)
  {
    mDrawStringArray.reserve(256);

    mShaderProgram = mRenderGL.createShaderProgram(nude::Program_DrawString);

    CHECK_GL_ERROR(glGenBuffers(1, &mVertexID));
    CHECK_GL_ERROR(glGenBuffers(1, &mElementID));

    CHECK_GL_ERROR(glGenVertexArrays(1, &mArrayID));

    CHECK_GL_ERROR(glBindVertexArray(mArrayID));

    CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, mVertexID));
    const i32 vertex_idx[4] = { 0, 1, 2, 3 };
    CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_idx), vertex_idx, GL_STATIC_DRAW));

    CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementID));
    const ui16 element[4] = { 0, 1, 2, 3 };
    CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element), element, GL_STATIC_DRAW));

    CHECK_GL_ERROR(glEnableVertexAttribArray(nude::DrawString_inVertexIndex));
    CHECK_GL_ERROR(glVertexAttribIPointer(nude::DrawString_inVertexIndex, 1, GL_INT,
                                          sizeof(i32), nullptr));
  }

  ~_DrawStringResource() {
    DrawStringListIterator it;
    for(it = mDrawStringList.begin(); it != mDrawStringList.end(); ++it) {
      delete *it;
      *it = nullptr;
    }

    CHECK_GL_ERROR(glDeleteBuffers(1, &mVertexID));
    CHECK_GL_ERROR(glDeleteBuffers(1, &mElementID));
    CHECK_GL_ERROR(glDeleteVertexArrays(1, &mArrayID));
  }

  void registerDrawString(nuDrawString& draw_string) {
    NU_ASSERT_C(draw_string.mRegistered == 0);
    nuMutex::Autolock lock(mMutex);
    draw_string.mRegistered = 1;
    mDrawStringList.push_back(draw_string.incRefCount());
  }

  void update(void) {
    nuMutex::Autolock lock(mMutex);
    mDrawStringArray.clear();

    DrawStringListIterator it = mDrawStringList.begin();
    while(it != mDrawStringList.end()) {
      if((*it)->mRefCount == 1) {
        (*it)->decRefCount();
        it = mDrawStringList.erase(it);
      } else {
        ++it;
      }
    }

    for(it = mDrawStringList.begin(); it != mDrawStringList.end(); ++it) {
      nuDrawString* p_drawstr = *it;
      if(p_drawstr->mDrawString == 1) {
        mDrawStringArray.push_back(p_drawstr);
        p_drawstr->mDrawString = 0;
      }
    }

    std::vector< nuDrawString* >::const_iterator cit;
    for(cit = mDrawStringArray.begin(); cit != mDrawStringArray.end(); ++cit) {
      (*cit)->update();
    }
  }

  void draw(nuRenderGL::RenderContext& render_context) {
    if(mDrawStringArray.empty())
      return;

    const nuRect& vp = mRenderGL.getViewport();

    CHECK_GL_ERROR(glUseProgram(mShaderProgram.cast< nuShaderProgram >()->getHandle()));
    GLuint u_loc = mShaderProgram.cast< nuShaderProgram >()->getUniformLocation(nude::DrawString_uniStringTex);
    CHECK_GL_ERROR(glUniform1i(u_loc, 0));
    u_loc = mShaderProgram.cast< nuShaderProgram >()->getUniformLocation(nude::DrawString_uniViewport);
    CHECK_GL_ERROR(glUniform4f(u_loc, vp.size().w(), vp.size().h(),
                               1.0f / vp.size().w(), 1.0f / vp.size().h()));

    if(render_context.current_vertex_array != mArrayID) {
      render_context.current_vertex_array = mArrayID;
      CHECK_GL_ERROR(glBindVertexArray(render_context.current_vertex_array));
    }

    if(render_context.current_array_buffer != mVertexID) {
      render_context.current_array_buffer = mVertexID;
      CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, render_context.current_array_buffer));
    }

    if(render_context.current_element_buffer != mElementID) {
      render_context.current_element_buffer = mElementID;
      CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_context.current_element_buffer));
    }

    GLint origin_x = static_cast< GLint >(vp.lb().x());
    GLint origin_y = static_cast< GLint >(vp.lb().y());
    GLsizei width = static_cast< GLint >(vp.size().w());
    GLsizei height = static_cast< GLint >(vp.size().h());

    if(origin_x != render_context.viewport.origin_x ||
       origin_y != render_context.viewport.origin_y ||
       width != render_context.viewport.width ||
       height != render_context.viewport.height) {
      render_context.viewport.origin_x = origin_x;
      render_context.viewport.origin_y = origin_y;
      render_context.viewport.width = width;
      render_context.viewport.height = height;
      CHECK_GL_ERROR(glViewport(render_context.viewport.origin_x,
                                render_context.viewport.origin_y,
                                render_context.viewport.width,
                                render_context.viewport.height));
    }

    if(render_context.scissor.enable != true) {
      render_context.scissor.enable = true;
      CHECK_GL_ERROR(glEnable(GL_SCISSOR_TEST));
    }

    if(origin_x != render_context.scissor.left ||
       origin_y != render_context.scissor.bottom ||
       width != render_context.scissor.width ||
       height != render_context.scissor.height) {
      render_context.scissor.left = origin_x;
      render_context.scissor.bottom = origin_y;
      render_context.scissor.width = width;
      render_context.scissor.height = height;
      CHECK_GL_ERROR(glScissor(render_context.scissor.left,
                               render_context.scissor.bottom,
                               render_context.scissor.width,
                               render_context.scissor.height));
    }

    if(render_context.depth_test.enable == true) {
      render_context.depth_test.enable = false;
      CHECK_GL_ERROR(glDisable(GL_DEPTH_TEST));
    }

    if(render_context.stencil_test.enable == true) {
      render_context.stencil_test.enable = false;
      CHECK_GL_ERROR(glDisable(GL_STENCIL_TEST));
    }

    if(render_context.blending.enable != true) {
      render_context.blending.enable = true;
      CHECK_GL_ERROR(glEnable(GL_BLEND));
    }
    if(render_context.blending.equation != nude::BLEND_EQ_ADD) {
      render_context.blending.equation = nude::BLEND_EQ_ADD;
      CHECK_GL_ERROR(glBlendEquation(render_context.blending.equation));
    }
    if(render_context.blending.source != nude::BLEND_SRC_ALPHA ||
       render_context.blending.destination != nude::BLEND_ONE_MINUS_SRC_ALPHA) {
      render_context.blending.source = nude::BLEND_SRC_ALPHA;
      render_context.blending.destination = nude::BLEND_ONE_MINUS_SRC_ALPHA;
      CHECK_GL_ERROR(glBlendFunc(render_context.blending.source, render_context.blending.destination));
    }

    if(render_context.rasterizer.culling == true) {
      render_context.rasterizer.culling = false;
      CHECK_GL_ERROR(glDisable(GL_CULL_FACE));
    }

    CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0));

    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    std::vector< nuDrawString* >::const_iterator it;
    for(it = mDrawStringArray.begin(); it != mDrawStringArray.end(); ++it) {
      nuDrawString* p_drawstr = *it;
      _MachDrawString* draw_string = static_cast< _MachDrawString* >(p_drawstr->mpDrawString);
      NSRect rect = [draw_string currentRect];

      CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_RECTANGLE, [draw_string textureID]));

      f32 temp[4][2];

      u_loc = mShaderProgram.cast< nuShaderProgram >()->getUniformLocation(nude::DrawString_uniPosition_0);
      temp[0][0] = static_cast< f32 >(rect.origin.x);
      temp[0][1] = static_cast< f32 >(rect.origin.y + rect.size.height);
      temp[1][0] = static_cast< f32 >(rect.origin.x);
      temp[1][1] = static_cast< f32 >(rect.origin.y);
      temp[2][0] = static_cast< f32 >(rect.origin.x + rect.size.width);
      temp[2][1] = static_cast< f32 >(rect.origin.y + rect.size.height);
      temp[3][0] = static_cast< f32 >(rect.origin.x + rect.size.width);
      temp[3][1] = static_cast< f32 >(rect.origin.y);

      CHECK_GL_ERROR(glUniform2fv(u_loc, 4, reinterpret_cast< f32* >(temp)));

      u_loc = mShaderProgram.cast< nuShaderProgram >()->getUniformLocation(nude::DrawString_uniUV_0);
      temp[0][0] = 0.0f;
      temp[0][1] = 0.0f;
      temp[1][0] = 0.0f;
      temp[1][1] = static_cast< f32 >(rect.size.height);
      temp[2][0] = static_cast< f32 >(rect.size.width);
      temp[2][1] = 0.0f;
      temp[3][0] = static_cast< f32 >(rect.size.width);
      temp[3][1] = static_cast< f32 >(rect.size.height);

      CHECK_GL_ERROR(glUniform2fv(u_loc, 4, reinterpret_cast< f32* >(temp)));

      u_loc = mShaderProgram.cast< nuShaderProgram >()->getUniformLocation(nude::DrawString_uniColor);
      CHECK_GL_ERROR(glUniform4f(u_loc,
                                 p_drawstr->mDrawColor.fr(),
                                 p_drawstr->mDrawColor.fg(),
                                 p_drawstr->mDrawColor.fb(),
                                 p_drawstr->mDrawColor.fa()));

      CHECK_GL_ERROR(glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, nullptr))
    }
  }
};

static _DrawStringResource* _DrawStringSingleton = nullptr;

void nuDrawString::initialize(nuRenderGL& render_gl)
{
  if(_DrawStringSingleton == nullptr) {
    _DrawStringSingleton = new _DrawStringResource(render_gl);
    NU_ASSERT(_DrawStringSingleton != nullptr, "Cannot create draw string manager.\n");
  }
}

void nuDrawString::terminate(void)
{
  if(_DrawStringSingleton) {
    delete _DrawStringSingleton;
    _DrawStringSingleton = nullptr;
  }
}

void nuDrawString::updateResource(void)
{
  _DrawStringSingleton->update();
}

void nuDrawString::draw(nuRenderGL::RenderContext& render_context)
{
  _DrawStringSingleton->draw(render_context);
}

IMPLEMENT_TYPE_INFO_INST(nuDrawString, nuObject, nullptr);

nuDrawString::nuDrawString(const nuRect& rect, f32 size)
    : mpDrawString(nullptr),
      mDrawString(0),
      mRegistered(0),
      mReserved(0),
      mRefCount(0),
      mColor(nuColor::White),
      mDrawColor(nuColor::White)
{
  NSRect frame = NSMakeRect(rect.lb().x(), rect.lb().y(), rect.size().w(), rect.size().h());
  _MachDrawString* draw_string = [[_MachDrawString alloc] initWithFrame: frame fontSize: size];
  mpDrawString = draw_string;
  NU_ASSERT(mpDrawString != nil, "Cannot allocate draw string.\n");
  _DrawStringSingleton->registerDrawString(*this);
}

nuDrawString::nuDrawString(const c8* str, f32 size)
    : mpDrawString(nullptr),
      mDrawString(0),
      mRegistered(0),
      mReserved(0),
      mRefCount(0),
      mColor(nuColor::White),
      mDrawColor(nuColor::White)
{
  _MachDrawString* draw_string = [[_MachDrawString alloc] initWithString: str fontSize: size];
  mpDrawString = draw_string;
  NU_ASSERT(mpDrawString != nil, "Cannot allocate draw string.\n");
  _DrawStringSingleton->registerDrawString(*this);
}

nuDrawString::nuDrawString(const c8* str, const nuRect& rect, f32 size)
    : mpDrawString(nullptr),
      mDrawString(0),
      mRegistered(0),
      mReserved(0),
      mRefCount(0),
      mColor(nuColor::White),
      mDrawColor(nuColor::White)
{
  NSRect frame = NSMakeRect(rect.lb().x(), rect.lb().y(), rect.size().w(), rect.size().h());
  _MachDrawString* draw_string = [[_MachDrawString alloc] initWithString: str withFrame: frame fontSize: size];
  mpDrawString = draw_string;
  NU_ASSERT(mpDrawString != nil, "Cannot allocate draw string.\n");
  _DrawStringSingleton->registerDrawString(*this);
}

nuDrawString::nuDrawString(const c8* str, const nuSize& margin, f32 size)
    : mpDrawString(nullptr),
      mDrawString(0),
      mRegistered(0),
      mReserved(0),
      mRefCount(0),
      mColor(nuColor::White),
      mDrawColor(nuColor::White)
{
  NSSize sz = NSMakeSize(margin.w(), margin.h());
  _MachDrawString* draw_string = [[_MachDrawString alloc] initWithString: str withMargin: sz fontSize: size];
  mpDrawString = draw_string;
  NU_ASSERT(mpDrawString != nil, "Cannot allocate draw string.\n");
  _DrawStringSingleton->registerDrawString(*this);
}

nuDrawString::~nuDrawString()
{
  _MachDrawString* draw_string = static_cast< _MachDrawString* >(mpDrawString);
  [draw_string terminate];
  [draw_string release];
}

void nuDrawString::update(void)
{
  _MachDrawString* draw_string = static_cast< _MachDrawString* >(mpDrawString);
  [draw_string update];
  if(mDrawColor != mColor)
    mDrawColor = mColor;
}

bool nuDrawString::isAntiAliased(void) const
{
  _MachDrawString* draw_string = static_cast< _MachDrawString* >(mpDrawString);
  return [draw_string isAntiAlias] == YES;
}

nuSize nuDrawString::getMargin(void) const
{
  _MachDrawString* draw_string = static_cast< _MachDrawString* >(mpDrawString);
  NSSize margin = [draw_string margin];
  return nuSize(static_cast< f32 >(margin.width), static_cast< f32 >(margin.height));
}

void nuDrawString::enableAntiAlias(bool enable)
{
  _MachDrawString* draw_string = static_cast< _MachDrawString* >(mpDrawString);
  [draw_string setAntiAlias: enable ? YES : NO];
}

void nuDrawString::setMargin(const nuSize& size)
{
  _MachDrawString* draw_string = static_cast< _MachDrawString* >(mpDrawString);
  NSSize sz = NSMakeSize(size.w(), size.h());
  [draw_string setMargin: sz];
}

const c8* nuDrawString::getString(void) const
{
  _MachDrawString* draw_string = static_cast< _MachDrawString* >(mpDrawString);
  return [[draw_string string] UTF8String];
}

const nuColor& nuDrawString::getColor(void) const
{
  return mColor;
}

void nuDrawString::setString(const c8* string)
{
  _MachDrawString* draw_string = static_cast< _MachDrawString* >(mpDrawString);
  [draw_string setCString: string];
}

void nuDrawString::setColor(const nuColor& color)
{
  mColor = color;
}

void nuDrawString::drawAt(const nuPoint& pos)
{
  _MachDrawString* draw_string = static_cast< _MachDrawString* >(mpDrawString);
  [draw_string drawAtPoint: NSMakePoint(pos.x(), pos.y())];
  mDrawString = 1;
}

nude::DrawString nuDrawString::create(const nuRect& rect, f32 size)
{
  nuDrawString* p_drawstr = new nuDrawString(rect, size);
  return nude::DrawString(p_drawstr);
}

nude::DrawString nuDrawString::create(const c8* string, f32 size)
{
  nuDrawString* p_drawstr = new nuDrawString(string, size);
  return nude::DrawString(p_drawstr);
}

nude::DrawString nuDrawString::create(const c8* string, const nuRect& rect, f32 size)
{
  nuDrawString* p_drawstr = new nuDrawString(string, rect, size);
  return nude::DrawString(p_drawstr);
}

nude::DrawString nuDrawString::create(const c8* string, const nuSize& margin, f32 size)
{
  nuDrawString* p_drawstr = new nuDrawString(string, margin, size);
  return nude::DrawString(p_drawstr);
}
