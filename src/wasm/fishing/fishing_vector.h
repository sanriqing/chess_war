#ifndef FISHING_VECTOR_H
#define FISHING_VECTOR_H

#include "fishing.h"

//容器
typedef struct _fishing_vector
{
    //数量
    U32 size;
    //容量
    U32 capacity;
    //数据
    void **data;
} fmVector;

//创建一个容器
FM_EXPORT fmVector *fmVectorNew(U32 capacity);
//删除一个容器
FM_EXPORT void fmVectorFree(fmVector *vector);
//压入对象
FM_EXPORT void fmVectorPush(fmVector *arr, void *object);
//弹出对象
FM_EXPORT void *fmVectorPop(fmVector *vector);
//删除指定对象
FM_EXPORT I32 fmVectorDeleteObj(fmVector *vector, void *obj);
//删除指定序号的对象
FM_EXPORT void *fmVectorDeleteObjByIndex(fmVector *vector, I32 index);
//是否包含
FM_EXPORT fmBool fmVectorContains(fmVector *vector, void *ptr, EQUALS_FUNC_PTR equalsFunc);
//自定义释放所有
FM_EXPORT void fmVectorFreeEach(fmVector *vector, FREE_FUNC_PTR freeFunc);
//拷贝，不传克隆函数默认浅拷贝
FM_EXPORT fmVector *fmVectorClone(fmVector *dst, fmVector *src, CLONE_FUNC_PTR dataCloneFunc);
//重新分配大小
FM_EXPORT void fmVectorRecapacity(fmVector *vector, U32 capacity);

#endif
