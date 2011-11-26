/*!
 * \file nuMath.h
 * \brief Math functions.
 * \author Nus
 * \date 2011/11/25 01:08
 */

#ifndef __NUMATH_H__
#define __NUMATH_H__

#include "nuType.h"

namespace nude
{
  const f32 FloatE = 0.000001f;
}

class nuVec2
{
public:

  union {
    f32 v[2];
    struct {
      f32 x;
      f32 y;
    };
  };

  nuVec2() {}
  ~nuVec2() {}

  nuVec2(const nuVec2& vec)
      : x(vec.x),
        y(vec.y)
  {
    // None...
  }

  nuVec2(f32 fx, f32 fy)
      : x(fx),
        y(fy)
  {
    // None...
  }

  nuVec2(const f32* fv)
      : x(fv[0]),
        y(fv[1])
  {
    // None...
  }

  nuVec2(f32 fs)
      : x(fs),
        y(fs)
  {
    // None...
  }

  nuVec2& operator = (const nuVec2& vec)
  {
    memcpy(v, vec.v, sizeof(v));
    return *this;
  }

  nuVec2& operator = (const f32* fv)
  {
    memcpy(v, fv, sizeof(v));
    return *this;
  }

  nuVec2& operator = (f32 fs)
  {
    x = y = fs;
    return *this;
  }

  nuVec2 operator + (const nuVec2& vec) const {
    nuVec2 ret;
    ret.x = x + vec.x;
    ret.y = y + vec.y;
    return ret;
  }

  nuVec2& operator += (const nuVec2& vec) {
    x += vec.x;
    y += vec.y;
    return *this;
  }

  nuVec2 operator - (const nuVec2& vec) const {
    nuVec2 ret;
    ret.x = x - vec.x;
    ret.y = y - vec.y;
    return ret;
  }

  nuVec2& operator -= (const nuVec2& vec) {
    x -= vec.x;
    y -= vec.y;
    return *this;
  }

  nuVec2 operator * (f32 scale) const {
    nuVec2 ret;
    ret.x = x * scale;
    ret.y = y * scale;
    return ret;
  }

  nuVec2& operator *= (f32 scale) {
    x *= scale;
    y *= scale;
    return *this;
  }

  nuVec2 operator * (const nuVec2& vec) const {
    nuVec2 ret;
    ret.x = x * vec.x;
    ret.y = y * vec.y;
    return ret;
  }

  nuVec2& operator *= (const nuVec2& vec) {
    x *= vec.x;
    y *= vec.y;
    return *this;
  }

  nuVec2 operator / (f32 div) const {
    nuVec2 ret;
    const f32 inv_div = 1.0f / div;
    ret.x = x * inv_div;
    ret.y = y * inv_div;
    return ret;
  }

  nuVec2& operator /= (f32 div) {
    const f32 inv_div = 1.0f / div;
    x *= inv_div;
    y *= inv_div;
    return *this;
  }

  nuVec2 operator / (const nuVec2& vec) const {
    nuVec2 ret;
    ret.x = x / vec.x;
    ret.y = y / vec.y;
    return ret;
  }

  nuVec2& operator /= (const nuVec2& vec) {
    x /= vec.x;
    y /= vec.y;
    return *this;
  }

  f32 operator ^ (const nuVec2& vec) const {
    return x * vec.x + y * vec.y;
  }

  f32 absSq(void) const {
    return x * x + y * y;
  }

  f32 abs(void) const {
    return sqrtf(absSq());
  }

};

class nuVec3
{
public:

  union {
    f32 v[3];
    struct {
      f32 x;
      f32 y;
      f32 z;
    };
  };

  nuVec3() {}
  ~nuVec3() {}

  nuVec3(const nuVec3& vec)
      : x(vec.x),
        y(vec.y),
        z(vec.z)
  {
    // None...
  }

  nuVec3(f32 fx, f32 fy, f32 fz)
      : x(fx),
        y(fy),
        z(fz)
  {
    // None...
  }

  nuVec3(const f32* fv)
      : x(fv[0]),
        y(fv[1]),
        z(fv[2])
  {
    // None...
  }

  nuVec3(const nuVec2& vec)
      : x(vec.x),
        y(vec.y),
        z(0.0f)
  {
    // None...
  }

  nuVec3(f32 fs)
      : x(fs),
        y(fs),
        z(fs)
  {
    // None...
  }

  nuVec3& operator = (const nuVec3& vec) {
    memcpy(v, vec.v, sizeof(v));
    return *this;
  }

