#include <string.h>
#include "fishing/fishing_actor.h"
#include "fishing/fishing_random32.h"
#include "fishing/fishing_vector.h"
#include "fishing/fishing_interpolation.h"
#include "fishing/fishing_math.h"

#if FM_USE_SELF_NUM
#include "fishing/fishing_number.h"
#endif

#ifdef FISHING_USE_CPP
extern "C"
{
#endif

    //当前场景
    static fmScene *currScene = fmNull;

    //外部数据接口
    static fmFloat *outputInfo = fmNull;
    //外部数据接口
    static U32 *addInfo = fmNull;
    //外部数据接口
    static U32 *delInfo = fmNull;

    //自类型数量
    static U8 getActorSubTypeCount(U8 mainType)
    {
        return 1;
    }

    //获取一个演员(判断id是否重置)
    static fmActor *getOneActor(U8 mainType, U8 subType)
    {
        fmActor *actor = (fmActor *)fmVectorPop(currScene->actorBuff[FM_SCENE_ACTOR_BUFF_FREE]);
        if (actor == fmNull)
        {
            actor = (fmActor *)fmCalloc(1, sizeof(fmActor));
            actor->listIndex = currScene->actors->size;
            fmVectorPush(currScene->actors, actor);
        }

        fmVector *actorNew = currScene->actorBuff[FM_SCENE_ACTOR_BUFF_NEW];
        fmVector *actorAlive = currScene->actorBuff[FM_SCENE_ACTOR_BUFF_ALIVE];
        fmVectorPush(actorNew, actor);
        fmVectorPush(actorAlive, actor);

        //保留数据
        U32 listIdx = actor->listIndex;

        //格式化
        memset(actor, 0, sizeof(fmActor));
        actor->id = fmSceneGetUID(currScene);
        actor->createFrame = currScene->frameIndex;

        //恢复数据
        actor->listIndex = listIdx;

        return actor;
    }

    //导出数据
    static void outputData(void)
    {
        U32 idx = 0;
        fmFloat *od = outputInfo;

        fmVector *actorAlive = currScene->actorBuff[FM_SCENE_ACTOR_BUFF_ALIVE];
        for (U32 i = 0; i < actorAlive->size; i++)
        {
            fmActor *actor = (fmActor *)actorAlive->data[i];

            //复制数据
            od[idx + FM_ACTOR_DATA_ID] = actor->id;
            od[idx + FM_ACTOR_DATA_POS] = actor->pos;
            od[idx + FM_ACTOR_DATA_MAIN_TYPE] = actor->mainType;
            od[idx + FM_ACTOR_DATA_SUB_TYPE] = actor->subType;
            od[idx + FM_ACTOR_DATA_Z] = actor->z;

#if FM_USE_SELF_NUM

            od[idx + FM_ACTOR_DATA_X] = fmNumberToFloat(actor->x);
            od[idx + FM_ACTOR_DATA_Y] = fmNumberToFloat(actor->y);
            od[idx + FM_ACTOR_DATA_SCALE] = fmNumberToFloat(actor->scale);
            od[idx + FM_ACTOR_DATA_ROTATION] = actor->rotation;
            od[idx + FM_ACTOR_DATA_ALPHA] = fmNumberToFloat(actor->alpha);
            od[idx + FM_ACTOR_DATA_TINT] = fmNumberToFloat(actor->tint);

#else

        od[idx + FM_ACTOR_DATA_X] = actor->x;
        od[idx + FM_ACTOR_DATA_Y] = actor->y;
        od[idx + FM_ACTOR_DATA_SCALE] = actor->scale;
        od[idx + FM_ACTOR_DATA_ROTATION] = actor->rotation;
        od[idx + FM_ACTOR_DATA_ALPHA] = actor->alpha;
        od[idx + FM_ACTOR_DATA_TINT] = actor->tint;

#endif

            //偏移
            idx += FM_ACTOR_DATA_COUNT;
        }

        //新增
        fmVector *actorNew = currScene->actorBuff[FM_SCENE_ACTOR_BUFF_NEW];
        addInfo[0] = actorNew->size;
        for (U32 i = 0; i < actorNew->size; i++)
        {
            fmActor *actor = (fmActor *)actorNew->data[i];
            addInfo[i + 1] = actor->id;
        }

        //死亡
        fmVector *actorDead = currScene->actorBuff[FM_SCENE_ACTOR_BUFF_DEAD];
        delInfo[0] = actorDead->size;
        for (U32 i = 0; i < actorDead->size; i++)
        {
            fmActor *actor = (fmActor *)actorDead->data[i];
            delInfo[i + 1] = actor->id;
        }
    }

    //初始化数据
    static void initDatas()
    {
        addInfo = (U32 *)fmCalloc(1 + MAX_ACTOR_SIZE, sizeof(U32));
        delInfo = (U32 *)fmCalloc(1 + MAX_ACTOR_SIZE, sizeof(U32));
        outputInfo = (fmFloat *)fmCalloc(FM_ACTOR_DATA_COUNT * MAX_ACTOR_SIZE, sizeof(fmFloat));
    }

    ///init actors
    fmFloat *fmActorInit(void)
    {
        if (outputInfo == fmNull)
        {
            initDatas();
        }
        return outputInfo;
    }

    U32 *fmActorGetAddInfo(void)
    {
        return addInfo;
    }

    U32 *fmActorGetDelInfo(void)
    {
        return delInfo;
    }

    U32 fmActorGetAliveCount(void)
    {
        fmVector *actorAlive = currScene->actorBuff[FM_SCENE_ACTOR_BUFF_ALIVE];
        return actorAlive->size;
    }

    //add one actor to scene
    fmActor *fmActorAdd(U8 mainType, U8 subType)
    {
        fmActor *actor = getOneActor(mainType, subType);

        actor->mainType = mainType;
        actor->subType = subType;

#if FM_USE_SELF_NUM

        actor->alpha = fmNumberInit(1);
        actor->scale = fmNumberInit(1);
        actor->speed = fmNumberInit(520);
        actor->tint = fmNumberInit(-1);

#else

    actor->alpha = 1;
    actor->scale = 1;
    actor->speed = 520;
    actor->tint = -1;

#endif

        return actor;
    }

    ///remove actor frome scene
    fmBool fmActorRemove(fmActor *actor)
    {
        if (actor != fmNull && actor->dead == fmFalse)
        {
            fmVector *actorAlive = currScene->actorBuff[FM_SCENE_ACTOR_BUFF_ALIVE];
            fmVector *actorDead = currScene->actorBuff[FM_SCENE_ACTOR_BUFF_DEAD];

            actor->dead = fmTrue;
            actor->deadFrame = currScene->frameIndex;
            fmVectorPush(actorDead, actor);
            fmVectorDeleteObj(actorAlive, actor);
            return fmTrue;
        }
        return fmFalse;
    }

    void fmActorFree(fmActor *actor)
    {
        if (actor != fmNull)
        {
            fmFree(actor);
        }
    }

    fmActor *fmActorClone(fmActor *dst, fmActor *src)
    {
        if (src == fmNull)
        {
            return dst;
        }
        if (dst == fmNull)
        {
            dst = (fmActor *)malloc(sizeof(fmActor));
        }
        memcpy(dst, src, sizeof(fmActor));
        return dst;
    }

    ///step update all alive actors (dt = 16ms)
    void fmActorUpdate(fmScene *scene)
    {
        currScene = scene;
        fmVector *actorAlive = currScene->actorBuff[FM_SCENE_ACTOR_BUFF_ALIVE];
        fmVector *actorFree = currScene->actorBuff[FM_SCENE_ACTOR_BUFF_FREE];
        fmVector *actorDead = currScene->actorBuff[FM_SCENE_ACTOR_BUFF_DEAD];

        //上一帧死亡的放到空闲中，清空死亡数组
        for (U32 i = 0; i < actorDead->size; i++)
        {
            fmVectorPush(actorFree, actorDead->data[i]);
        }
        fmVectorFreeEach(actorDead, fmNull);

        //碰撞检测

        //联网指令

        //TODO 测试逻辑
        const I32 POINT_COUNT = 8;
        const I32 INFO_COUNT = 3;

#if FM_USE_SELF_NUM
        fmData sw = fmNumberInit(screenWidth);
        fmData sh = fmNumberInit(screenHeight);
#endif

        if (actorAlive->size < 960)
        {
            fmActor *curr = fmActorAdd(0, fmRandom32Zone(currScene->random, 0, 30));
            curr->z = fmRandom32Zone(currScene->random, 1, 60);

            //产生坐标点
            fmData *pos = curr->posInfo;
            I32 randomX1 = 60;
            I32 randomY1 = 60;
            I32 randomX2 = screenWidth / 2 - randomX1;
            I32 randomY2 = screenHeight / 2 - randomY1;
            for (U32 posIdx = 0; posIdx < POINT_COUNT; posIdx++)
            {
                U32 dataIdx = posIdx * INFO_COUNT;
#if FM_USE_SELF_NUM
                switch (posIdx)
                {
                case 0:
                case 1:
                    pos[dataIdx] = fmNumberInit(fmRandom32Zone(currScene->random, -200, -100));
                    pos[dataIdx + 1] = fmNumberInit(fmRandom32Zone(currScene->random, -200, -100));
                    break;

                case 2:
                    pos[dataIdx] = fmNumberInit(fmRandom32Zone(currScene->random, randomX1, randomX2));
                    pos[dataIdx + 1] = fmNumberInit(fmRandom32Zone(currScene->random, randomY1, randomY2));
                    break;

                case 3:
                    pos[dataIdx] = fmNumberInit(fmRandom32Zone(currScene->random, screenWidth - randomX1, screenWidth - randomX2));
                    pos[dataIdx + 1] = fmNumberInit(fmRandom32Zone(currScene->random, randomY1, randomY2));
                    break;

                case 4:
                    pos[dataIdx] = fmNumberInit(fmRandom32Zone(currScene->random, screenWidth - randomX1, screenWidth - randomX2));
                    pos[dataIdx + 1] = fmNumberInit(fmRandom32Zone(currScene->random, screenHeight - randomY1, screenHeight - randomY2));
                    break;

                case 5:
                    pos[dataIdx] = fmNumberInit(fmRandom32Zone(currScene->random, randomX1, randomX2));
                    pos[dataIdx + 1] = fmNumberInit(fmRandom32Zone(currScene->random, screenHeight - randomY1, screenHeight - randomY2));
                    break;

                case 6:
                case 7:
                    pos[dataIdx] = fmNumberInit(fmRandom32Zone(currScene->random, -200, -100));
                    pos[dataIdx + 1] = fmNumberInit(fmRandom32Zone(currScene->random, screenHeight + 100, screenHeight + 200));
                    break;
                }

#else

            switch (posIdx)
            {
            case 0:
            case 1:
                pos[dataIdx] = fmRandom32Zone(currScene->random, -200, -100);
                pos[dataIdx + 1] = fmRandom32Zone(currScene->random, -200, -100);
                break;

            case 2:
                pos[dataIdx] = fmRandom32Zone(currScene->random, randomX1, randomX2);
                pos[dataIdx + 1] = fmRandom32Zone(currScene->random, randomY1, randomY2);
                break;

            case 3:
                pos[dataIdx] = fmRandom32Zone(currScene->random, screenWidth - randomX1, screenWidth - randomX2);
                pos[dataIdx + 1] = fmRandom32Zone(currScene->random, randomY1, randomY2);
                break;

            case 4:
                pos[dataIdx] = fmRandom32Zone(currScene->random, screenWidth - randomX1, screenWidth - randomX2);
                pos[dataIdx + 1] = fmRandom32Zone(currScene->random, screenHeight - randomY1, screenHeight - randomY2);
                break;

            case 5:
                pos[dataIdx] = fmRandom32Zone(currScene->random, randomX1, randomX2);
                pos[dataIdx + 1] = fmRandom32Zone(currScene->random, screenHeight - randomY1, screenHeight - randomY2);
                break;

            case 6:
            case 7:
                pos[dataIdx] = fmRandom32Zone(currScene->random, -200, -100);
                pos[dataIdx + 1] = fmRandom32Zone(currScene->random, screenHeight + 100, screenHeight + 200);
                break;
            }
#endif
            }

            //计算直线距离
            for (U32 posIdx = 0; posIdx < POINT_COUNT - 1; posIdx++)
            {
                U32 dataIdx = posIdx * INFO_COUNT;
#if FM_USE_SELF_NUM
                fmData dx = fmNumberSub(pos[dataIdx + INFO_COUNT], pos[dataIdx]);
                fmData dy = fmNumberSub(pos[dataIdx + 1 + INFO_COUNT], pos[dataIdx + 1]);
                pos[dataIdx + 2] = fmNumberSqrt2(dx, dy);
#else
            fmData dx = pos[dataIdx + INFO_COUNT] - pos[dataIdx];
            fmData dy = pos[dataIdx + 1 + INFO_COUNT] - pos[dataIdx + 1];
            pos[dataIdx + 2] = fmSqrt2(dx, dy);
#endif
            }

            //左右镜像
            if (fmRandom32Zone(currScene->random, 0, 100) <= 50)
            {
                for (U32 posIdx = 0; posIdx < POINT_COUNT; posIdx++)
                {
                    U32 dataIdx = posIdx * INFO_COUNT;
#if FM_USE_SELF_NUM
                    pos[dataIdx] = fmNumberSub(sw, pos[dataIdx]);
#else
                pos[dataIdx] = screenWidth - pos[dataIdx];
#endif
                }
            }
            //上下镜像
            if (fmRandom32Zone(currScene->random, 0, 100) <= 50)
            {
                for (U32 posIdx = 0; posIdx < POINT_COUNT; posIdx++)
                {
                    U32 dataIdx = posIdx * INFO_COUNT + 1;
#if FM_USE_SELF_NUM
                    pos[dataIdx] = fmNumberSub(sh, pos[dataIdx]);
#else
                pos[dataIdx] = screenHeight - pos[dataIdx];
#endif
                }
            }
        }

        //状态更新，并生成下一帧用的碰撞范围
        for (U32 i = 0; i < actorAlive->size; i++)
        {
            fmActor *actor = (fmActor *)actorAlive->data[i];
            if (actor->mainType == 1)
            {
#if FM_USE_SELF_NUM
                actor->x = fmNumberAdd(actor->x, actor->bullet_dx);
                actor->y = fmNumberAdd(actor->y, actor->bullet_dy);

                if ((fmNumberLess0(actor->bullet_dx) && fmNumberLess0(actor->x)) || (fmNumberGreater0(actor->bullet_dx) && fmNumberGreater(actor->x, sw)))
                {
                    actor->bullet_dx.negative = !actor->bullet_dx.negative;
                    actor->bullet_wallCollision++;
                }
                else if ((fmNumberLess0(actor->bullet_dy) && fmNumberLess0(actor->y)) || (fmNumberGreater0(actor->bullet_dy) && fmNumberGreater(actor->y, sh)))
                {
                    actor->bullet_dy.negative = !actor->bullet_dy.negative;
                    actor->bullet_wallCollision++;
                }

                if (actor->bullet_wallCollision > 4)
                {
                    fmActorRemove(actor);
                    i--;
                }
                else
                {
                   actor->rotation = fmAtan2(fmNumberToFloat(actor->bullet_dy), fmNumberToFloat(actor->bullet_dx)) + PI / 2;
                }
#else
            actor->x = actor->x + actor->bullet_dx;
            actor->y = actor->y + actor->bullet_dy;

            if ((actor->bullet_dx < 0 && actor->x < 0) || (actor->bullet_dx > 0 && actor->x > screenWidth))
            {
                actor->bullet_dx = -actor->bullet_dx;
                actor->bullet_wallCollision++;
            }
            else if ((actor->bullet_dy < 0 && actor->y < 0) || (actor->bullet_dy > 0 && actor->y > screenHeight))
            {
                actor->bullet_dy = -actor->bullet_dy;
                actor->bullet_wallCollision++;
            }

            if (actor->bullet_wallCollision > 4)
            {
                fmActorRemove(actor);
                i--;
            }
            else
            {
                actor->rotation = fmAtan2(actor->bullet_dy, actor->bullet_dx) + PI / 2;
            }
#endif
                continue;
            }

            actor->posTime++;

            I32 idx = -1;
            U32 dataIdx = 0;

#if FM_USE_SELF_NUM
            fmData allTime = fmNumberInit(0);
            fmData currTime = fmNumberInit(actor->posTime);
            fmData *infos = actor->posInfo;

            for (U32 posIdx = 1; posIdx < POINT_COUNT - 1; posIdx++)
            {
                dataIdx = posIdx * INFO_COUNT + 2;
                if (fmNumberGreater(infos[dataIdx], currTime))
                {
                    idx = posIdx;
                    allTime = infos[dataIdx];
                    break;
                }
                currTime = fmNumberSub(currTime, infos[dataIdx]);
            }

            if (idx == -1)
            {
                fmActorRemove(actor);
                i--;
                continue;
            }

            dataIdx = idx * INFO_COUNT;

            fmData t = fmNumberDiv(currTime, allTime);
            fmData x = fmCatmullRom(infos[dataIdx - INFO_COUNT], infos[dataIdx], infos[dataIdx + INFO_COUNT], infos[dataIdx + 2 * INFO_COUNT], t);
            dataIdx++;
            fmData y = fmCatmullRom(infos[dataIdx - INFO_COUNT], infos[dataIdx], infos[dataIdx + INFO_COUNT], infos[dataIdx + 2 * INFO_COUNT], t);
            fmData dx = fmNumberSub(x, actor->x);
            fmData dy = fmNumberSub(y, actor->y);
            actor->x = x;
            actor->y = y;
            actor->rotation = fmAtan2(fmNumberToFloat(dy), fmNumberToFloat(dx)) + PI / 2;

#else

        fmData allTime = 0;
        fmData currTime = actor->posTime;
        fmData *infos = actor->posInfo;

        for (U32 posIdx = 1; posIdx < POINT_COUNT - 1; posIdx++)
        {
            dataIdx = posIdx * INFO_COUNT + 2;
            if (infos[dataIdx] > currTime)
            {
                idx = posIdx;
                allTime = infos[dataIdx];
                break;
            }
            currTime = currTime - infos[dataIdx];
        }

        if (idx == -1)
        {
            fmActorRemove(actor);
            i--;
            continue;
        }

        dataIdx = idx * INFO_COUNT;

        fmData t = currTime / allTime;
        fmData x = fmCatmullRom(infos[dataIdx - INFO_COUNT], infos[dataIdx], infos[dataIdx + INFO_COUNT], infos[dataIdx + 2 * INFO_COUNT], t);
        dataIdx++;
        fmData y = fmCatmullRom(infos[dataIdx - INFO_COUNT], infos[dataIdx], infos[dataIdx + INFO_COUNT], infos[dataIdx + 2 * INFO_COUNT], t);
        fmData dx = x - actor->x;
        fmData dy = y - actor->y;
        actor->x = x;
        actor->y = y;
        actor->rotation = fmAtan2(dy, dx) + PI / 2;

#endif
        }

        //导出数据
        outputData();

        //清空新增
        fmVector *actorNew = currScene->actorBuff[FM_SCENE_ACTOR_BUFF_NEW];
        fmVectorFreeEach(actorNew, fmNull);
    }

#ifdef FISHING_USE_CPP
}
#endif