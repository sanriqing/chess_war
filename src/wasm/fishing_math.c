#include "fishing/fishing_math.h"
#include <math.h>

#if FM_USE_SELF_NUM
#include "fishing/fishing_number.h"
#endif

#ifdef FISHING_USE_CPP
extern "C"
{
#endif

    U32 fmLCM(U32 x, U32 y)
    {
        return x * y / fmGCD(x, y); //可能溢出
    }

    U32 fmGCD(U32 x, U32 y)
    {
        if (x == 0 || y == 0)
        {
            return x > y ? x : y;
        }
        U32 r = x % y;
        while (r > 0)
        {
            x = y;
            y = r;
            r = x % y;
        }
        return y;
    }

    fmFloat fmAtan2(fmFloat dy, fmFloat dx)
    {
        return atan2(dy, dx);
    }

    fmFloat fmSqrt(fmFloat v)
    {
        return sqrt(v);
    }

    fmFloat fmSqrt2(fmFloat dx, fmFloat dy)
    {
        return fmSqrt(dx * dx + dy * dy);
    }

    //sqrt
    U64 fmSqrt64(U64 v)
    {
        fmFloat c = v;
        return (U64)fmSqrt(c);
    }

#ifdef FISHING_USE_CPP
}
#endif
