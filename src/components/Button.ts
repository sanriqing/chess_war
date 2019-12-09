import gasp, { TimelineLite } from 'gsap';
import { Texture } from 'pixi.js';
import BaseComponents from './BaseComponent';

export default class Button extends BaseComponents {
    private onScale = false;
    public constructor(texture?: Texture) {
        super({
            texture
        });
        this.anchor.set(0.5, 0.5);
        this.interactive = true;
        this.on('tap', async (e: any) => {
            if (this.onScale) {
                return;
            }
            this.onScale = true;

            const tl = new TimelineLite(this.scale);
            tl
                .to(this.scale, {
                    x: 0.9,
                    y: 0.9
                })
                .to(this.scale, {
                    x: 1,
                    y: 1
                })
                .duration(0.1)
                .play()
                .then(() => {
                    tl.kill();
                    this.onScale = false;
                    this.emit('tap.button', e);
                });
        });
    }
}