  nuVec3& operator = (const f32* fv) {
    memcpy(v, fv, sizeof(v));
    return *this;
  }

  nuVec3& operator = (const nuVec2& vec) {
    x = vec.x;
    y = vec.y;
    z = 0.0f;
    return *this;
  }

  nuVec3& operator = (f32 fs) {
    x = y = z = fs;
    return *this;
  }

  nuVec3 operator + (const nuVec3& vec) const {
    nuVec3 ret;
    ret.x = x + vec.x;
    ret.y = y + vec.y;
    ret.z = z + vec.z;
    return ret;
  }

  nuVec3& operator += (const nuVec3& vec) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
    return *this;
  }

  nuVec3 operator - (const nuVec3& vec) const {
    nuVec3 ret;
    ret.x = x - vec.x;
    ret.y = y - vec.y;
    ret.z = z - vec.z;
    return ret;
  }

  nuVec3& operator -= (const nuVec3& vec) {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    return *this;
  }

  nuVec3 operator * (f32 scale) const {
    nuVec3 ret;
    ret.x = x * scale;
    ret.y = y * scale;
    ret.z = z * scale;
    return ret;
  }

  nuVec3& operator *= (f32 scale) {
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
  }

  nuVec3 operator * (const nuVec3& vec) const {
    nuVec3 ret;
    ret.x = x * vec.x;
    ret.y = y * vec.y;
    ret.z = z * vec.z;
    return ret;
  }

  nuVec3& operator *= (const nuVec3& vec) {
    x *= vec.x;
    y *= vec.y;
    z *= vec.z;
    return *this;
  }

  nuVec3 operator * (const class nuMtx3& mtx) const;

  nuVec3& operator *= (const class nuMtx3& mtx);

  nuVec3 operator / (f32 div) const {
    nuVec3 ret;
    const f32 inv_div = 1.0f / div;
    ret.x = x * inv_div;
    ret.y = y * inv_div;
    ret.z = z * inv_div;
    return ret;
  }

  nuVec3& operator /= (f32 div) {
    const f32 inv_div = 1.0f / div;
    x *= inv_div;
    y *= inv_div;
    z *= inv_div;
    return *this;
  }

  nuVec3 operator / (const nuVec3& vec) const {
    nuVec3 ret;
    ret.x = x / vec.x;
    ret.y = y / vec.y;
    ret.z = z / vec.z;
    return ret;
  }

  nuVec3& operator /= (const nuVec3& vec) {
    x /= vec.x;
    y /= vec.y;
    z /= vec.z;
    return *this;
  }

  f32 operator ^ (const nuVec3& vec) const {
    return x * vec.x + y * vec.y + z * vec.z;
  }

  nuVec3 operator & (const nuVec3& vec) const {
    nuVec3 ret;
    ret.x = y * vec.z - z * vec.y;
    ret.y = z * vec.x - x * vec.z;
    ret.z = x * vec.y - y * vec.x;
    return ret;
  }

  nuVec3& operator &= (const nuVec3& vec) {
    nuVec3 temp = *this;
    x = temp.y * vec.z - temp.z * vec.y;
    y = temp.z * vec.x - temp.x * vec.z;
    z = temp.x * vec.y - temp.y * vec.x;
    return *this;
  }

  f32 absSq(void) const {
    return x * x + y * y + z * z;
  }

  f32 abs(void) const {
    return sqrtf(absSq());
  }

};

class nuVec4
{
public:

  union {
    f32 v[4];
    struct {
      f32 x;
      f32 y;
      f32 z;
      f32 w;
    };
  };

  nuVec4() {}
  ~nuVec4() {}

  nuVec4(const nuVec4& vec)
      : x(vec.x),
        y(vec.y),
        z(vec.z),
        w(vec.w)
  {
    // None...
  }

  nuVec4(f32 fx, f32 fy, f32 fz, f32 fw)
      : x(fx),
        y(fy),
        z(fz),
        w(fw)
  {
    // None...
  }

  nuVec4(const f32* fv)
      : x(fv[0]),
        y(fv[1]),
        z(fv[2]),
        w(fv[3])
  {
    // None...
  }

  nuVec4(const nuVec3& vec)
      : x(vec.x),
        y(vec.y),
        z(vec.z),
        w(1.0f)
  {
    // None...
  }

  nuVec4(const nuVec2& vec)
      : x(vec.x),
        y(vec.y),
        z(0.0f),
        w(1.0f)
  {
    // None...
  }

