import extend from 'lodash/extend';
import { Graphics } from 'pixi.js';
export interface BaseRoundRectInterface {
    lineStyle?: {
        width?: number;
        color?: number;
        alpha?: number;
        alignment?: number;
        native?: boolean;
    };
    boxStyle?: {
        fillColor?: number;
        x?: number;
        y?: number;
        width?: number;
        height?: number;
        radius?: number;
        fillAlpha?: number;
    };
}

export default class BaseRoundRect extends Graphics {
    public constructor(config: BaseRoundRectInterface = {}) {
        super();
        const defaultConfig: Required<BaseRoundRectInterface> = {
            lineStyle: {
                color: 0x000000,
                alpha: 1
            },
            boxStyle: {
                fillColor: 0xFFFFFF,
                fillAlpha: 0,
                x: 0,
                y: 0,
                width: 100,
                height: 100,
                radius: 0
            }
        };
        const realConfig = extend(defaultConfig, config);
        const { lineStyle, boxStyle } = realConfig;
        this.lineStyle(lineStyle.width, lineStyle.color, lineStyle.alpha, lineStyle.alignment, lineStyle.native);
        this.beginFill(boxStyle.fillColor, boxStyle.fillAlpha);
        this.drawRoundedRect(boxStyle.x!, boxStyle.y!, boxStyle.width!, boxStyle.height!, boxStyle.radius!);
        this.endFill();
    }
}
