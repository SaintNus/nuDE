/*!
 * \file nuGraphics.h
 * \brief Graphics.
 * \author Nus
 * \date 2012/05/03 18:45
 */

#ifndef __NUGRAPHICS_H__
#define __NUGRAPHICS_H__

namespace nude {

  enum PASS {
    PASS_OPAQUE = 0,
    PASS_TRANSPARENCY,
    PASS_DRAW_2D,
  };

  enum PRIMITIVE_MODE {
    POINTS = 0,

    LINE_STRIP,
    LINE_LOOP,
    LINE_LINES,

    TRIANGLE_STRIP,
    TRIANGLE_FAN,
    TRIANGLES,
    
    LINES_ADJACENCY,
    LINE_STRIP_ADJACENCY,
    TRIANGLES_ADJACENCY,
    TRIANGLE_STRIP_ADJACENCY,
  };

}

#endif
