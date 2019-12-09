#ifndef FISHING_FRACTION_H
#define FISHING_FRACTION_H

#include "fishing_bigint.h"

//分数
typedef struct _fishing_fraction
{
    //分子(定点数符号与此一致)
    fmBigInt up;
    //分母
    fmBigInt down;
} fmFraction;

//转成int32
FM_EXPORT I32 fmFractionToI32(fmFraction v);
//转成浮点数
FM_EXPORT fmFloat fmFractionToFloat(fmFraction v);

//相等
FM_EXPORT fmBool fmFractionEquals(fmFraction x, fmFraction y);
//<
FM_EXPORT fmBool fmFractionLess(fmFraction x, fmFraction y);
//<=
FM_EXPORT fmBool fmFractionLessE(fmFraction x, fmFraction y);
//>
FM_EXPORT fmBool fmFractionGreater(fmFraction x, fmFraction y);
//>
FM_EXPORT fmBool fmFractionGreaterE(fmFraction x, fmFraction y);
//检测两数大小 -1-<, 0-==,1->
FM_EXPORT I8 fmFractionCheck(fmFraction x, fmFraction y);

//==0
FM_EXPORT fmBool fmFractionEquals0(fmFraction x);
//<0
FM_EXPORT fmBool fmFractionLess0(fmFraction x);
//<=0
FM_EXPORT fmBool fmFractionLessE0(fmFraction x);
//>0
FM_EXPORT fmBool fmFractionGreater0(fmFraction x);
//>=0
FM_EXPORT fmBool fmFractionGreaterE0(fmFraction x);


//初始化
FM_EXPORT fmFraction fmFractionInit(I32 up);
//初始化
FM_EXPORT fmFraction fmFractionInitWithDown(I32 up, U32 down);

//加
FM_EXPORT fmFraction fmFractionAdd(fmFraction x, fmFraction y);
//减
FM_EXPORT fmFraction fmFractionSub(fmFraction x, fmFraction y);
//乘
FM_EXPORT fmFraction fmFractionMul(fmFraction x, fmFraction y);
//除
FM_EXPORT fmFraction fmFractionDiv(fmFraction x, fmFraction y);

//绝对值
FM_EXPORT fmFraction fmFractionAbs(fmFraction x);

//开平方,损失精度)
FM_EXPORT fmFraction fmFractionSqrt(fmFraction x);
//开平方(sqrt(x * x + y * y))
FM_EXPORT fmFraction fmFractionSqrt2(fmFraction x, fmFraction y);


#endif
