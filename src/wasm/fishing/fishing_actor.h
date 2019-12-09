#ifndef FM_ACTOR_H
#define FM_ACTOR_H

#include "fishing.h"
#include "fishing_scene.h"

//最多对象数量
#define MAX_ACTOR_SIZE 1024

/** 数据顺序*/
typedef enum
{
    /**标志*/
    FM_ACTOR_DATA_ID,
    /**位置*/
    FM_ACTOR_DATA_POS,
    /**主类型*/
    FM_ACTOR_DATA_MAIN_TYPE,
    /**子类型*/
    FM_ACTOR_DATA_SUB_TYPE,
    /**X坐标malloc*/
    FM_ACTOR_DATA_X,
    /**Y坐标*/
    FM_ACTOR_DATA_Y,
    /**景深*/
    FM_ACTOR_DATA_Z,
    /**缩放*/
    FM_ACTOR_DATA_SCALE,
    /**旋转角度*/
    FM_ACTOR_DATA_ROTATION,
    /**透明度*/
    FM_ACTOR_DATA_ALPHA,
    /**变色 < 0代表没有*/
    FM_ACTOR_DATA_TINT,

    /**数据总数量*/
    FM_ACTOR_DATA_COUNT
} FM_ACTOR_DATA;

/** 演员主类别*/
typedef enum
{
    //炮台
    ACTOR_MAIN_TYPE_CANNON,
    //子弹
    ACTOR_MAIN_TYPE_BULLET,
    //鱼
    ACTOR_MAIN_TYPE_FISH,
    //类别数量
    ACTOR_MAIN_TYPE_COUNT
} ACTOR_MAIN_TYPE;

//场景物件结构体
typedef struct _fm_actor
{
    //////////////////////导出数据

    //id
    U32 id;
    //玩家方位 < 0 鱼群
    I8 pos;
    //主类型
    U8 mainType;
    //子类型
    U8 subType;
    //x坐标
    fmData x;
    //y坐标
    fmData y;
    //z index
    U16 z;
    //缩放
    fmData scale;
    //旋转
    fmFloat rotation;
    //透明度
    fmData alpha;
    //颜色 < 0 代表没变色
    fmData tint;

    //////////////////////逻辑数据

    //速度
    fmData speed;

    //死亡
    fmBool dead;
    //创建帧(死亡后没有马上复活就重置)
    U32 createFrame;
    //死亡帧
    U32 deadFrame;

    ////////////////////////辅助数据

    //数组坐标
    U16 listIndex;

    //位置时间
    I32 posTime;
    //位置信息 x,y,time
    fmData posInfo[3 * 8];
    I32 bullet_wallCollision;

    fmData bullet_dx;
    fmData bullet_dy;

} fmActor;

//init actors
FM_EXPORT fmFloat *fmActorInit(void);
//step update all alive actors
FM_EXPORT void fmActorUpdate(fmScene *scene);
//get how many new actor
FM_EXPORT U32 *fmActorGetAddInfo(void);
//get how many del actor
FM_EXPORT U32 *fmActorGetDelInfo(void);
//get how many alive actor
FM_EXPORT U32 fmActorGetAliveCount(void);

//add one actor to scene
FM_EXPORT fmActor *fmActorAdd(U8 mainType, U8 subType);
//remove actor and add it to buf
FM_EXPORT fmBool fmActorRemove(fmActor *actor);
//free actor
FM_EXPORT void fmActorFree(fmActor *actor);
//clone actor
FM_EXPORT fmActor *fmActorClone(fmActor *dst, fmActor *src);

#endif
