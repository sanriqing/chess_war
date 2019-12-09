#include "fishing/fishing_number.h"
#include "fishing/fishing_math.h"

#ifdef FISHING_USE_CPP
extern "C"
{
#endif

    I32 fmNumberToI32(fmNumber x)
    {
        U64 v = x.up / x.down;
        I32 i32v = v;
        if (x.negative && i32v > 0)
        {
            i32v = -i32v;
        }
        return i32v;
    }

    fmFloat fmNumberToFloat(fmNumber x)
    {
        fmFloat r = 1.0 * x.up;
        r /= x.down;
        if (x.negative)
        {
            r = -r;
        }
        return r;
    }

    fmBool fmNumberEquals0(fmNumber x)
    {
        return x.up == 0;
    }

    //<0
    FM_EXPORT fmBool fmNumberLess0(fmNumber x)
    {
        return x.negative && x.up > 0;
    }

    //<=0
    FM_EXPORT fmBool fmNumberLessE0(fmNumber x)
    {
        return x.up == 0 || x.negative;
    }

    //>0
    FM_EXPORT fmBool fmNumberGreater0(fmNumber x)
    {
        return (!x.negative) && x.up > 0;
    }

    //>=0
    FM_EXPORT fmBool fmNumberGreaterE0(fmNumber x)
    {
        return x.up == 0 || (!x.negative);
    }

    I8 fmNumberCheck(fmNumber x, fmNumber y)
    {
        //分子都为0
        if (x.up == 0 && y.up == 0)
        {
            return 0;
        }
        //异号
        if (x.negative != y.negative)
        {
            return x.negative ? -1 : 1;
        }

        //同号
        U64 xv = x.up * y.down;
        U64 yv = x.down * y.up;
        if (xv == yv)
        {
            return 0;
        }

        if (xv > yv)
        {
            return x.negative ? -1 : 1;
        }
        else
        {
            return x.negative ? 1 : -1;
        }
    }

    //是否相等
    fmBool fmNumberEquals(fmNumber x, fmNumber y)
    {
        return fmNumberCheck(x, y) == 0;
    }

    fmBool fmNumberLess(fmNumber x, fmNumber y)
    {
        return fmNumberCheck(x, y) == -1;
    }

    fmBool fmNumberLessE(fmNumber x, fmNumber y)
    {
        return fmNumberCheck(x, y) <= 0;
    }

    fmBool fmNumberGreater(fmNumber x, fmNumber y)
    {
        return fmNumberCheck(x, y) == 1;
    }

    fmBool fmNumberGreaterE(fmNumber x, fmNumber y)
    {
        return fmNumberCheck(x, y) >= 0;
    }

    //最大公约数
    static U64 fmNumberGCD(U64 x, U64 y)
    {
        if (x == 0 && y == 0)
        {
            return 1;
        }

        U64 z = y;
        U64 t;

        while (fmTrue)
        {
            t = x % y;
            if (t == 0)
            {
                break;
            }
            z = t;
            x = y;
            y = z;
        }

        return z;
    }

    //对分数部分执行最大公约数化简和进位
    static void checkGCDAndCarry(fmNumber *x)
    {
        if (x->up == 0)
        {
            //分子为0
            x->down = 1;
            x->negative = fmFalse;
            return;
        }
        if (x->up == x->down)
        {
            x->up = x->down = 1;
            return;
        }

        //去除最大公约数
        if (x->up == 1 || x->down == 1)
        {
            return;
        }

        // U64 MAX_U = 10000 * 10000 * 10;
        // if (x->up > x->down)
        // {
        //     U64 div = x->up / x->down;
        //     if (div >= MAX_U)
        //     {
        //         x->up = div;
        //         x->down = 1;
        //         return;
        //     }
        // }
        // else
        // {
        //     U64 div = x->down / x->up;
        //     if (div >= MAX_U)
        //     {
        //         x->up = 1;
        //         x->down = div;
        //         return;
        //     }
        // }

        U64 gcd = fmNumberGCD(x->up, x->down);
        x->up /= gcd;
        x->down /= gcd;
    }

    //初始化
    fmNumber fmNumberInit(I64 up)
    {
        fmNumber v;
        v.negative = up < 0;
        v.up = fmAbs(up);
        v.down = 1;
        return v;
    }

    //初始化
    fmNumber fmNumberInitWithDown(I64 up, U64 down)
    {
        //参数合法检验
        if (down == 0)
        {
            printf("错误的分子！=0，分母==0\n");
            down = 1;
        }
        if (up == 0)
        {
            up = 1;
        }

        fmNumber v;
        v.negative = up < 0;
        v.up = fmAbs(up);
        v.down = down;
        checkGCDAndCarry(&v);
        return v;
    }

    fmNumber fmNumberAdd(fmNumber x, fmNumber y)
    {
        fmNumber v;
        v.down = x.down * y.down;
        if (x.negative == y.negative)
        {
            v.up = x.up * y.down + x.down * y.up;
            v.negative = x.negative;
        }
        else
        {
            U64 v1 = x.up * y.down;
            U64 v2 = x.down * y.up;
            if (v1 >= v2)
            {
                v.up = v1 - v2;
                v.negative = x.negative;
            }
            else
            {
                v.up = v2 - v1;
                v.negative = y.negative;
            }
        }
        checkGCDAndCarry(&v);
        return v;
    }

    fmNumber fmNumberSub(fmNumber x, fmNumber y)
    {
        y.negative = !y.negative;
        return fmNumberAdd(x, y);
    }

    fmNumber fmNumberMul(fmNumber x, fmNumber y)
    {
        x.negative = x.negative != y.negative;
        x.up *= y.up;
        x.down *= y.down;
        checkGCDAndCarry(&x);
        return x;
    }

    fmNumber fmNumberDiv(fmNumber x, fmNumber y)
    {
        if (y.up == 0)
        {
            printf("fmNumber 除0错误\n");
            return x;
        }
        x.negative = x.negative != y.negative;
        x.up *= y.down;
        x.down *= y.up;
        checkGCDAndCarry(&x);
        return x;
    }

    fmNumber fmNumberAbs(fmNumber x)
    {
        x.negative = fmFalse;
        return x;
    }

    fmNumber fmNumberSqrt(fmNumber x)
    {
        if (fmNumberEquals0(x))
        {
            return x;
        }

        //负数转正
        if (x.negative)
        {
            printf("fm number 负数开平方错误\n");
            x.negative = fmFalse;
        }

        x.up = fmSqrt64(x.up);
        x.down = fmSqrt64(x.down);
        return x; //TODO 精度很差
    }

    //开平方(sqrt(x * x + y * y))
    fmNumber fmNumberSqrt2(fmNumber x, fmNumber y)
    {
        return fmNumberSqrt(fmNumberAdd(fmNumberMul(x, x), fmNumberMul(y, y)));
    }

#ifdef FISHING_USE_CPP
}
#endif
