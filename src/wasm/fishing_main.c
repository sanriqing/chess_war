// #define IN_DEBUG
#include "fishing/fishing_random32.h"
#include "fishing/fishing_actor.h"
#include "fishing/fishing_interpolation.h"
#include "fishing/fishing_number.h"

#ifndef IN_DEBUG
#include <emscripten/emscripten.h>
#endif

//兼容直接测试
#ifndef EMSCRIPTEN_KEEPALIVE
#define EMSCRIPTEN_KEEPALIVE
#endif

#ifdef FISHING_USE_CPP
extern "C"
{
#endif

  //初始化
  fmPtr EMSCRIPTEN_KEEPALIVE init(I32 w, I32 h, U32 seed)
  {
    screenWidth = w;
    screenHeight = h;
    fmInterpolationinit();

    fmSceneInit(seed);
    fmFloat *ptr = fmActorInit();

    return (fmPtr)ptr;
  }

  //获取添加指针
  fmPtr EMSCRIPTEN_KEEPALIVE getAddPtr(void)
  {
    return (fmPtr)fmActorGetAddInfo();
  }

  //获取移除指针
  fmPtr EMSCRIPTEN_KEEPALIVE getDelPtr(void)
  {
    return (fmPtr)fmActorGetDelInfo();
  }

  //当前帧演员数
  U32 EMSCRIPTEN_KEEPALIVE getMaxActorSize()
  {
    return MAX_ACTOR_SIZE;
  }

  //引擎步进
  void EMSCRIPTEN_KEEPALIVE update()
  {
    fmSceneStepUpdate();
  }

  //当前帧演员数
  U32 EMSCRIPTEN_KEEPALIVE getActorCount()
  {
    return fmActorGetAliveCount();
  }

  //当前演员属性数
  U32 EMSCRIPTEN_KEEPALIVE getAttributeCount()
  {
    return FM_ACTOR_DATA_COUNT;
  }

  //发射子弹
  void EMSCRIPTEN_KEEPALIVE fire(I32 pos, I32 x, I32 y, U32 speed)
  {
    fmActor *actor = fmActorAdd(1, 0);
    actor->z = 99;

    I32 px = (pos == 0 || pos == 3) ? (screenWidth / 4) : (screenWidth * 3 / 4);
    I32 py = (pos == 0 || pos == 1) ? (screenHeight) : 0;

#if FM_USE_SELF_NUM

    fmData dx = fmNumberInit(x - px);
    fmData dy = fmNumberInit(y - py);
    fmData all = fmNumberAdd(fmNumberAbs(dx), fmNumberAbs(dy));
    fmData bulleSpeed = fmNumberDiv(fmNumberInitWithDown(speed > 0 ? speed : 600, 60), all);

    actor->bullet_wallCollision = 0;
    actor->bullet_dx = fmNumberMul(dx, bulleSpeed);
    actor->bullet_dy = fmNumberMul(dy, bulleSpeed);
    actor->x = fmNumberInit(px);
    actor->y = fmNumberInit(py);

#else

  fmData dx = x - px;
  fmData dy = y - py;
  fmData all = fmAbs(dx) + fmAbs(dy);

  fmData bulleSpeed = speed > 0 ? speed : 600;
  bulleSpeed = bulleSpeed / 60;

  actor->bullet_wallCollision = 0;
  actor->bullet_dx = bulleSpeed * dx / all;
  actor->bullet_dy = bulleSpeed * dy / all;
  actor->x = px;
  actor->y = py;

#endif
  }

#ifdef FISHING_USE_CPP
}
#endif

//锁定
void EMSCRIPTEN_KEEPALIVE lock() //I32 pos, I32 x, I32 y
{
}

I32 main()
{
  printf("\n\n\n------------------\n");

#ifdef IN_DEBUG
  init(1241, 640, 0);

  for (int i = 0; i < 60 * 100000; i++)
  {
    update();
    if (i == 0)
    {
      //  fire(2, 742, 433, 600);
    }
  }
#endif

  // fmNumber v1 = fmNumberInitWithDown(19, 4);
  // fmNumber v2 = fmNumberInitWithDown(6, 4);

  // fmNumber v3 = fmNumberMul(v1, v2);
  // fmNumber v4 = fmNumberDiv(v1, v2);

  // fmNumber rn = v4;
  // fmFloat r = fmNumberToFloat(rn);
  // printf("%f\n", r);
  //
  printf("\n------------------\n\n\n");

  return 0;
}
