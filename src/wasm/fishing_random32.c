#include "fishing/fishing_random32.h"

#ifdef FISHING_USE_CPP
extern "C"
{
#endif

    static const U32 W = 32;
    static const U32 N = 624;
    static const U32 M = 397;
    static const U32 R = 31;
    static const U32 A = 0x9908B0DF;
    static const U32 F = 1812433253;
    static const U32 U = 11;
    static const U32 D = 0xFFFFFFFF;
    static const U32 S = 7;
    static const U32 B = 0x9D2C5680;
    static const U32 T = 15;
    static const U32 C = 0xEFC60000;
    static const U32 L = 18;
    static const U32 MASK_UPPER = 1 << R;
    static const U32 MASK_LOWER = MASK_UPPER - 1;

    //翻转两次
    static void twist(fmRandom32 *r)
    {
        U32 *mt = r->mt;
        for (U32 i = 0; i < N; i++)
        {
            U32 x = (mt[i] & MASK_UPPER) + (mt[(i + 1) % N] & MASK_LOWER);
            U32 xA = x >> 1;
            if ((x & 0x1) != 0)
            {
                xA ^= A;
            }
            mt[i] = mt[(i + M) % N] ^ xA;
        }
        r->index = 0;
    }

    /** 获取一个随机数 */
    fmRandom32 *fmRandom32New(U32 seed)
    {
        fmRandom32 *r = (fmRandom32 *)fmCalloc(1, sizeof(fmRandom32));
        fmRandom32SetSeed(r, seed);
        return r;
    }

    //释放随机数
    void fmRandom32Free(fmRandom32 *r)
    {
        if (r != fmNull)
        {
            fmFree(r);
            r = fmNull;
        }
    }

    //随机数-设置种子
    void fmRandom32SetSeed(fmRandom32 *r, U32 seed)
    {
        U32 *mt = r->mt;
        mt[0] = seed;
        for (U32 i = 1; i < N; i++)
        {
            U32 t = mt[i - 1] ^ (mt[i - 1] >> (W - 2));
            mt[i] = F * t + i;
        }
        r->index = N;
    }

    //随机数-拷贝
    fmRandom32 *fmRandom32Clone(fmRandom32 *dst, fmRandom32 *src)
    {
        if (src == fmNull)
        {
            return dst;
        }
        if (dst == fmNull)
        {
            dst = (fmRandom32 *)malloc(sizeof(fmRandom32));
        }
        memcpy(dst, src, sizeof(fmRandom32));
        return dst;
    }

    //随机数随机一次
    U32 fmRandom32Once(fmRandom32 *r, U32 limit)
    {
        U32 *mt = r->mt;
        if (r->index >= N)
        {
            twist(r);
        }

        U32 y = mt[r->index++];
        y ^= (y >> U) & D;
        y ^= (y << S) & B;
        y ^= (y << T) & C;
        y ^= y >> L;

        return y % limit;
    }

    //随机数随机一次(前后包含)
    I32 fmRandom32Zone(fmRandom32 *r, I32 minv, I32 maxv)
    {
        U32 count = maxv > minv ? (maxv - minv) : (minv - maxv);
        I32 v = count == 0 ? minv : (I32)fmRandom32Once(r, count);
        if (count > 0)
        {
            v += maxv > minv ? minv : maxv;
        }
        return v;
    }

#ifdef FISHING_USE_CPP
}
#endif