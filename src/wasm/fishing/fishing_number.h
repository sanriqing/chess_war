#ifndef FISHING_FLOAT_H
#define FISHING_FLOAT_H

#include "fishing_bigint.h"

#ifndef FM_NUMBER_DEFINED
#define FM_NUMBER_DEFINED
//定点数
typedef struct _fishing_number
{
    //负号
    fmBool negative;
    //分子
    U64 up;
    //分母
    U64 down;
} fmNumber;
#endif

//转成int32
FM_EXPORT I32 fmNumberToI32(fmNumber v);
//转成浮点数
FM_EXPORT fmFloat fmNumberToFloat(fmNumber v);

//相等
FM_EXPORT fmBool fmNumberEquals(fmNumber x, fmNumber y);
//<
FM_EXPORT fmBool fmNumberLess(fmNumber x, fmNumber y);
//<=
FM_EXPORT fmBool fmNumberLessE(fmNumber x, fmNumber y);
//>
FM_EXPORT fmBool fmNumberGreater(fmNumber x, fmNumber y);
//>
FM_EXPORT fmBool fmNumberGreaterE(fmNumber x, fmNumber y);
//检测两数大小 -1-<, 0-==,1->
FM_EXPORT I8 fmNumberCheck(fmNumber x, fmNumber y);

//==0
FM_EXPORT fmBool fmNumberEquals0(fmNumber x);
//<0
FM_EXPORT fmBool fmNumberLess0(fmNumber x);
//<=0
FM_EXPORT fmBool fmNumberLessE0(fmNumber x);
//>0
FM_EXPORT fmBool fmNumberGreater0(fmNumber x);
//>=0
FM_EXPORT fmBool fmNumberGreaterE0(fmNumber x);

//初始化
FM_EXPORT fmNumber fmNumberInit(I64 integer);
//初始化
FM_EXPORT fmNumber fmNumberInitWithDown(I64 integer, U64 decimal);

//加
FM_EXPORT fmNumber fmNumberAdd(fmNumber x, fmNumber y);
//减
FM_EXPORT fmNumber fmNumberSub(fmNumber x, fmNumber y);
//乘
FM_EXPORT fmNumber fmNumberMul(fmNumber x, fmNumber y);
//除
FM_EXPORT fmNumber fmNumberDiv(fmNumber x, fmNumber y);

//绝对值
FM_EXPORT fmNumber fmNumberAbs(fmNumber x);

//开平方
FM_EXPORT fmNumber fmNumberSqrt(fmNumber x);
//开平方(sqrt(x * x + y * y))
FM_EXPORT fmNumber fmNumberSqrt2(fmNumber x, fmNumber y);

#endif
