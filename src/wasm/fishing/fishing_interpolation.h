#ifndef FM_INTERPOLATION_H
#define FM_INTERPOLATION_H

#include "fishing.h"

//插值初始化
FM_EXPORT void fmInterpolationinit(void);

//CatmullRom interpolation
FM_EXPORT fmData fmCatmullRom(fmData p0, fmData p1, fmData p2, fmData p3, fmData t);

#endif