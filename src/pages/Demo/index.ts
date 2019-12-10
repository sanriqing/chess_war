import { Sprite, Texture, TilingSprite } from 'pixi.js';
import BaseScene from '~/components/BaseScene';
import GameMap from '~/components/GameMap';
import { ExistModule, GameSize } from '~/constants';
import { NotReadOnly } from '~/util/typeUtil';
import { images } from './constants';

export default class Demo extends BaseScene<NotReadOnly<typeof images>> {
    public sceneName = ExistModule.DEMO;
    public images = images as any;
    public hook_resourceReady(resources: Exclude<Demo['resources'], undefined>) {
        const mapData = [
            [1, 2, 3, 4, 5, 6, 7],
            [1, 2, 3, 4, 5, 6, 7],
            [1, 2, 3, 4, 5, 6, 7],
            [1, 2, 3, 4, 5, 7],
            [1, 2, 3, 4, 5, 7],
            [1, 2, 3, 4, 5, 7],
            [1, 2, 3, 4, 5, 7],
            [1, 2, 3, 4, 5, 7],
            [1, 2, 3, 4, 5, 7],
            [1, 2, 3, 4, 5, 6, 7],
            [2, 3, 4, 5, 6, 7]
        ].map((row) => row.map((i) => resources.images[`map/${i}.png`]));
        const gameMap = new GameMap({
            mapData
        });
        this.addChild(gameMap);
    }
}
