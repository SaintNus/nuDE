/*!
 * \file nuGContext.h
 * \brief Graphic context.
 * \author Nus
 * \date 2011/10/30 21:49
 */

#ifndef __NUGCONTEXT_H__
#define __NUGCONTEXT_H__

/*!
 * \class nuGContext
 * \brief Graphic context.
 */
class nuGContext
{
  friend class nuRenderGL;

public:
  enum TYPE {
    CLEAR = 0,
  };
    
  enum CLEAR_BIT {
    COLOR = GL_COLOR_BUFFER_BIT,
    DEPTH = GL_DEPTH_BUFFER_BIT,
    STENCIL = GL_STENCIL_BUFFER_BIT,
  };

private:
  template< class T >
  struct DrawCmd {
    TYPE type;
    T data;
  };

  struct Clear {
    nuColor clear_color;
    f32 depth_value;
    ui32 clear_bit;
  };

  typedef DrawCmd< Clear > ClearCmd;
  i64 mFrameID;

  nuGContext();
  ~nuGContext();

public:
  void clear(ui32 clear_bit, const nuColor& color, f32 depth);

};

#endif