  nuVec4(f32 fs)
      : x(fs),
        y(fs),
        z(fs),
        w(fs)
  {
    // None...
  }

  nuVec4& operator = (const nuVec4& vec) {
    memcpy(v, vec.v, sizeof(v));
    return *this;
  }

  nuVec4& operator = (const f32* fv) {
    memcpy(v, fv, sizeof(v));
    return *this;
  }

  nuVec4& operator = (const nuVec3& vec) {
    memcpy(v, vec.v, sizeof(vec.v));
    w = 1.0f;
    return *this;
  }

  nuVec4& operator = (const nuVec2& vec) {
    memcpy(v, vec.v, sizeof(vec.v));
    z = 0.0f;
    w = 1.0f;
    return *this;
  }

  nuVec4& operator = (f32 fs) {
    x = y = z = w = fs;
    return *this;
  }

  nuVec4 operator + (const nuVec4& vec) const {
    nuVec4 ret;
    ret.x = x + vec.x;
    ret.y = y + vec.y;
    ret.z = z + vec.z;
    ret.w = w + vec.w;
    return ret;
  }

  nuVec4& operator += (const nuVec4& vec) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
    w += vec.w;
    return *this;
  }

  nuVec4 operator - (const nuVec4& vec) const {
    nuVec4 ret;
    ret.x = x - vec.x;
    ret.y = y - vec.y;
    ret.z = z - vec.z;
    ret.w = w - vec.w;
    return ret;
  }

  nuVec4& operator -= (const nuVec4& vec) {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    w -= vec.w;
    return *this;
  }

  nuVec4 operator * (f32 scale) const {
    nuVec4 ret;
    ret.x = x * scale;
    ret.y = y * scale;
    ret.z = z * scale;
    ret.w = w * scale;
    return ret;
  }

  nuVec4& operator *= (f32 scale) {
    x *= scale;
    y *= scale;
    z *= scale;
    w *= scale;
    return *this;
  }

  nuVec4 operator * (const nuVec4& vec) const {
    nuVec4 ret;
    ret.x = x * vec.x;
    ret.y = y * vec.y;
    ret.z = z * vec.z;
    ret.w = w * vec.w;
    return ret;
  }

  nuVec4& operator *= (const nuVec4& vec) {
    x *= vec.x;
    y *= vec.y;
    z *= vec.z;
    w *= vec.w;
    return *this;
  }

  nuVec4 operator * (const class nuMtx4& mtx) const;

  nuVec4& operator *= (const class nuMtx4& mtx);

  nuVec4 operator / (f32 div) const {
    nuVec4 ret;
    const f32 inv_div = 1.0f / div;
    ret.x = x * inv_div;
    ret.y = y * inv_div;
    ret.z = z * inv_div;
    ret.w = w * inv_div;
    return ret;
  }

  nuVec4& operator /= (f32 div) {
    const f32 inv_div = 1.0f / div;
    x *= inv_div;
    y *= inv_div;
    z *= inv_div;
    w *= inv_div;
    return *this;
  }

  nuVec4 operator / (const nuVec4& vec) const {
    nuVec4 ret;
    ret.x = x / vec.x;
    ret.y = y / vec.y;
    ret.z = z / vec.z;
    ret.w = w / vec.w;
    return ret;
  }

  nuVec4& operator /= (const nuVec4& vec) {
    x /= vec.x;
    y /= vec.y;
    z /= vec.z;
    w /= vec.w;
    return *this;
  }

  f32 operator ^ (const nuVec4& vec) const {
    return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
  }

  f32 absSq(void) const {
    return x * x + y * y + z * z + w * w;
  }

  f32 abs(void) const {
    return sqrtf(absSq());
  }

};

class nuMtx3
{
public:
  union {
    f32 v[9];
    f32 m[3][3];
    struct {
      f32 m00, m01, m02;
      f32 m10, m11, m12;
      f32 m20, m21, m22;
    };
  };

  nuMtx3() {}
  ~nuMtx3() {}

  nuMtx3(const nuMtx3& mtx) {
    memcpy(v, mtx.v, sizeof(v));
  }

  nuMtx3(const f32* fv) {
    memcpy(v, fv, sizeof(v));
  }

  nuMtx3(f32 f_00, f32 f_01, f32 f_02,
         f32 f_10, f32 f_11, f32 f_12,
         f32 f_20, f32 f_21, f32 f_22)
      : m00(f_00), m01(f_01), m02(f_02),
        m10(f_10), m11(f_11), m12(f_12),
        m20(f_20), m21(f_21), m22(f_22)
  {
    // None...
  }

