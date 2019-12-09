import { gsap } from 'gsap';
// @ts-ignore
import { PixiPlugin } from 'gsap/PixiPlugin';
import * as PIXI from 'pixi.js';
PixiPlugin.registerPIXI(PIXI);

import Game from './components/Game';
import Demo from './pages/Demo';
// @ts-ignore
gsap.registerPlugin(PixiPlugin);

// @ts-ignore
// tslint:disable-next-line: no-unused-expression
window.__PIXI_INSPECTOR_GLOBAL_HOOK__ && window.__PIXI_INSPECTOR_GLOBAL_HOOK__.register({ PIXI });

export default new Game({
    scenes: [
        new Demo()
    ]
});
