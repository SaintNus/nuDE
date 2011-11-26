/*!
 * \file nuMath.cpp
 * \brief Math functions.
 * \author Nus
 * \date 2011/11/25 01:08
 */

#include "nuMath.h"

const nuMtx3 nuMtx3::Identity(1.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 1.0f);

const nuMtx4 nuMtx4::Identity(1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 1.0f);

nuVec3 nuVec3::operator * (const class nuMtx3& mtx) const
{
  nuVec3 ret;
  ret.x = x * mtx.m00 + y * mtx.m10 + z * mtx.m20;
  ret.y = x * mtx.m01 + y * mtx.m11 + z * mtx.m21;
  ret.z = x * mtx.m02 + y * mtx.m12 + z * mtx.m22;
  return ret;
}

nuVec3& nuVec3::operator *= (const class nuMtx3& mtx)
{
  nuVec3 temp = *this;
  x = temp.x * mtx.m00 + temp.y * mtx.m10 + temp.z * mtx.m20;
  y = temp.x * mtx.m01 + temp.y * mtx.m11 + temp.z * mtx.m21;
  z = temp.x * mtx.m02 + temp.y * mtx.m12 + temp.z * mtx.m22;
  return *this;
}

nuVec4 nuVec4::operator * (const class nuMtx4& mtx) const
{
  nuVec4 ret;
  ret.x = x * mtx.m00 + y * mtx.m10 + z * mtx.m20 + w * mtx.m30;
  ret.y = x * mtx.m01 + y * mtx.m11 + z * mtx.m21 + w * mtx.m31;
  ret.z = x * mtx.m02 + y * mtx.m12 + z * mtx.m22 + w * mtx.m32;
  ret.w = x * mtx.m03 + y * mtx.m13 + z * mtx.m23 + w * mtx.m33;
  return ret;
}

nuVec4& nuVec4::operator *= (const class nuMtx4& mtx)
{
  nuVec4 temp = *this;
  x = temp.x * mtx.m00 + temp.y * mtx.m10 + temp.z * mtx.m20 + temp.w * mtx.m30;
  y = temp.x * mtx.m01 + temp.y * mtx.m11 + temp.z * mtx.m21 + temp.w * mtx.m31;
  z = temp.x * mtx.m02 + temp.y * mtx.m12 + temp.z * mtx.m22 + temp.w * mtx.m32;
  w = temp.x * mtx.m03 + temp.y * mtx.m13 + temp.z * mtx.m23 + temp.w * mtx.m33;
  return *this;
}
