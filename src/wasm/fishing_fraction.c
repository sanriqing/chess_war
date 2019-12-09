#include "fishing/fishing_fraction.h"

#ifdef FISHING_USE_CPP
extern "C"
{
#endif

    I32 fmFractionToI32(fmFraction v)
    {
        fmBigInt div = fmBigIntDiv(v.up, v.down);
        return fmBigIntToInt32(div);
    }

    fmFloat fmFractionToFloat(fmFraction v)
    {
        fmFloat r = fmBigIntToFloat(v.up);
        r /= fmBigIntToFloat(v.down);
        return r;
    }

    fmBool fmFractionEquals0(fmFraction v)
    {
        return fmBigIntEquals0(v.up);
    }

    //<0
    FM_EXPORT fmBool fmFractionLess0(fmFraction x)
    {
        return fmBigIntCheck(x.up, fmBigIntInitU(0)) < 0;
    }

    //<=0
    FM_EXPORT fmBool fmFractionLessE0(fmFraction x)
    {
        return fmBigIntCheck(x.up, fmBigIntInitU(0)) <= 0;
    }

    //>0
    FM_EXPORT fmBool fmFractionGreater0(fmFraction x)
    {
        return fmBigIntCheck(x.up, fmBigIntInitU(0)) > 0;
    }

    //>=0
    FM_EXPORT fmBool fmFractionGreaterE0(fmFraction x)
    {
        return fmBigIntCheck(x.up, fmBigIntInitU(0)) >= 0;
    }

    I8 fmFractionCheck(fmFraction x, fmFraction y)
    {
        //分子都为0
        if (fmBigIntEquals0(x.up) && fmBigIntEquals0(y.up))
        {
            return 0;
        }
        //异号
        if (x.up.negative != y.up.negative)
        {
            return x.up.negative ? -1 : 1;
        }
        //b/a-c/d=(bd - ac)/ad
        return fmBigIntCheck(fmBigIntMul(x.up, y.down), fmBigIntMul(x.down, y.up));
    }

    //是否相等
    fmBool fmFractionEquals(fmFraction x, fmFraction y)
    {
        return fmFractionCheck(x, y) == 0;
    }

    fmBool fmFractionLess(fmFraction x, fmFraction y)
    {
        return fmFractionCheck(x, y) == -1;
    }

    fmBool fmFractionLessE(fmFraction x, fmFraction y)
    {
        return fmFractionCheck(x, y) <= 0;
    }

    fmBool fmFractionGreater(fmFraction x, fmFraction y)
    {
        return fmFractionCheck(x, y) == 1;
    }

    fmBool fmFractionGreaterE(fmFraction x, fmFraction y)
    {
        return fmFractionCheck(x, y) >= 0;
    }

    //对分数部分执行最大公约数化简和进位
    static fmFraction checkGCDAndCarry(fmFraction x)
    {
        //分子为0
        if (fmBigIntEquals0(x.up))
        {
            x.down = fmBigIntInitU(1);
            return x;
        }
        //去除最大公约数
        fmBigIntGCDReduce(&x.up, &x.down);
        return x;
    }

    //初始化
    fmFraction fmFractionInit(I32 up)
    {
        return fmFractionInitWithDown(up, 1);
    }

    //初始化
    fmFraction fmFractionInitWithDown(I32 up, U32 down)
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

        fmFraction v;
        v.up = fmBigIntInit(up);
        v.down = fmBigIntInitU(down);
        return checkGCDAndCarry(v);
    }

    fmFraction fmFractionAdd(fmFraction x, fmFraction y)
    {
        fmBigInt r1 = fmBigIntMul(x.up, y.down);
        fmBigInt r2 = fmBigIntMul(y.up, x.down);
        fmFraction v;
        v.up = fmBigIntAdd(r1, r2);
        v.down = fmBigIntMul(x.down, y.down);
        return checkGCDAndCarry(v);
    }

    fmFraction fmFractionSub(fmFraction x, fmFraction y)
    {
        y.up.negative = !y.up.negative;
        return fmFractionAdd(x, y);
    }

    fmFraction fmFractionMul(fmFraction x, fmFraction y)
    {
        fmBool negative = x.up.negative != y.up.negative;
        x.up.negative = y.up.negative = fmFalse;

        fmFraction v;
        v.up = fmBigIntMul(x.up, y.up);
        v.down = fmBigIntMul(x.down, y.down);

        v = checkGCDAndCarry(v);
        v.up.negative = negative;
        return v;
    }

    fmFraction fmFractionDiv(fmFraction x, fmFraction y)
    {
        if (fmBigIntEquals0(y.up))
        {
            printf("fmFraction 除0错误\n");
            return x;
        }
        fmBigInt tmp = y.up;
        y.up = y.down;
        y.up.negative = tmp.negative;
        y.down = tmp;
        y.down.negative = fmFalse;
        return fmFractionMul(x, y);
    }

    fmFraction fmFractionAbs(fmFraction x)
    {
        x.up.negative = fmFalse;
        return x;
    }

    fmFraction fmFractionSqrt(fmFraction x)
    {
        if (fmFractionEquals0(x))
        {
            return x;
        }

        //负数转正
        if (x.up.negative)
        {
            printf("fm number 负数开平方错误\n");
            x.up.negative = fmFalse;
        }

        //扩大1w
        x.up = fmBigIntSqrt(x.up);
        x.down = fmBigIntSqrt(x.down);
        return x; //TODO 精度很差
    }

    //开平方(sqrt(x * x + y * y))
    fmFraction fmFractionSqrt2(fmFraction x, fmFraction y)
    {
        return fmFractionSqrt(fmFractionAdd(fmFractionMul(x, x), fmFractionMul(y, y)));
    }

#ifdef FISHING_USE_CPP
}
#endif
