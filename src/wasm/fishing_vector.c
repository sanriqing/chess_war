#include "fishing/fishing_vector.h"

#ifdef FISHING_USE_CPP
extern "C"
{
#endif

    fmVector *fmVectorNew(U32 capacity)
    {
        fmVector *vector = (fmVector *)fmCalloc(1, sizeof(fmVector));
        vector->size = 0;
        vector->capacity = (capacity > 0 ? capacity : 4);
        vector->data = (void **)fmCalloc(vector->capacity, sizeof(void *));
        return vector;
    }

    void fmVectorFree(fmVector *vector)
    {
        if (vector != fmNull)
        {
            if (vector->data != fmNull)
            {
                fmFree(vector->data);
                vector->data = fmNull;
            }
            fmFree(vector);
        }
    }

    void fmVectorPush(fmVector *vector, void *object)
    {
        if (vector->size >= vector->capacity)
        {
            fmVectorRecapacity(vector, 3 * (vector->capacity + 1) / 2); //+50%
        }
        vector->data[vector->size] = object;
        vector->size++;
    }

    void *fmVectorPop(fmVector *vector)
    {
        if (vector->size > 0)
        {
            vector->size--;
            void *value = vector->data[vector->size];
            vector->data[vector->size] = fmNull;
            return value;
        }
        else
        {
            return fmNull;
        }
    }

    I32 fmVectorDeleteObj(fmVector *vector, void *obj)
    {
        for (I32 i = 0; i < vector->size; i++)
        {
            if (vector->data[i] == obj)
            {
                fmVectorDeleteObjByIndex(vector, i);
                return i;
            }
        }
        return -1;
    }

    //删除指定序号的对象
    void *fmVectorDeleteObjByIndex(fmVector *vector, I32 index)
    {
        if (index < 0 || index >= vector->size)
        {
            return fmNull;
        }
        void *obj = vector->data[index];
        vector->size--;
        vector->data[index] = vector->data[vector->size];
        vector->data[vector->size] = fmNull;
        return obj;
    }

    void fmVectorFreeEach(fmVector *vector, FREE_FUNC_PTR freeFunc)
    {
        if (vector != fmNull)
        {
            if (freeFunc != fmNull)
            {
                while (vector->size > 0)
                {
                    vector->size--;
                    freeFunc(vector->data[vector->size]);
                }
            }
            else
            {
                vector->size = 0;
            }
        }
    }

    fmBool fmVectorContains(fmVector *vector, void *ptr, EQUALS_FUNC_PTR equalsFunc)
    {
        for (U32 i = 0; i < vector->size; i++)
        {
            if (vector->data[i] == ptr)
            {
                return fmTrue;
            }
        }

        if (equalsFunc != fmNull)
        {
            for (U32 i = 0; i < vector->size; i++)
            {
                if (equalsFunc(vector->data[i], ptr))
                {
                    return fmTrue;
                }
            }
        }

        return fmFalse;
    }

    //克隆，不传克隆函数默认浅拷贝
    fmVector *fmVectorClone(fmVector *dst, fmVector *src, CLONE_FUNC_PTR dataCloneFunc)
    {
        if (src == fmNull)
        {
            return dst;
        }

        //没有传递目标则默认新建
        if (dst == fmNull)
        {
            dst = fmVectorNew(src->size);
        }

        //扩容
        fmVectorRecapacity(dst, src->size);
        dst->size = src->size;

        //拷贝数据，未指定拷贝函数则默认为浅拷贝
        if (dataCloneFunc == fmNull)
        {
            memcpy(dst->data, src->data, src->size * sizeof(void *));
        }
        else
        {
            for (U32 i = 0; i < src->size; i++)
            {
                dst->data[i] = dataCloneFunc(dst->data[i], src->data[i]);
            }
        }

        return dst;
    }

    //重新分配大小
    void fmVectorRecapacity(fmVector *vector, U32 capacity)
    {
        if (vector != fmNull && vector->capacity < capacity)
        {
            vector->capacity = capacity;
            vector->data = (void **)fmRealloc(vector->data, capacity * sizeof(void *));
            memset(vector->data + vector->size, 0, sizeof(void *) * (capacity - vector->size));
        }
    }

#ifdef FISHING_USE_CPP
}
#endif
