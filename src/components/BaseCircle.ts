import BaseRoundRect, { BaseRoundRectInterface } from './BaseRoundRect';


export interface BaseCircleInterface {
    lineStyle?: BaseRoundRectInterface['lineStyle'];
    x: number;
    y: number;
    r: number;
}
export default class BaseCircle extends BaseRoundRect {
    public constructor(config: BaseCircleInterface = { x: 0, y: 0, r: 0 }) {
        super({
            lineStyle: config.lineStyle,
            boxStyle: {
                x: config.x + config.r / 2,
                y: config.y + config.r / 2

            }
        });
    }
}
