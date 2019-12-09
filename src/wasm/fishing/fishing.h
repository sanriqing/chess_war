#ifndef FISHING_H
#define FISHING_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef alloca
#ifdef _WIN32
#include <malloc.h>
#elif defined(__FreeBSD__)
/* already included in <stdlib.h> */
#else
#include <alloca.h>
#endif
#endif

#ifdef _WIN32
#define FM_EXPORT __declspec(dllexport)
#else
#define FM_EXPORT
#endif

#include "fishing_types.h"

#ifdef FISHING_USE_CPP
extern "C"
{
#endif

#ifndef fmCalloc
/// calloc() alias.
#define fmCalloc calloc
#endif

#ifndef fmRealloc
/// realloc() alias.
#define fmRealloc realloc
#endif

#ifndef fmFree
/// free() alias.
#define fmFree free
#endif

#ifdef FISHING_USE_CPP
}
#endif

//克隆函数指针类型
typedef void *(*CLONE_FUNC_PTR)(void *, void *);
//释放函数指针类型
typedef void (*FREE_FUNC_PTR)(void *);
//释放函数指针类型
typedef fmBool (*EQUALS_FUNC_PTR)(void *, void *);

#ifndef fmAbs
#define fmAbs(x) ((x) >= 0 ? (x) : -(x))
#endif

#ifndef fmMax
#define fmMax(x, y) ((x) >= (y) ? (x) : (y))
#endif

#ifndef fmMin
#define fmMin(x, y) ((x) <= (y) ? (x) : (y))
#endif

#endif
