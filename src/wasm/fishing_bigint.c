#include "fishing/fishing_bigint.h"

#ifdef FISHING_USE_CPP
extern "C"
{
#endif

    //初始化 0
    static fmBigInt initFmBigInt()
    {
        fmBigInt v;
        memset(&v, 0, sizeof(fmBigInt));
        return v;
    }

    //设置最高有效位
    static void setMaxActiveIndex(fmBigInt *x, I16 capacity)
    {
        if (x == fmNull)
        {
            return;
        }

        if (capacity < 0 || capacity >= FM_BIG_INT_CAPACITY)
        {
            capacity = FM_BIG_INT_CAPACITY - 1;
        }

        x->maxActiveIndex = 0;
        for (I16 i = capacity; i >= 0; i--)
        {
            if (x->values[i] > 0)
            {
                x->maxActiveIndex = i;
                break;
            }
        }
    }

    //初始化值
    fmBigInt fmBigIntInitU(U32 uintv)
    {
        fmBigInt v = initFmBigInt();
        if (uintv > 0)
        {
            U8 idx = 0;
            while (uintv > 0)
            {
                v.values[idx++] = uintv % FM_BIG_INT_MAX;
                uintv /= FM_BIG_INT_MAX;
            }
            setMaxActiveIndex(&v, idx);
        }
        return v;
    }

    //初始化值
    fmBigInt fmBigIntInit(I32 intv)
    {
        fmBigInt v = fmBigIntInitU(intv >= 0 ? intv : -intv);
        v.negative = intv < 0;
        return v;
    }

    //x非负 y非正 且 abs(x) > abs(y) 的加法计算
    static fmBigInt doFmBigIntAdd(fmBigInt *x, fmBigInt *y)
    {
        U32 subv = 0; //借位
        fmBigInt v = initFmBigInt();
        U8 capacity = fmMax(x->maxActiveIndex, y->maxActiveIndex);

        for (I16 i = 0; i <= capacity; i++)
        {
            if (x->values[i] >= y->values[i] + subv)
            {
                v.values[i] = x->values[i] - y->values[i] - subv;
                subv = 0;
            }
            else
            {
                v.values[i] = FM_BIG_INT_MAX + x->values[i] - y->values[i] - subv;
                subv = 1;
            }
        }

        setMaxActiveIndex(&v, capacity);

        return v;
    }

    //加
    fmBigInt fmBigIntAdd(fmBigInt x, fmBigInt y)
    {
        if (x.negative == y.negative)
        {
            //同号
            U32 addv = 0; //进位
            fmBigInt v = initFmBigInt();
            U8 capacity = fmMin(FM_BIG_INT_CAPACITY - 1, fmMax(x.maxActiveIndex, y.maxActiveIndex) + 1);

            v.negative = x.negative;
            for (I32 i = 0; i <= capacity; i++)
            {
                addv += x.values[i] + y.values[i];
                if (addv < FM_BIG_INT_MAX)
                {
                    v.values[i] = addv;
                    addv = 0;
                }
                else
                {
                    v.values[i] = addv - FM_BIG_INT_MAX;
                    addv = 1;
                }
            }

            setMaxActiveIndex(&v, capacity);

            return v;
        }
        else
        {
            //异号
            if (x.negative)
            {
                //负数在前，调整顺序
                return fmBigIntAdd(y, x);
            }
            else
            {
                //正数在前
                y.negative = fmFalse;
                fmBool isGreaterE = fmBigIntGreaterE(x, y);
                if (isGreaterE)
                {
                    //绝对值 x 比 y 大
                    y.negative = fmTrue;
                    return doFmBigIntAdd(&x, &y);
                }
                else
                {
                    //绝对值 x 比 y 小，反转正负号，再把结果反转
                    x.negative = fmTrue;
                    fmBigInt v = doFmBigIntAdd(&y, &x);
                    v.negative = fmTrue;
                    return v;
                }
            }
        }
    }

    //减
    fmBigInt fmBigIntSub(fmBigInt x, fmBigInt y)
    {
        y.negative = !y.negative;
        return fmBigIntAdd(x, y);
    }

    //乘
    fmBigInt fmBigIntMul(fmBigInt x, fmBigInt y)
    {
        fmBigInt v = initFmBigInt();
        if (fmBigIntEquals0(x) || fmBigIntEquals0(y))
        {
            return v;
        }

        //外循环比内循环小，加快速度
        if (x.maxActiveIndex > y.maxActiveIndex)
        {
            fmBigInt tmp = x;
            x = y;
            y = tmp;
        }

        //同号为正
        v.negative = x.negative != y.negative;

        //竖式乘法累加
        for (I16 i = 0; i <= x.maxActiveIndex; i++)
        {
            if (x.values[i] == 0)
            {
                continue;
            }
            for (I16 j = 0; j <= y.maxActiveIndex; j++)
            {
                if (y.values[j] == 0)
                {
                    continue;
                }

                I16 didx = i + j;

                //相乘并考虑进位
                I16 checked = 0;
                U32 mulv = x.values[i] * y.values[j];
                if (mulv >= FM_BIG_INT_MAX)
                {
                    v.values[didx] += mulv % FM_BIG_INT_MAX;
                    v.values[didx + 1] += mulv / FM_BIG_INT_MAX;
                    checked = 2;
                }
                else
                {
                    v.values[didx] += mulv;
                    checked = 1;
                }

                //进位计算
                for (I16 k = 0; k < checked; k++)
                {
                    I16 idx = didx + k;
                    if (v.values[idx] >= FM_BIG_INT_MAX)
                    {
                        v.values[idx + 1] += v.values[idx] / FM_BIG_INT_MAX;
                        v.values[idx] %= FM_BIG_INT_MAX;
                        checked++;
                    }
                }
            }
        }

        setMaxActiveIndex(&v, x.maxActiveIndex + y.maxActiveIndex + 1);

        return v;
    }

    //求商和余
    void fmBigIntDivAndMode(fmBigInt x, fmBigInt y, fmBigInt *divValue, fmBigInt *modValue)
    {
        //被除数 or 除数 为0
        if (fmBigIntEquals0(x) || fmBigIntEquals0(y))
        {
            if (fmBigIntEquals0(y))
            {
                printf("big int 除0错误\n");
            }
            if (divValue != fmNull)
            {
                memset(divValue, 0, sizeof(fmBigInt));
            }
            if (modValue != fmNull)
            {
                memset(modValue, 0, sizeof(fmBigInt));
            }
            return;
        }

        fmBool xn = x.negative;
        fmBool yn = y.negative;

        //除数绝对值为1
        if (y.maxActiveIndex == 0 && y.values[0] == 1)
        {
            if (divValue != fmNull)
            {
                *divValue = x;
                divValue->negative = xn != yn;
            }
            if (modValue != fmNull)
            {
                memset(modValue, 0, sizeof(fmBigInt));
            }
            return;
        }

        //被除数不比除数大 abs(x) <= abs(y)
        I8 checkR = fmBigIntCheckAbs(x, y);
        if (checkR == 0)
        {
            if (divValue != fmNull)
            {
                memset(divValue, 0, sizeof(fmBigInt));
                divValue->values[0] = 1;
                divValue->negative = xn != yn;
            }
            if (modValue != fmNull)
            {
                memset(modValue, 0, sizeof(fmBigInt));
            }
            return;
        }
        else if (checkR == -1)
        {
            if (divValue != fmNull)
            {
                memset(divValue, 0, sizeof(fmBigInt));
            }
            if (modValue != fmNull)
            {
                *modValue = x;
            }
            return;
        }

        //全部转化为正数计算
        x.negative = y.negative = fmFalse;

        //开始除法
        fmBigInt divTmp = initFmBigInt();
        fmBigInt modTmp = initFmBigInt();
        //分行写，否则U8非负
        I16 capacity = x.maxActiveIndex;
        capacity -= y.maxActiveIndex;
        //余数移位位数
        I16 modMaxCapacity = fmMin(y.maxActiveIndex + 1, FM_BIG_INT_CAPACITY - 1);

        //初始余数
        for (I16 i = 0; i < y.maxActiveIndex; i++)
        {
            modTmp.values[y.maxActiveIndex - 1 - i] = x.values[x.maxActiveIndex - i]; //y.maxActiveIndex - 1是为了后面的余数移位相对应
        }

        for (I16 n = capacity; n >= 0; n--)
        {
            //余数移位
            for (I16 i = modMaxCapacity; i > 0; i--)
            {
                modTmp.values[i] = modTmp.values[i - 1];
            }
            //被除数补位
            modTmp.values[0] = x.values[n];
            setMaxActiveIndex(&modTmp, y.maxActiveIndex + 1);

            //循环减
            while (fmBigIntGreaterE(modTmp, y))
            {
                divTmp.values[n]++;
                modTmp = fmBigIntSub(modTmp, y);
            }
        }

        //存储最终值
        if (divValue != fmNull)
        {
            *divValue = divTmp;
            divValue->negative = xn != yn;
            setMaxActiveIndex(divValue, -1);
        }
        if (modValue != fmNull)
        {
            *modValue = modTmp;
            modValue->negative = xn;
            setMaxActiveIndex(modValue, -1);
        }
    }

    //最大公约数
    fmBigInt fmBigIntGCD(fmBigInt x, fmBigInt y)
    {
        if (fmBigIntEquals0(x) || fmBigIntEquals0(y))
        {
            return initFmBigInt();
        }

        //公约数只有正数
        x.negative = y.negative = fmFalse;

        fmBigInt z = y;
        fmBigInt t;

        while (fmTrue)
        {
            t = fmBigIntMode(x, y);
            if (fmBigIntEquals0(t))
            {
                break;
            }
            z = t;
            x = y;
            y = z;
        }

        return z;
    }

    //约分
    void fmBigIntGCDReduce(fmBigInt *x, fmBigInt *y)
    {
        fmBigInt z = fmBigIntGCD(*x, *y);
        if (!fmBigIntEquals0(z))
        {
            *x = fmBigIntDiv(*x, z);
            *y = fmBigIntDiv(*y, z);
        }
    }

    //商
    fmBigInt fmBigIntDiv(fmBigInt x, fmBigInt y)
    {
        fmBigInt v = initFmBigInt();
        fmBigIntDivAndMode(x, y, &v, fmNull);
        return v;
    }

    //余数
    fmBigInt fmBigIntMode(fmBigInt x, fmBigInt y)
    {
        fmBigInt v = initFmBigInt();
        fmBigIntDivAndMode(x, y, fmNull, &v);
        return v;
    }

    //对角线
    fmBigInt fmBigIntSqrt2(fmBigInt x, fmBigInt y)
    {
        return fmBigIntSqrt(fmBigIntAdd(fmBigIntMul(x, x), fmBigIntMul(y, y)));
    }

    //开方
    fmBigInt fmBigIntSqrt(fmBigInt x)
    {
        if (fmBigIntEquals0(x))
        {
            return x;
        }
        if (x.negative)
        {
            printf("big int 负数开平方错误\n");
            x.negative = fmFalse;
        }

        //牛顿迭代
        fmBigInt s = x;
        fmBigInt two = fmBigIntInit(2);
        while (fmBigIntGreater(fmBigIntMul(s, s), x))
        {
            s = fmBigIntDiv(fmBigIntAdd(s, fmBigIntDiv(x, s)), two);
        }
        return s;
    }

    //0值判断
    fmBool fmBigIntEquals0(fmBigInt x)
    {
        return x.maxActiveIndex == 0 && x.values[0] == 0;
    }

    //检测两数绝对值大小
    I8 fmBigIntCheckAbs(fmBigInt x, fmBigInt y)
    {
        //长度不一致
        if (x.maxActiveIndex != y.maxActiveIndex)
        {
            return x.maxActiveIndex > y.maxActiveIndex ? 1 : -1;
        }

        //长度一致
        for (I16 i = x.maxActiveIndex; i >= 0; i--)
        {
            if (x.values[i] != y.values[i])
            {
                return x.values[i] > y.values[i] ? 1 : -1;
            }
        }

        return 0;
    }

    //检测两数大小
    I8 fmBigIntCheck(fmBigInt x, fmBigInt y)
    {
        if (x.negative == y.negative)
        {
            //符号一样
            I8 r = fmBigIntCheckAbs(x, y);
            if (r != 0 && x.negative)
            {
                r = -r;
            }
            return r;
        }
        else
        {
            //符号不一样
            if (x.maxActiveIndex > 0 || x.values[0] > 0)
            {
                return x.negative ? -1 : 1;
            }
            else if (y.maxActiveIndex > 0 || y.values[0] > 0)
            {
                return y.negative ? 1 : -1;
            }
            else
            {
                return 0;
            }
        }
    }

    //相等判断
    fmBool fmBigIntEquals(fmBigInt x, fmBigInt y)
    {
        return fmBigIntCheck(x, y) == 0;
    }

    //<
    fmBool fmBigIntLess(fmBigInt x, fmBigInt y)
    {
        return fmBigIntCheck(x, y) == -1;
    }

    //<=
    fmBool fmBigIntLessE(fmBigInt x, fmBigInt y)
    {
        return fmBigIntCheck(x, y) <= 0;
    }

    //>
    fmBool fmBigIntGreater(fmBigInt x, fmBigInt y)
    {
        return fmBigIntCheck(x, y) == 1;
    }

    //>=
    fmBool fmBigIntGreaterE(fmBigInt x, fmBigInt y)
    {
        return fmBigIntCheck(x, y) >= 0;
    }

    //转化成int32
    I32 fmBigIntToInt32(fmBigInt x)
    {
        I32 v = 0;
        for (I16 i = x.maxActiveIndex; i >= 0; i--)
        {
            v = v * FM_BIG_INT_MAX + x.values[i];
        }
        if (x.negative)
        {
            v = -v;
        }
        return v;
    }

    //转化成float
    fmFloat fmBigIntToFloat(fmBigInt x)
    {
        fmFloat v = 0;
        for (I16 i = x.maxActiveIndex; i >= 0; i--)
        {
            v = v * FM_BIG_INT_MAX + x.values[i];
        }
        if (x.negative)
        {
            v = -v;
        }
        return v;
    }

#ifdef FISHING_USE_CPP
}
#endif
