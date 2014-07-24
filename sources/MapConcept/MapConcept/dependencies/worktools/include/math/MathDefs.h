#pragma once

#define FL_PI  f32(3.14159265358979323846264338327950288419716939937510) // pi
#define FL_PI2 f32(3.14159265358979323846264338327950288419716939937510*2.0) // 2*pi

//Variable redefinition
	typedef signed char         int8;
	typedef signed short        int16;
	typedef signed int			int32;
	typedef signed __int64		int64;

	typedef unsigned char		uint8;
	typedef unsigned short		uint16;
	typedef unsigned int		uint32;
	typedef unsigned __int64	uint64;

	typedef float               f32;
	typedef double              f64;

//Variable redefinition: Windows style
	typedef unsigned long       DWORD;
	typedef int                 BOOL;
	typedef unsigned char       BYTE;
	typedef unsigned short      WORD;
	typedef float               FLOAT;
	typedef int                 INT;
	typedef unsigned int        UINT;

//Variable rounding
	#define VEC_EPSILON 0.05f
	#define ROUNDING_ERROR_S32  0
	#define ROUNDING_ERROR_f32  0.000001f
	#define ROUNDING_ERROR_f64  0.00000001

//Some math macro
	#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
	#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
	#define DEG2RAD( a ) ( (a) * (FL_PI/180.0) )
	#define RAD2DEG( a ) ( (a) * (180.0/FL_PI) )
	#define LERP(A, B, Alpha) (A + Alpha * (B-A))
	#define SATURATE(X) clamp(X, 0.f, 1.f)
	#define CLAMP(X, mn, mx) ((X)<(mn) ? (mn) : ((X)<(mx) ? (X) : (mx)))

//Math elements
	template <typename F> class Vec2T;
	template <typename F> class Vec3T;
	template <typename F> class Vec4T;

	template <typename F> class Ang3T;
	template <typename F> class QuatT;
	template <typename F> class Matrix44T;
	template <typename T> class ColorT;

#define assert _ASSERT