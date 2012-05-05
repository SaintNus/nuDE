/*!
 * \file nuColor.h
 * \brief Color class.
 * \author Nus
 * \date 2011/11/25 00:26
 */

#ifndef __NUCOLOR_H__
#define __NUCOLOR_H__

#include "nuType.h"

class nuColor
{
public:
  union {
    ui32 rgba;
    struct {
      ui32 a: 8;
      ui32 b: 8;
      ui32 g: 8;
      ui32 r: 8;
    };
  };

  nuColor() {}
  ~nuColor() {}

  nuColor(const nuColor& color)
      : rgba(color.rgba)
  {
    // None...
  }

  nuColor(ui32 color)
      : rgba(color)
  {
    // None...
  }

  nuColor(ui8 ir, ui8 ig, ui8 ib, ui8 ia)
      : r(ir),
        g(ig),
        b(ib),
        a(ia)
  {
    // None...
  }

  nuColor& operator = (const nuColor& color) {
    rgba = color.rgba;
    return *this;
  }

  nuColor& set(const nuColor& color) {
    rgba = color.rgba;
    return *this;
  }

  nuColor& set(ui32 color) {
    rgba = color;
    return *this;
  }

  nuColor& set(ui8 ir, ui8 ig, ui8 ib, ui8 ia) {
    r = ir;
    g = ig;
    b = ib;
    a = ia;
    return *this;
  }

  bool operator == (const nuColor& color) const {
    return rgba == color.rgba;
  }

  bool operator != (const nuColor& color) const {
    return rgba != color.rgba;
  }

  f32 fr(void) const {
    return static_cast< f32 >(r) / 255.0f;
  }

  f32 fg(void) const {
    return static_cast< f32 >(g) / 255.0f;
  }

  f32 fb(void) const {
    return static_cast< f32 >(b) / 255.0f;
  }

  f32 fa(void) const {
    return static_cast< f32 >(a) / 255.0f;
  }

  nuColor operator + (const nuColor& color) const {
    nuColor ret;
    ret.r = r + color.r;
    ret.g = g + color.g;
    ret.b = b + color.b;
    ret.a = a + color.a;
    return ret;
  }

  nuColor& operator += (const nuColor& color) {
    r += color.r;
    g += color.g;
    b += color.b;
    a += color.a;
    return *this;
  }

  nuColor operator - (const nuColor& color) const {
    nuColor ret;
    ret.r = r - color.r;
    ret.g = g - color.g;
    ret.b = b - color.b;
    ret.a = a - color.a;
    return ret;
  }

  nuColor& operator -= (const nuColor& color) {
    r -= color.r;
    g -= color.g;
    b -= color.b;
    a -= color.a;
    return *this;
  }

  nuColor operator + (ui8 adder) const {
    nuColor ret;
    ret.r = r + adder;
    ret.g = g + adder;
    ret.b = b + adder;
    ret.a = a + adder;
    return ret;
  }

  nuColor& operator += (ui8 adder) {
    r += adder;
    g += adder;
    b += adder;
    a += adder;
    return *this;
  }

  nuColor operator - (ui8 adder) const {
    nuColor ret;
    ret.r = r - adder;
    ret.g = g - adder;
    ret.b = b - adder;
    ret.a = a - adder;
    return ret;
  }

  nuColor& operator -= (ui8 adder) {
    r -= adder;
    g -= adder;
    b -= adder;
    a -= adder;
    return *this;
  }

  nuColor operator * (f32 scale) const {
    nuColor ret;
    ret.r = static_cast< ui8 >(fr() * scale);
    ret.g = static_cast< ui8 >(fg() * scale);
    ret.b = static_cast< ui8 >(fb() * scale);
    ret.a = static_cast< ui8 >(fa() * scale);
    return ret;
  }

  nuColor& operator *= (f32 scale) {
    r = static_cast< ui8 >(fr() * scale);
    g = static_cast< ui8 >(fg() * scale);
    b = static_cast< ui8 >(fb() * scale);
    a = static_cast< ui8 >(fa() * scale);
    return *this;
  }

  nuColor operator / (f32 scale) const {
    nuColor ret;
    f32 inv_scale = 1.0f / scale;
    ret.r = static_cast< ui8 >(fr() * inv_scale);
    ret.g = static_cast< ui8 >(fg() * inv_scale);
    ret.b = static_cast< ui8 >(fb() * inv_scale);
    ret.a = static_cast< ui8 >(fa() * inv_scale);
    return ret;
  }

  nuColor& operator /= (f32 scale) {
    f32 inv_scale = 1.0f / scale;
    r = static_cast< ui8 >(fr() * inv_scale);
    g = static_cast< ui8 >(fg() * inv_scale);
    b = static_cast< ui8 >(fb() * inv_scale);
    a = static_cast< ui8 >(fa() * inv_scale);
    return *this;
  }

  nuColor operator * (const nuColor& color) const {
    nuColor ret;
    ret.r = static_cast< ui8 >(fr() * color.fr());
    ret.g = static_cast< ui8 >(fg() * color.fg());
    ret.b = static_cast< ui8 >(fb() * color.fb());
    ret.a = static_cast< ui8 >(fa() * color.fa());
    return ret;
  }

  nuColor& operator *= (const nuColor& color) {
    r = static_cast< ui8 >(fr() * color.fr());
    g = static_cast< ui8 >(fg() * color.fg());
    b = static_cast< ui8 >(fb() * color.fb());
    a = static_cast< ui8 >(fa() * color.fa());
    return *this;
  }

  nuColor operator / (const nuColor& color) const {
    nuColor ret;
    ret.r = static_cast< ui8 >(fr() / color.fr());
    ret.g = static_cast< ui8 >(fg() / color.fg());
    ret.b = static_cast< ui8 >(fb() / color.fb());
    ret.a = static_cast< ui8 >(fa() / color.fa());
    return ret;
  }

  nuColor& operator /= (const nuColor& color) {
    r = static_cast< ui8 >(fr() / color.fr());
    g = static_cast< ui8 >(fg() / color.fg());
    b = static_cast< ui8 >(fb() / color.fb());
    a = static_cast< ui8 >(fa() / color.fa());
    return *this;
  }

  static const nuColor Red;
  static const nuColor Green;
  static const nuColor Blue;

  static const nuColor Yellow;
  static const nuColor Cyan;
  static const nuColor Magenta;

  static const nuColor Black;
  static const nuColor White;
  static const nuColor Gray;

  static const nuColor DarkRed;
  static const nuColor DarkGreen;
  static const nuColor DarkBlue;

  static const nuColor DarkYellow;
  static const nuColor DarkCyan;
  static const nuColor DarkMagenta;

};

#endif
