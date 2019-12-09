#ifndef FISHING_RANDOM_32_H
#define FISHING_RANDOM_32_H

#include "fishing.h"

//随机数
typedef struct _fm_random32
{
    //索引
    U32 index;
    //表
    U32 mt[624]; //  需要根据.c N变化
} fmRandom32;

//随机数-获取
FM_EXPORT fmRandom32 *fmRandom32New(U32 seed);
//随机数-释放
FM_EXPORT void fmRandom32Free(fmRandom32 *r);
//随机数-设置种子
FM_EXPORT void fmRandom32SetSeed(fmRandom32 *r, U32 seed);
//随机数-随机一次
FM_EXPORT U32 fmRandom32Once(fmRandom32 *r, U32 limit);
//随机数-随机一次(前后包含)
FM_EXPORT I32 fmRandom32Zone(fmRandom32 *r, I32 minv, I32 maxv);
//随机数-拷贝
FM_EXPORT fmRandom32 *fmRandom32Clone(fmRandom32 *dst, fmRandom32 *src);

#endif