  nuMtx3(f32 fs)
      : m00(fs), m01(fs), m02(fs),
        m10(fs), m11(fs), m12(fs),
        m20(fs), m21(fs), m22(fs)
  {
    // None...
  }

  nuMtx3& operator = (const nuMtx3& mtx) {
    memcpy(v, mtx.v, sizeof(v));
    return *this;
  }

  nuMtx3& operator = (const f32* fv) {
    memcpy(v, fv, sizeof(v));
    return *this;
  }

  nuMtx3& operator = (f32 fs) {
    m00 = m01 = m02 = m00 = m01 = m02 = m00 = m01 = m02 = fs;
    return *this;
  }

  nuMtx3 operator + (const nuMtx3& mtx) const {
    nuMtx3 ret;
    ret.v[0] = v[0] + mtx.v[0];
    ret.v[1] = v[1] + mtx.v[1];
    ret.v[2] = v[2] + mtx.v[2];
    ret.v[3] = v[3] + mtx.v[3];
    ret.v[4] = v[4] + mtx.v[4];
    ret.v[5] = v[5] + mtx.v[5];
    ret.v[6] = v[6] + mtx.v[6];
    ret.v[7] = v[7] + mtx.v[7];
    ret.v[8] = v[8] + mtx.v[8];
    return ret;
  }

  nuMtx3& operator += (const nuMtx3& mtx) {
    v[0] += mtx.v[0];
    v[1] += mtx.v[1];
    v[2] += mtx.v[2];
    v[3] += mtx.v[3];
    v[4] += mtx.v[4];
    v[5] += mtx.v[5];
    v[6] += mtx.v[6];
    v[7] += mtx.v[7];
    v[8] += mtx.v[8];
    return *this;
  }

  nuMtx3 operator - (const nuMtx3& mtx) const {
    nuMtx3 ret;
    ret.v[0] = v[0] - mtx.v[0];
    ret.v[1] = v[1] - mtx.v[1];
    ret.v[2] = v[2] - mtx.v[2];
    ret.v[3] = v[3] - mtx.v[3];
    ret.v[4] = v[4] - mtx.v[4];
    ret.v[5] = v[5] - mtx.v[5];
    ret.v[6] = v[6] - mtx.v[6];
    ret.v[7] = v[7] - mtx.v[7];
    ret.v[8] = v[8] - mtx.v[8];
    return ret;
  }

  nuMtx3& operator -= (const nuMtx3& mtx) {
    v[0] -= mtx.v[0];
    v[1] -= mtx.v[1];
    v[2] -= mtx.v[2];
    v[3] -= mtx.v[3];
    v[4] -= mtx.v[4];
    v[5] -= mtx.v[5];
    v[6] -= mtx.v[6];
    v[7] -= mtx.v[7];
    v[8] -= mtx.v[8];
    return *this;
  }

  nuMtx3 operator * (const nuMtx3& mtx) const {
    nuMtx3 ret;
    ret.m00 = m00 * mtx.m00 + m01 * mtx.m10 + m02 * mtx.m20;
    ret.m01 = m00 * mtx.m01 + m01 * mtx.m11 + m02 * mtx.m21;
    ret.m02 = m00 * mtx.m02 + m01 * mtx.m12 + m02 * mtx.m22;
    ret.m10 = m10 * mtx.m00 + m11 * mtx.m10 + m12 * mtx.m20;
    ret.m11 = m10 * mtx.m01 + m11 * mtx.m11 + m12 * mtx.m21;
    ret.m12 = m10 * mtx.m02 + m11 * mtx.m12 + m12 * mtx.m22;
    ret.m20 = m20 * mtx.m00 + m21 * mtx.m10 + m22 * mtx.m20;
    ret.m21 = m20 * mtx.m01 + m21 * mtx.m11 + m22 * mtx.m21;
    ret.m22 = m20 * mtx.m02 + m21 * mtx.m12 + m22 * mtx.m22;
    return ret;
  }

