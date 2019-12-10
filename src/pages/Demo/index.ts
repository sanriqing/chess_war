import { Sprite, Texture, TilingSprite } from 'pixi.js';
import BaseScene from '~/components/BaseScene';
import { ExistModule, GameSize } from '~/constants';
import { NotReadOnly } from '~/util/typeUtil';
import { images } from './constants';

export default class Demo extends BaseScene<NotReadOnly<typeof images>> {
    public sceneName = ExistModule.DEMO;
    public images = images as any;
    public hook_resourceReady(resources: Exclude<Demo['resources'], undefined>) {
        const mapRoot = new Sprite(resources.images['map/0.png']);
        new Array(10).fill(1).reduce<Sprite>((parent) => {
            const child = new Sprite(resources.images[`map/${Math.random() * 120 | 0}.png`]);
            child.x = parent.width - 1;
            parent.addChild(child);
            return child;
        }, mapRoot);
        this.addChild(mapRoot);
    }
}
