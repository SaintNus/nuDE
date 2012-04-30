/*!
 * \file nuType.h
 * \brief Fundamental types.
 * \author Nus
 * \date 2011/09/11 18:04
 */

#ifndef __NUTYPE_H__
#define __NUTYPE_H__

//! \brief 8-bit integer.
typedef char i8;
//! \brief Unsigned integer.
typedef unsigned char ui8;

//! \brief 16-bit integer.
typedef short i16;
//! \brief Unsigned integer.
typedef unsigned short ui16;

//! \brief Integer.
typedef int32_t i32;
//! \brief Unsigned integer.
typedef uint32_t ui32;

//! \brief 64-bit integer.
typedef int64_t i64;
//! \brief 64-bit unsigned integer.
typedef uint64_t ui64;

//! \brief Float.
typedef float f32;
//! \brief Double.
typedef double f64;

//! \brief Boolean.
typedef bool boolean;

//! \brief String.
typedef char* cstr;
//! \brief String constant.
typedef const char* ccstr;

//! \brief String.
typedef wchar_t* wcstr;
//! \brief String constant.
typedef const wchar_t* wccstr;

//! \brief String.
typedef char c8;
//! \brief String.
typedef wchar_t c32;

#define FORCE_VISIBLE __attribute__((visibility("default")))

#endif