  nuMtx3& operator *= (const nuMtx3& mtx) {
    nuMtx3 temp;
    temp.m00 = m00 * mtx.m00 + m01 * mtx.m10 + m02 * mtx.m20;
    temp.m01 = m00 * mtx.m01 + m01 * mtx.m11 + m02 * mtx.m21;
    temp.m02 = m00 * mtx.m02 + m01 * mtx.m12 + m02 * mtx.m22;
    temp.m10 = m10 * mtx.m00 + m11 * mtx.m10 + m12 * mtx.m20;
    temp.m11 = m10 * mtx.m01 + m11 * mtx.m11 + m12 * mtx.m21;
    temp.m12 = m10 * mtx.m02 + m11 * mtx.m12 + m12 * mtx.m22;
    temp.m20 = m20 * mtx.m00 + m21 * mtx.m10 + m22 * mtx.m20;
    temp.m21 = m20 * mtx.m01 + m21 * mtx.m11 + m22 * mtx.m21;
    temp.m22 = m20 * mtx.m02 + m21 * mtx.m12 + m22 * mtx.m22;
    *this = temp;
    return *this;
  }

  nuMtx3& transpose(void) {
    std::swap(m01, m10);
    std::swap(m02, m20);
    std::swap(m12, m21);
    return *this;
  }

  f32 determinant(void) const {
    f32 det;
    det = m[0][0] * minorDeterminant(0, 0, *this);
    det -= m[0][1] * minorDeterminant(0, 1, *this);
    det += m[0][2] * minorDeterminant(0, 2, *this);
    return det;
  }

  nuMtx3& inverse(void) {
    f32 det = determinant();

    if(fabsf(det) > nude::FloatE) {
      f32 inv_det = 1.0f / det;
      nuMtx3 temp = *this;

      m[0][0] = minorDeterminant(0, 0, temp) * inv_det;
      m[0][1] = -minorDeterminant(1, 0, temp) * inv_det;
      m[0][2] = minorDeterminant(2, 0, temp) * inv_det;
      m[1][0] = -minorDeterminant(0, 1, temp) * inv_det;
      m[1][1] = minorDeterminant(1, 1, temp) * inv_det;
      m[1][2] = -minorDeterminant(2, 1, temp) * inv_det;
      m[2][0] = minorDeterminant(0, 2, temp) * inv_det;
      m[2][1] = -minorDeterminant(1, 2, temp) * inv_det;
      m[2][2] = minorDeterminant(2, 2, temp) * inv_det;
    }

    return *this;
  }

  static const nuMtx3 Identity;

private:
  f32 minorDeterminant(ui32 rhy, ui32 cwx, const nuMtx3& mtx) const {
    ui32 x1 = cwx == 0 ? 1 : 0;
    ui32 x2 = cwx == 2 ? 1 : 2;
    ui32 y1 = rhy == 0 ? 1 : 0;
    ui32 y2 = rhy == 2 ? 1 : 2;
    return mtx.m[y1][x1] *  mtx.m[y2][x2] - mtx.m[y1][x2] * mtx.m[y2][x1];
  }

};

class nuMtx4
{
public:
  union {
    f32 v[16];
    f32 m[4][4];
    struct {
      f32 m00, m01, m02, m03;
      f32 m10, m11, m12, m13;
      f32 m20, m21, m22, m23;
      f32 m30, m31, m32, m33;
    };
  };

  nuMtx4() {}
  ~nuMtx4() {}

  nuMtx4(const nuMtx4& mtx) {
    memcpy(v, mtx.v, sizeof(v));
  }

  nuMtx4(const f32* fv) {
    memcpy(v, fv, sizeof(v));
  }

  nuMtx4(f32 f_00, f32 f_01, f32 f_02, f32 f_03,
         f32 f_10, f32 f_11, f32 f_12, f32 f_13,
         f32 f_20, f32 f_21, f32 f_22, f32 f_23,
         f32 f_30, f32 f_31, f32 f_32, f32 f_33)
      : m00(f_00), m01(f_01), m02(f_02), m03(f_03),
        m10(f_10), m11(f_11), m12(f_12), m13(f_13),
        m20(f_20), m21(f_21), m22(f_22), m23(f_23),
        m30(f_30), m31(f_31), m32(f_32), m33(f_33)
  {
    // None...
  }

  nuMtx4(const nuMtx3& mtx) {
    m00 = mtx.m00; m01 = mtx.m01; m02 = mtx.m02; m03 = 0.0f;
    m10 = mtx.m10; m11 = mtx.m11; m12 = mtx.m12; m13 = 0.0f;
    m20 = mtx.m20; m21 = mtx.m21; m22 = mtx.m22; m23 = 0.0f;
    m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
  }

  nuMtx4(f32 fs)
      : m00(fs), m01(fs), m02(fs), m03(fs),
        m10(fs), m11(fs), m12(fs), m13(fs),
        m20(fs), m21(fs), m22(fs), m23(fs),
        m30(fs), m31(fs), m32(fs), m33(fs)
  {
    // None...
  }

