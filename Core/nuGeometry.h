/*!
 * \file nuGeometry.h
 * \brief Geometry.
 * \author Nus
 * \date 2011/11/24 23:40
 */

#ifndef __NUGEOMETRY_H__
#define __NUGEOMETRY_H__

#include "nuType.h"

class nuPoint
{
private:
  f32 mOriginX;
  f32 mOriginY;

public:
  nuPoint() {}
  ~nuPoint() {}

  nuPoint(const nuPoint& point)
      : mOriginX(point.mOriginX),
        mOriginY(point.mOriginY)
  {
    // None...
  }

  nuPoint(f32 fx, f32 fy)
      : mOriginX(fx),
        mOriginY(fy)
  {
    // None...
  }

  nuPoint(const f32* fv)
      : mOriginX(fv[0]),
        mOriginY(fv[1])
  {
    // None...
  }

  nuPoint& set(const nuPoint& point) {
    mOriginX = point.mOriginX;
    mOriginY = point.mOriginY;
    return *this;
  }

  nuPoint& set(f32 fx, f32 fy) {
    mOriginX = fx;
    mOriginY = fy;
    return *this;
  }

  nuPoint& set(const f32* fv) {
    mOriginX = fv[0];
    mOriginY = fv[1];
    return *this;
  }

  f32 x(void) const {
    return mOriginX;
  }

  void setX(f32 fx) {
    mOriginX = fx;
  }

  f32 y(void) const {
    return mOriginY;
  }

  void setY(f32 fy) {
    mOriginY = fy;
  }

  nuPoint& operator = (const nuPoint& point) {
    mOriginX = point.mOriginX;
    mOriginY = point.mOriginY;
    return *this;
  }

};



class nuSize
{
private:
  f32 mWidth;
  f32 mHeight;

public:
  nuSize() {}
  ~nuSize() {}

  nuSize(const nuSize& size)
      : mWidth(size.mWidth),
        mHeight(size.mHeight)
  {
    // None...
  }

  nuSize(f32 width, f32 height)
      : mWidth(width),
        mHeight(height)
  {
    // None...
  }

  nuSize(const f32* fv)
      : mWidth(fv[0]),
        mHeight(fv[1])
  {
    // None...
  }

  nuSize& set(const nuSize& size) {
    mWidth = size.mWidth;
    mHeight = size.mHeight;
    return *this;
  }

  nuSize& set(f32 width, f32 height) {
    mWidth = width;
    mHeight = height;
    return *this;
  }

  nuSize& set(const f32* fv) {
    mWidth = fv[0];
    mHeight = fv[1];
    return *this;
  }

  f32 w(void) const {
    return mWidth;
  }

  void setW(f32 width) {
    mWidth = width;
  }

  f32 h(void) const {
    return mHeight;
  }

  void setH(f32 height) {
    mHeight = height;
  }

  nuSize operator + (const nuSize& size) const {
    nuSize ret;
    ret.mWidth = mWidth + size.mWidth;
    ret.mHeight = mHeight + size.mHeight;
    return ret;
  }

  nuSize& operator += (const nuSize& size) {
    mWidth += size.mWidth;
    mHeight += size.mHeight;
    return *this;
  }

  nuSize operator - (const nuSize& size) const {
    nuSize ret;
    ret.mWidth = mWidth - size.mWidth;
    ret.mHeight = mHeight - size.mHeight;
    return ret;
  }

  nuSize& operator -= (const nuSize& size) {
    mWidth -= size.mWidth;
    mHeight -= size.mHeight;
    return *this;
  }

  nuSize& operator = (const nuSize& size) {
    mWidth = size.mWidth;
    mHeight = size.mHeight;
    return *this;
  }

};



class nuRect
{
private:
  nuPoint mOrigin;
  nuSize mSize;

public:
  nuRect() {}
  ~nuRect() {}

  nuRect(const nuRect& rect)
      : mOrigin(rect.mOrigin),
        mSize(rect.mSize)
  {
    // None...
  }

  nuRect(const nuPoint& point, const nuSize& size)
      : mOrigin(point),
        mSize(size)
  {
    // None...
  }

  nuRect& set(const nuRect& rect) {
    mOrigin = rect.mOrigin;
    mSize = rect.mSize;
    return *this;
  }

  nuRect& set(const nuPoint& point, const nuSize& size) {
    mOrigin = point;
    mSize = size;
    return *this;
  }

  const nuPoint& origin(void) const {
    return mOrigin;
  }

  void setOrigin(const nuPoint& point) {
    mOrigin = point;
  }

  const nuSize& size(void) const {
    return mSize;
  }

  void setSize(const nuSize& size) {
    mSize = size;
  }

  nuPoint lt(void) const {
    return mOrigin;
  }

  nuPoint rb(void) const {
    return nuPoint(mOrigin.x() + mSize.w(), mOrigin.y() + mSize.h());
  }

  nuPoint rt(void) const {
    return nuPoint(mOrigin.x() + mSize.w(), mOrigin.y());
  }

  nuPoint lb(void) const {
    return nuPoint(mOrigin.x(), mOrigin.y() + mSize.h());
  }

};

#endif
