// 整个舞台在event bus中的事件
export const enum EventStage {
    // 全面屏改变
    BANG_SCREEN_CHANGE = 'BANG_SCREEN_CHANGE'
}

/** 炮塔相关事件 */
export const enum EventBattery {
    /** 更改炮台弹窗显示 */
    BATTERY_CHANGE_MODAL_SHOW = 'BATTERY_CHANGE_MODAL_SHOW',
    /** 更改炮台 */
    BATTERY_CHANGE_BTN_TAP = 'BATTERY_CHANGE_BTN_TAP'
}