  nuMtx4& operator = (const nuMtx4& mtx) {
    memcpy(v, mtx.v, sizeof(v));
    return *this;
  }

  nuMtx4& operator = (const f32* fv) {
    memcpy(v, fv, sizeof(v));
    return *this;
  }

  nuMtx4& operator = (const nuMtx3& mtx) {
    m00 = mtx.m00; m01 = mtx.m01; m02 = mtx.m02; m03 = 0.0f;
    m10 = mtx.m10; m11 = mtx.m11; m12 = mtx.m12; m13 = 0.0f;
    m20 = mtx.m20; m21 = mtx.m21; m22 = mtx.m22; m23 = 0.0f;
    m30 = 0.0f; m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
    return *this;
  }

  nuMtx4& operator = (f32 fs)
  {
    m00 = m01 = m02 = m03 =
    m10 = m11 = m12 = m13 =
    m20 = m21 = m22 = m23 =
    m30 = m31 = m32 = m33 = fs;
    return *this;
  }

  nuMtx4 operator + (const nuMtx4& mtx) const {
    nuMtx4 ret;
    ret.v[0] = v[0] + mtx.v[0];
    ret.v[1] = v[1] + mtx.v[1];
    ret.v[2] = v[2] + mtx.v[2];
    ret.v[3] = v[3] + mtx.v[3];
    ret.v[4] = v[4] + mtx.v[4];
    ret.v[5] = v[5] + mtx.v[5];
    ret.v[6] = v[6] + mtx.v[6];
    ret.v[7] = v[7] + mtx.v[7];
    ret.v[8] = v[8] + mtx.v[8];
    ret.v[9] = v[9] + mtx.v[9];
    ret.v[10] = v[10] + mtx.v[10];
    ret.v[11] = v[11] + mtx.v[11];
    ret.v[12] = v[12] + mtx.v[12];
    ret.v[13] = v[13] + mtx.v[13];
    ret.v[14] = v[14] + mtx.v[14];
    ret.v[15] = v[15] + mtx.v[15];
    return ret;
  }

  nuMtx4& operator += (const nuMtx4& mtx) {
    v[0] += mtx.v[0];
    v[1] += mtx.v[1];
    v[2] += mtx.v[2];
    v[3] += mtx.v[3];
    v[4] += mtx.v[4];
    v[5] += mtx.v[5];
    v[6] += mtx.v[6];
    v[7] += mtx.v[7];
    v[8] += mtx.v[8];
    v[9] += mtx.v[9];
    v[10] += mtx.v[10];
    v[11] += mtx.v[11];
    v[12] += mtx.v[12];
    v[13] += mtx.v[13];
    v[14] += mtx.v[14];
    v[15] += mtx.v[15];
    return *this;
  }

  nuMtx4 operator - (const nuMtx4& mtx) const {
    nuMtx4 ret;
    ret.v[0] = v[0] - mtx.v[0];
    ret.v[1] = v[1] - mtx.v[1];
    ret.v[2] = v[2] - mtx.v[2];
    ret.v[3] = v[3] - mtx.v[3];
    ret.v[4] = v[4] - mtx.v[4];
    ret.v[5] = v[5] - mtx.v[5];
    ret.v[6] = v[6] - mtx.v[6];
    ret.v[7] = v[7] - mtx.v[7];
    ret.v[8] = v[8] - mtx.v[8];
    ret.v[9] = v[9] - mtx.v[9];
    ret.v[10] = v[10] - mtx.v[10];
    ret.v[11] = v[11] - mtx.v[11];
    ret.v[12] = v[12] - mtx.v[12];
    ret.v[13] = v[13] - mtx.v[13];
    ret.v[14] = v[14] - mtx.v[14];
    ret.v[15] = v[15] - mtx.v[15];
    return ret;
  }

  nuMtx4& operator -= (const nuMtx4& mtx) {
    v[0] -= mtx.v[0];
    v[1] -= mtx.v[1];
    v[2] -= mtx.v[2];
    v[3] -= mtx.v[3];
    v[4] -= mtx.v[4];
    v[5] -= mtx.v[5];
    v[6] -= mtx.v[6];
    v[7] -= mtx.v[7];
    v[8] -= mtx.v[8];
    v[9] -= mtx.v[9];
    v[10] -= mtx.v[10];
    v[11] -= mtx.v[11];
    v[12] -= mtx.v[12];
    v[13] -= mtx.v[13];
    v[14] -= mtx.v[14];
    v[15] -= mtx.v[15];
    return *this;
  }

