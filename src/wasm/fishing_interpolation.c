#include "fishing/fishing_interpolation.h"

#ifdef FISHING_USE_CPP
extern "C"
{
#endif

#if FM_USE_SELF_NUM
#include "fishing/fishing_number.h"
    static fmData two;
    static fmData ntwo;
    static fmData three;
    static fmData nthree;
#endif

    void fmInterpolationinit(void)
    {
#if FM_USE_SELF_NUM
        two = ntwo = fmNumberInit(2);
        three = nthree = fmNumberInit(3);
        ntwo.negative = nthree.negative = fmTrue;
#endif
    }

    //CatmullRom interpolation
    fmData fmCatmullRom(fmData p0, fmData p1, fmData p2, fmData p3, fmData t)
    {
#if FM_USE_SELF_NUM
        fmData v0 = fmNumberDiv(fmNumberSub(p2, p0), two);
        fmData v1 = fmNumberDiv(fmNumberSub(p3, p1), two);
        fmData t2 = fmNumberMul(t, t);
        fmData t3 = fmNumberMul(t, t2);

        fmData r1 = fmNumberAdd(fmNumberMul(fmNumberSub(p1, p2), two), fmNumberAdd(v0, v1));
        fmData r2 = fmNumberSub(fmNumberMul(fmNumberSub(p2, p1), three), fmNumberAdd(fmNumberMul(v0, two), v1));

        return fmNumberAdd(fmNumberAdd(fmNumberAdd(fmNumberMul(r1, t3), fmNumberMul(r2, t2)), fmNumberMul(v0, t)), p1);

#else

    fmData v0 = (p2 - p0) / 2;
    fmData v1 = (p3 - p1) / 2;
    fmData t2 = t * t;
    fmData t3 = t * t2;
    return (2 * p1 - 2 * p2 + v0 + v1) * t3 + (-3 * p1 + 3 * p2 - 2 * v0 - v1) * t2 + v0 * t + p1;
#endif
    }

#ifdef FISHING_USE_CPP
}
#endif
