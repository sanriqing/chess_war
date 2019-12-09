#ifndef FISHING_TYPES_H
#define FISHING_TYPES_H

#include <stdint.h>
#include <float.h>
#include <math.h>

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

// Use CGTypes by default on iOS and Mac.
// Also enables usage of doubles on 64 bit.
// Performance is usually very comparable when the CPU cache is well utilised.
#if (TARGET_OS_IPHONE || TARGET_OS_MAC) && (!defined FM_USE_CGTYPES)
#define FM_USE_CGTYPES 1
#endif

#if FM_USE_CGTYPES
#if TARGET_OS_IPHONE
#include <CoreGraphics/CGGeometry.h>
#include <CoreGraphics/CGAffineTransform.h>
#elif TARGET_OS_MAC
#include <ApplicationServices/ApplicationServices.h>
#endif

#if defined(__LP64__) && __LP64__
#define FM_USE_DOUBLES 1
#else
#define FM_USE_DOUBLES 0
#endif
#endif

#ifndef FM_USE_DOUBLES
// Use doubles by default for higher precision.
#define FM_USE_DOUBLES 1
#endif

#if FM_USE_DOUBLES
/// Fish's floating point type.
/// Can be reconfigured at compile time.
typedef double fmFloat;
#else
typedef float fmFloat;
#endif

#ifndef PI
#define PI ((fmFloat)3.14159265358979323846264338327950288)
#endif

//每帧间隔时间
#define TICK 16

#ifdef BOOL
typedef BOOL fmBool;
#else
typedef unsigned char fmBool;
#endif

#ifndef fmTrue
/// true value.
#define fmTrue 1
#endif

#ifndef fmFalse
/// false value.
#define fmFalse 0
#endif

#ifdef DATA_POINTER_TYPE
typedef DATA_POINTER_TYPE fmPtr;
#else
/// Type used for user data pointers.
typedef size_t fmPtr;
#endif

#ifndef fmNull
#define fmNull NULL
#endif

//Java byte
typedef char I8;
//java unsigned byte
typedef unsigned char U8;
//java short
typedef short I16;
//java unsigned short
typedef unsigned short U16;
//java int
typedef int I32;
//java unsigned int
typedef unsigned int U32;
//java long
typedef long long int I64;
//java unsigned long
typedef unsigned long long int U64;

//使用自定义数值
#define FM_USE_SELF_NUM 0

//逻辑计算数值类型
typedef I32 fmData;


#ifdef __cplusplus
#define FISHING_USE_CPP 1
#endif

#endif
