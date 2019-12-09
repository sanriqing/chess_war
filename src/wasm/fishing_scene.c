#include "fishing/fishing_scene.h"
#include "fishing/fishing_actor.h"

#ifdef FISHING_USE_CPP
extern "C"
{
#endif

    I32 screenWidth = 0;
    I32 screenHeight = 0;

    //当前帧数
    static U32 frameCount = 0;

    //所有场景
    static U32 sceneConfirmFrameIndex = 0;
    //所有场景
    static fmVector *sceneList = fmNull;

    //产生新场景
    static fmScene *fmSceneNew(void)
    {
        fmScene *scene = (fmScene *)fmCalloc(1, sizeof(fmScene));
        scene->id = sceneList->size;
        fmVectorPush(sceneList, scene);

        scene->random = fmRandom32New(0);

        const U32 initSize = 128;
        scene->actors = fmVectorNew(initSize);
        for (U32 i = 0; i < FM_SCENE_ACTOR_BUFF_COUNT; i++)
        {
            scene->actorBuff[i] = fmVectorNew(initSize);
        }
        return scene;
    }

    //释放场景
    static void fmSceneFree(fmScene *scene)
    {
        if (scene == fmNull || scene->destroyed == fmTrue)
        {
            return;
        }
        scene->destroyed = fmTrue;
        fmVectorDeleteObj(sceneList, scene);

        fmRandom32Free(scene->random);
        scene->random = fmNull;

        fmVectorFreeEach(scene->actors, (FREE_FUNC_PTR)fmActorFree);
        fmVectorFree(scene->actors);
        scene->actors = fmNull;

        for (U32 i = 0; i < FM_SCENE_ACTOR_BUFF_COUNT; i++)
        {
            fmVectorFree(scene->actorBuff[i]);
            scene->actorBuff[i] = fmNull;
        }

        fmFree(scene);
    }

    //获取指定场景
    static fmScene *fmSceneGet(U32 frameIndex)
    {
        //寻找可重复利用的帧
        fmScene *selScene = fmNull;
        for (U32 i = 0; i < sceneList->size; i++)
        {
            fmScene *scene = (fmScene *)sceneList->data[i];
            if (scene->frameIndex < sceneConfirmFrameIndex - 1) //向前一些帧数方便做插值
            {
                selScene = scene;
                break;
            }
        }

        //查找上一帧数据
        for (U32 i = 0; i < sceneList->size; i++)
        {
            fmScene *scene = (fmScene *)sceneList->data[i];
            if (scene->frameIndex == frameIndex - 1)
            {
                selScene = fmSceneClone(selScene, scene); //拷贝数据
                break;
            }
        }

        //没有符合的当前帧就创新
        if (selScene == fmNull)
        {
            selScene = fmSceneNew();
        }

        selScene->frameIndex = frameIndex;

        return selScene;
    }

    //初始化
    void fmSceneInit(U32 seed)
    {
        //数据初始化
        frameCount = 0;
        sceneConfirmFrameIndex = 0;

        if (sceneList == fmNull)
        {
            sceneList = fmVectorNew(10);
        }

        //重置每个场景的数据
        for (U32 i = 0; i < sceneList->size; i++)
        {
            fmScene *scene = (fmScene *)sceneList->data[i];
            scene->frameIndex = 0;
            scene->maxActorUID = 0;
        }

        if (sceneList->size == 0)
        {
            fmSceneGet(0);
        }

        //设置随机数种子
        fmScene *scene = (fmScene *)sceneList->data[0];
        fmRandom32SetSeed(scene->random, seed);
    }

    //释放场景
    void fmSceneClear(void)
    {
        if (sceneList != fmNull)
        {
            while (sceneList->size > 0)
            {
                fmScene *scene = (fmScene *)fmVectorPop(sceneList);
                if (scene != fmNull)
                {
                    fmSceneFree(scene);
                }
            }
        }
    }

    //克隆场景
    fmScene *fmSceneClone(fmScene *dst, fmScene *src)
    {
        if (src == fmNull)
        {
            return dst;
        }
        if (dst == fmNull)
        {
            dst = fmSceneNew();
        }

        dst->maxActorUID = src->maxActorUID;
        dst->destroyed = src->destroyed;
        dst->frameIndex = src->frameIndex;
        fmRandom32Clone(dst->random, src->random);
        dst->actors = fmVectorClone(dst->actors, src->actors, (CLONE_FUNC_PTR)fmActorClone);

        //拷贝对应的缓冲队列
        for (U32 buffIdx = 0; buffIdx < FM_SCENE_ACTOR_BUFF_COUNT; buffIdx++)
        {
            fmVector *dstBuff = dst->actorBuff[buffIdx];
            fmVector *srcBuff = src->actorBuff[buffIdx];

            fmVectorRecapacity(dstBuff, srcBuff->size);
            dstBuff->size = srcBuff->size;

            for (U32 actorIdx = 0; actorIdx < srcBuff->size; actorIdx++)
            {
                fmActor *srcActor = (fmActor *)srcBuff->data[actorIdx];
                dstBuff->data[actorIdx] = dst->actors->data[srcActor->listIndex];
            }
        }

        return dst;
    }

    //获取一个新id
    U32 fmSceneGetUID(fmScene *scene)
    {
        return ++scene->maxActorUID;
    }

    //获取指定id的场景
    fmScene *fmSceneGetById(U16 sceneId)
    {
        for (U32 i = 0; i < sceneList->size; i++)
        {
            fmScene *scene = (fmScene *)sceneList->data[i];
            if (scene->id == sceneId)
            {
                return scene;
            }
        }
        return fmNull;
    }

    //已确认场景序号
    void fmSceneConfirm(U32 frameIndex)
    {
        sceneConfirmFrameIndex = frameIndex;
    }

    //更新场景
    void fmSceneUpdate(U32 frameIndex)
    {
        fmScene *scene = fmSceneGet(frameIndex);
        fmActorUpdate(scene);
    }

    //迭代更新场景
    void fmSceneStepUpdate(void)
    {
        ++frameCount;
        fmSceneUpdate(frameCount);

        //TODO 测试
        if (frameCount >= 5)
        {
            fmSceneConfirm(frameCount - 5);
        }
    }

#ifdef FISHING_USE_CPP
}
#endif