  nuMtx4 operator * (const nuMtx4& mtx) const {
    nuMtx4 ret;

    ret.m00 = m00 * mtx.m00 + m01 * mtx.m10 + m02 * mtx.m20 + m03 * mtx.m30;
    ret.m01 = m00 * mtx.m01 + m01 * mtx.m11 + m02 * mtx.m21 + m03 * mtx.m31;
    ret.m02 = m00 * mtx.m02 + m01 * mtx.m12 + m02 * mtx.m22 + m03 * mtx.m32;
    ret.m03 = m00 * mtx.m03 + m01 * mtx.m13 + m02 * mtx.m23 + m03 * mtx.m33;

    ret.m10 = m10 * mtx.m00 + m11 * mtx.m10 + m12 * mtx.m20 + m13 * mtx.m30;
    ret.m11 = m10 * mtx.m01 + m11 * mtx.m11 + m12 * mtx.m21 + m13 * mtx.m31;
    ret.m12 = m10 * mtx.m02 + m11 * mtx.m12 + m12 * mtx.m22 + m13 * mtx.m32;
    ret.m13 = m10 * mtx.m03 + m11 * mtx.m13 + m12 * mtx.m23 + m13 * mtx.m33;

    ret.m20 = m20 * mtx.m00 + m21 * mtx.m10 + m22 * mtx.m20 + m23 * mtx.m30;
    ret.m21 = m20 * mtx.m01 + m21 * mtx.m11 + m22 * mtx.m21 + m23 * mtx.m31;
    ret.m22 = m20 * mtx.m02 + m21 * mtx.m12 + m22 * mtx.m22 + m23 * mtx.m32;
    ret.m23 = m20 * mtx.m03 + m21 * mtx.m13 + m22 * mtx.m23 + m23 * mtx.m33;

    ret.m30 = m30 * mtx.m00 + m31 * mtx.m10 + m32 * mtx.m20 + m33 * mtx.m30;
    ret.m31 = m30 * mtx.m01 + m31 * mtx.m11 + m32 * mtx.m21 + m33 * mtx.m31;
    ret.m32 = m30 * mtx.m02 + m31 * mtx.m12 + m32 * mtx.m22 + m33 * mtx.m32;
    ret.m33 = m30 * mtx.m03 + m31 * mtx.m13 + m32 * mtx.m23 + m33 * mtx.m33;

    return ret;
  }

  nuMtx4& operator *= (const nuMtx4& mtx) {
    nuMtx4 temp;

    temp.m00 = m00 * mtx.m00 + m01 * mtx.m10 + m02 * mtx.m20 + m03 * mtx.m30;
    temp.m01 = m00 * mtx.m01 + m01 * mtx.m11 + m02 * mtx.m21 + m03 * mtx.m31;
    temp.m02 = m00 * mtx.m02 + m01 * mtx.m12 + m02 * mtx.m22 + m03 * mtx.m32;
    temp.m03 = m00 * mtx.m03 + m01 * mtx.m13 + m02 * mtx.m23 + m03 * mtx.m33;

    temp.m10 = m10 * mtx.m00 + m11 * mtx.m10 + m12 * mtx.m20 + m13 * mtx.m30;
    temp.m11 = m10 * mtx.m01 + m11 * mtx.m11 + m12 * mtx.m21 + m13 * mtx.m31;
    temp.m12 = m10 * mtx.m02 + m11 * mtx.m12 + m12 * mtx.m22 + m13 * mtx.m32;
    temp.m13 = m10 * mtx.m03 + m11 * mtx.m13 + m12 * mtx.m23 + m13 * mtx.m33;

    temp.m20 = m20 * mtx.m00 + m21 * mtx.m10 + m22 * mtx.m20 + m23 * mtx.m30;
    temp.m21 = m20 * mtx.m01 + m21 * mtx.m11 + m22 * mtx.m21 + m23 * mtx.m31;
    temp.m22 = m20 * mtx.m02 + m21 * mtx.m12 + m22 * mtx.m22 + m23 * mtx.m32;
    temp.m23 = m20 * mtx.m03 + m21 * mtx.m13 + m22 * mtx.m23 + m23 * mtx.m33;

    temp.m30 = m30 * mtx.m00 + m31 * mtx.m10 + m32 * mtx.m20 + m33 * mtx.m30;
    temp.m31 = m30 * mtx.m01 + m31 * mtx.m11 + m32 * mtx.m21 + m33 * mtx.m31;
    temp.m32 = m30 * mtx.m02 + m31 * mtx.m12 + m32 * mtx.m22 + m33 * mtx.m32;
    temp.m33 = m30 * mtx.m03 + m31 * mtx.m13 + m32 * mtx.m23 + m33 * mtx.m33;

    *this = temp;
    return *this;
  }

