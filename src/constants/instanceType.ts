/** 鱼，子弹等类的type属性 */

export interface MainType2SubTypeDic {
    [MAIN_TYPE.FISH]: FISH_SUB_TYPE;
    [MAIN_TYPE.BULLET]: BULLET_SUB_TYPE;
    [MAIN_TYPE.BATTERY]: BATTERY_SUB_TYPE;
}

export type MainTypeMap<T extends MAIN_TYPE> = T extends keyof MAIN_TYPE ? MainType2SubTypeDic[T] : any;

// 主类型
export const enum MAIN_TYPE {
    /** 鱼 */
    FISH,
    /** 子弹 */
    BULLET,
    /** 金币 */
    COIN,
    /** 子弹火焰 */
    BULLET_FIRE,
    /** 渔网 */
    NET,
    /** 锁定的瞄准器 */
    GUN_SIGHT,
    /** 锁定的原点 */
    GUN_SIGHT_CIRCLE,
    /** 炮台 */
    BATTERY
}

export const enum BATTERY_SUB_TYPE {
    BATTERY0,
    BATTERY1,
    BATTERY2,
    BATTERY3
}

/** 子弹的子类型 */
export const enum BULLET_SUB_TYPE {
    BULLET1,
    BULLET2,
    BULLET3,
    BULLET4,
    BULLET5
}

/** 鱼的子类型 */
export const enum FISH_SUB_TYPE {
    FISH0 = 0,
    FISH1 = 1,
    FISH2 = 2,
    FISH3 = 3,
    FISH4 = 4,
    FISH5 = 5,
    FISH6 = 6,
    FISH7 = 7,
    FISH8 = 8,
    FISH9 = 9,
    FISH10 = 10,
    FISH11 = 11,
    FISH12 = 12,
    FISH13 = 13,
    FISH14 = 14,
    FISH15 = 15,
    FISH16 = 16,
    FISH17 = 17,
    FISH18 = 18,
    FISH21 = 21,
    FISH23 = 23,
    FISH24 = 24,
    FISH25 = 25,
    FISH26 = 26,
    FISH27 = 27,
    FISH28 = 28,
    FISH29 = 29,
    FISH30 = 30,
    FISH31 = 31,
    FISH32 = 32
}
