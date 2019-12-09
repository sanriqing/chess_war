import BaseScene from '~/components/BaseScene';
import { ExistModule, GameSize } from '~/constants';
import { images } from './constants'
import { NotReadOnly } from '~/util/typeUtil';
import { Sprite, TilingSprite } from 'pixi.js';
export default class Demo extends BaseScene<NotReadOnly<typeof images>> {
    public sceneName = ExistModule.DEMO;
    public images = images as any
    public hook_resourceReady(resources: Exclude<Demo['resources'], undefined>) {
        const map0 = new Sprite(resources.images['map/0.png'])
        const map1 = new Sprite(resources.images['map/1.png'])
        const map2 = new Sprite(resources.images['map/1.png'])
        map1.y = map0.height / 2
        map1.x = map0.width / 2

        map2.x = map0.width / 2
        map0.addChild(map1)
        map0.addChild(map2)
        this.addChild(map0)
    }
}
