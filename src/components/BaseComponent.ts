import { Sprite, Texture } from 'pixi.js';
export interface BaseComponentsConfig {
    texture?: Texture;
}
export default class BaseComponents extends Sprite {
    public constructor(config: BaseComponentsConfig = {}) {
        super(config.texture);
    }
}
