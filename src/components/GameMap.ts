import { get, last } from 'lodash';

import { Container, Sprite, Texture } from 'pixi.js';
import { IS_DEV } from '~/constants/index';
import { warn } from '~/util';

export interface IMapConfig {
    mapData: Texture[][];
}
export default class GameMap extends Container {
    public mapWidth = 0;
    public mapHeight = 0;
    public constructor(config: IMapConfig) {
        super();
        const { mapData } = config;
        if (IS_DEV) {
            if (!get(mapData, '0.0')) {
                warn('error mapData');
                return;
            }
        }
        // 将地图元素推入容器铺开
        const children = mapData.map((textures) => {
            const container = new Container();
            let width = 0;
            let height = 0;
            const sprites = textures.reduce<Sprite[]>((sprites, texture) => {
                const sprite = Sprite.from(texture);
                height = Math.max(sprite.height, height);
                sprite.x = width;
                width += sprite.width;
                return sprites.concat(sprite);
            }, [] as Sprite[]);
            if (sprites.length) {
                container.addChild(...sprites);
            }
            return {
                container,
                width,
                height
            };
        });
        this.addChild(...children.map(({container, width, height}) => {
            container.y = this.mapHeight;
            this.mapWidth += width;
            this.mapHeight += height;
            return container;
        }));
        this.cacheAsBitmap = true;
    }
}
