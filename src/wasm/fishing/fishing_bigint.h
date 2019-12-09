#ifndef FISHING_BIG_INT_H
#define FISHING_BIG_INT_H

#include "fishing.h"

//每一位代表的最大值 （4294967295 的开方取好算的值 2的12次方 大概6W (1024 * 32 = 32768)）
#define FM_BIG_INT_MAX 10

//大数最大长度
#ifndef FM_BIG_INT_CAPACITY
#define FM_BIG_INT_CAPACITY 32
//大数
typedef struct _fishing_big_int
{
    //负号
    fmBool negative;
    //最高有效位
    U8 maxActiveIndex;
    //值
    U8 values[FM_BIG_INT_CAPACITY];
} fmBigInt;
#endif

//初始化
FM_EXPORT fmBigInt fmBigIntInit(I32 intv);
//初始化
FM_EXPORT fmBigInt fmBigIntInitU(U32 uintv);

//加
FM_EXPORT fmBigInt fmBigIntAdd(fmBigInt x, fmBigInt y);
//减
FM_EXPORT fmBigInt fmBigIntSub(fmBigInt x, fmBigInt y);
//乘
FM_EXPORT fmBigInt fmBigIntMul(fmBigInt x, fmBigInt y);
//除
FM_EXPORT fmBigInt fmBigIntDiv(fmBigInt x, fmBigInt y);
//取模
FM_EXPORT fmBigInt fmBigIntMode(fmBigInt x, fmBigInt y);
//求 x / y 和 x % y
FM_EXPORT void fmBigIntDivAndMode(fmBigInt x, fmBigInt y, fmBigInt *divValue, fmBigInt *modValue);
//最大公约数
FM_EXPORT fmBigInt fmBigIntGCD(fmBigInt x, fmBigInt y);
//最大公约数化简
FM_EXPORT void fmBigIntGCDReduce(fmBigInt *x, fmBigInt *y);

//开平方
FM_EXPORT fmBigInt fmBigIntSqrt(fmBigInt x);
//开平方(sqrt(x * x + y * y))
FM_EXPORT fmBigInt fmBigIntSqrt2(fmBigInt x, fmBigInt y);

//0值判断
FM_EXPORT fmBool fmBigIntEquals0(fmBigInt x);
//相等判断
FM_EXPORT fmBool fmBigIntEquals(fmBigInt x, fmBigInt y);
//<
FM_EXPORT fmBool fmBigIntLess(fmBigInt x, fmBigInt y);
//<=
FM_EXPORT fmBool fmBigIntLessE(fmBigInt x, fmBigInt y);
//>
FM_EXPORT fmBool fmBigIntGreater(fmBigInt x, fmBigInt y);
//>=
FM_EXPORT fmBool fmBigIntGreaterE(fmBigInt x, fmBigInt y);
//检测两数大小 -1-<, 0-==,1->
FM_EXPORT I8 fmBigIntCheck(fmBigInt x, fmBigInt y);
//检测两数绝对值大小 -1-<, 0-==,1->
FM_EXPORT I8 fmBigIntCheckAbs(fmBigInt x, fmBigInt y);

//转化成int32
FM_EXPORT I32 fmBigIntToInt32(fmBigInt x);
//转化成float
FM_EXPORT fmFloat fmBigIntToFloat(fmBigInt x);

#endif