#ifndef FISHING_MATH_H
#define FISHING_MATH_H

#include "fishing.h"

//最小公倍数
FM_EXPORT U32 fmLCM(U32 v, U32 v2);
//最大公约数
FM_EXPORT U32 fmGCD(U32 v, U32 v2);
//atan2
FM_EXPORT fmFloat fmAtan2(fmFloat dy, fmFloat dx);
//sqrt
FM_EXPORT fmFloat fmSqrt(fmFloat v);
//sqrt
FM_EXPORT U64 fmSqrt64(U64 v);
//sqrt2
FM_EXPORT fmFloat fmSqrt2(fmFloat dx, fmFloat dy);

#endif