  nuMtx4& transpose(void) {
    std::swap(m01, m10);
    std::swap(m02, m20);
    std::swap(m03, m30);
    std::swap(m12, m21);
    std::swap(m13, m31);
    std::swap(m23, m32);
    return *this;
  }

  f32 determinant(void) const {
    f32 a0 = v[0] * v[5] - v[1] * v[4];
    f32 a1 = v[0] * v[6] - v[2] * v[4];
    f32 a2 = v[0] * v[7] - v[3] * v[4];
    f32 a3 = v[1] * v[6] - v[2] * v[5];
    f32 a4 = v[1] * v[7] - v[3] * v[5];
    f32 a5 = v[2] * v[7] - v[3] * v[6];

    f32 b0 = v[8] * v[13] - v[ 9] * v[12];
    f32 b1 = v[8] * v[14] - v[10] * v[12];
    f32 b2 = v[8] * v[15] - v[11] * v[12];
    f32 b3 = v[9] * v[14] - v[10] * v[13];
    f32 b4 = v[9] * v[15] - v[11] * v[13];
    f32 b5 = v[10] * v[15] - v[11] * v[14];

    return a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
  }

  nuMtx4& inverse(void) {
    f32 det;

    f32 a0 = v[0] * v[5] - v[1] * v[4];
    f32 a1 = v[0] * v[6] - v[2] * v[4];
    f32 a2 = v[0] * v[7] - v[3] * v[4];
    f32 a3 = v[1] * v[6] - v[2] * v[5];
    f32 a4 = v[1] * v[7] - v[3] * v[5];
    f32 a5 = v[2] * v[7] - v[3] * v[6];

    f32 b0 = v[8] * v[13] - v[ 9] * v[12];
    f32 b1 = v[8] * v[14] - v[10] * v[12];
    f32 b2 = v[8] * v[15] - v[11] * v[12];
    f32 b3 = v[9] * v[14] - v[10] * v[13];
    f32 b4 = v[9] * v[15] - v[11] * v[13];
    f32 b5 = v[10] * v[15] - v[11] * v[14];

    det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

    if(fabsf(det) > nude::FloatE) {
      f32 inv_det = 1.0f / det;
      nuMtx4 temp;

      temp.v[0] = inv_det * (v[5] * b5 - v[6] * b4 + v[7] * b3);
      temp.v[1] = inv_det * (-v[1] * b5 + v[2] * b4 - v[3] * b3);
      temp.v[2] = inv_det * (v[13] * a5 - v[14] * a4 + v[15] * a3);
      temp.v[3] = inv_det * (-v[9] * a5 + v[10] * a4 - v[11] * a3);

      temp.v[4] = inv_det * (-v[4] * b5 + v[6] * b2 - v[7] * b1);
      temp.v[5] = inv_det * (v[0] * b5 - v[2] * b2 + v[3] * b1);
      temp.v[6] = inv_det * (-v[12] * a5 + v[14] * a2 - v[15] * a1);
      temp.v[7] = inv_det * (v[8] * a5 - v[10] * a2 + v[11] * a1);

      temp.v[8] = inv_det * (v[4] * b4 - v[5] * b2 + v[7] * b0);
      temp.v[9] = inv_det * (-v[0] * b4 + v[1] * b2 - v[3] * b0);
      temp.v[10] = inv_det * (v[12] * a4 - v[13] * a2 + v[15] * a0);
      temp.v[11] = inv_det * (-v[8] * a4 + v[9] * a2 - v[11] * a0);

      temp.v[12] = inv_det * (-v[4] * b3 + v[5] * b1 - v[6] * b0);
      temp.v[13] = inv_det * (v[0] * b3 - v[1] * b1 + v[2] * b0);
      temp.v[14] = inv_det * (-v[12] * a3 + v[13] * a1 - v[14] * a0);
      temp.v[15] = inv_det * (v[8] * a3 - v[9] * a1 + v[10] * a0);

      *this = temp;
    }

    return *this;
  }

  static const nuMtx4 Identity;

};

#endif
