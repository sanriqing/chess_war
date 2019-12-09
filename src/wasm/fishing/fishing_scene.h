#ifndef FM_SCENE_H
#define FM_SCENE_H

#include "fishing.h"
#include "fishing_random32.h"
#include "fishing_vector.h"

enum
{
    /**活着*/
    FM_SCENE_ACTOR_BUFF_ALIVE,
    /**自由*/
    FM_SCENE_ACTOR_BUFF_FREE,
    /**新创*/
    FM_SCENE_ACTOR_BUFF_NEW,
    /**死亡*/
    FM_SCENE_ACTOR_BUFF_DEAD,
    /**数量*/
    FM_SCENE_ACTOR_BUFF_COUNT
};

//场景
typedef struct _fm_scene
{
    //场景ID
    U16 id;
    //已分配的最大演员ID
    U32 maxActorUID;
    //已释放
    fmBool destroyed;
    //隶属帧数
    U32 frameIndex;
    //随机数
    fmRandom32 *random;
    //所有演员（只增不减）
    fmVector *actors;
    //存活演员
    fmVector *actorBuff[FM_SCENE_ACTOR_BUFF_COUNT];

    //预测操作

} fmScene;

//屏幕宽
extern I32 screenWidth;
//屏幕高
extern I32 screenHeight;

//初始化
FM_EXPORT void fmSceneInit(U32 seed);
//释放场景
FM_EXPORT void fmSceneClear(void);
//确认场景
FM_EXPORT void fmSceneConfirm(U32 frameIndex);
//更新场景
FM_EXPORT void fmSceneUpdate(U32 frameIndex);
//迭代更新场景
FM_EXPORT void fmSceneStepUpdate(void);
//克隆场景
FM_EXPORT fmScene *fmSceneClone(fmScene *dst, fmScene *src);
//获取指定id的场景
FM_EXPORT fmScene *fmSceneGetById(U16 sceneId);
//获取一个新id
FM_EXPORT U32 fmSceneGetUID(fmScene *scene);

#endif