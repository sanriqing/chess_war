// @ts-ignore
export const IS_DEV = process.env.NODE_ENV === 'development';
// export const IS_DEV = false;

/** 现存模块 */
export const enum ExistModule {
    DEMO = 'DEMO'
}

export const enum LocalStorageKey {
    // 是否是刘海屏
    IS_BANG_SCREEN_KEY = 'IS_BANG_SCREEN_KEY',
    // 音乐音量
    MUSIC_VOLUME_KEY = 'MUSIC_VOLUME_KEY',
    // 背景音乐音量
    BG_MUSIC_VOLUME_KEY = 'BG_MUSIC_VOLUME_KEY',
    // 特效按钮
    SPECIAL_EFFECTS_KEY = 'SPECIAL_EFFECTS_KEY'
}

const cacheSettingValue: Partial<typeof SettingConfig> = {};
export const SettingConfig = {
    // 是否是刘海屏幕
    get isBangScreen(): boolean {
        if (cacheSettingValue.isBangScreen !== undefined) {
            return cacheSettingValue.isBangScreen!;
        }
        const val = !!+(localStorage.getItem(LocalStorageKey.IS_BANG_SCREEN_KEY) || 0);
        cacheSettingValue.isBangScreen = val;
        return val;
    },
    set isBangScreen(val) {
        localStorage.setItem(LocalStorageKey.IS_BANG_SCREEN_KEY, val ? '1' : '0');
        cacheSettingValue.isBangScreen = val;
    },
    // 是否关闭特效
    get isOpenSpecialEffects(): boolean {
        if (cacheSettingValue.isOpenSpecialEffects !== undefined) {
            return cacheSettingValue.isOpenSpecialEffects!;
        }
        const val = !!+(localStorage.getItem(LocalStorageKey.SPECIAL_EFFECTS_KEY) || 1);
        cacheSettingValue.isOpenSpecialEffects = val;
        return val;
    },
    set isOpenSpecialEffects(val) {
        localStorage.setItem(LocalStorageKey.SPECIAL_EFFECTS_KEY, val ? '1' : '0');
        cacheSettingValue.isOpenSpecialEffects = val;
    },
    // 音乐音量
    get musicVolume(): number {
        if (cacheSettingValue.musicVolume !== undefined) {
            return cacheSettingValue.musicVolume!;
        }
        const val = +(localStorage.getItem(LocalStorageKey.MUSIC_VOLUME_KEY) || 50);
        cacheSettingValue.musicVolume = val;
        return val;
    },
    set musicVolume(val) {
        localStorage.setItem(LocalStorageKey.MUSIC_VOLUME_KEY, val ? '1' : '0');
        cacheSettingValue.musicVolume = val;
    },
    // 背景音量音量
    get bgMusicVolume(): number {
        if (cacheSettingValue.bgMusicVolume !== undefined) {
            return cacheSettingValue.bgMusicVolume!;
        }
        const val = +(localStorage.getItem(LocalStorageKey.BG_MUSIC_VOLUME_KEY) || 50);
        cacheSettingValue.bgMusicVolume = val;
        return val;
    },
    set bgMusicVolume(val) {
        localStorage.setItem(LocalStorageKey.BG_MUSIC_VOLUME_KEY, val ? '1' : '0');
        cacheSettingValue.bgMusicVolume = val;
    }
};


export const bangScreenPadding = 30;

export const ScreenSize = IS_DEV ? {
    width: window.screen.width,
    height: window.screen.height
} : {
    height: window.screen.width,
    width:  window.screen.height
} as const;

/** ui稿尺寸 */
export const UISize = {
    width: 1176,
    height: 660
} as const;

/** 缩放比率 */
export const ScaleRate = ScreenSize.width / UISize.width!;

/** 游戏画布尺寸 */
export const GameSize = {
    width: UISize.width,
    height: ScreenSize.height / ScaleRate
} as const;


export const enum Position {
    TOP,
    LEFT,
    RIGHT,
    BOTTOM
}


export const enum BatteryType {
    COMMON1,
    COMMON2,
    COMMON3,
    COMMON4,
    COMMON5,
    COMMON6,
    COMMON7
}


export const enum BatteryEquipStatus {
    /** 装备 */
    EQUIP,
    /** 获取 */
    GET,
    /** 切换 */
    CHANGE
}

/** 聊天事件类型 */
export const enum ChatEventType {
    HAPPY,
    CRY,
    ANGRY,
    GOOD,
    TEXT
}
