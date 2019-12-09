import first from 'lodash/first';
import { Application } from 'pixi.js';
import { GameSize, ScaleRate, ScreenSize, SettingConfig, UISize } from '~/constants';
import { bangScreenPadding, IS_DEV } from '~/constants/index';
import BaseScene from './BaseScene';

const ActiveSceneSymbol = Symbol('activeScene');
interface ApplicationConfig {
    autoStart?: boolean;
    width?: number;
    height?: number;
    view?: HTMLCanvasElement;
    transparent?: boolean;
    autoDensity?: boolean;
    antialias?: boolean;
    preserveDrawingBuffer?: boolean;
    resolution?: number;
    forceCanvas?: boolean;
    backgroundColor?: number;
    clearBeforeRender?: boolean;
    forceFXAA?: boolean;
    powerPreference?: string;
    sharedTicker?: boolean;
    sharedLoader?: boolean;
    resizeTo?: Window | HTMLElement;
}
export interface GameConfig extends ApplicationConfig {
    uiWidth?: number;
    uiHeight?: number;
    scenes: BaseScene[];
}


export default class Game extends Application {
    public scenes: { [K: string]: BaseScene };
    private preScene?: BaseScene;
    private [ActiveSceneSymbol]: BaseScene;

    private get activeScene() {
        return this[ActiveSceneSymbol];
    }
    private set activeScene(scene) {
        this.preScene = this[ActiveSceneSymbol];
        if (this.preScene) {
            this.preScene.destroy();
        }
        this[ActiveSceneSymbol] = scene;
        this.stage.addChild(scene);
        scene.init();
    }
    public constructor(param: GameConfig = {
        scenes: []
    }) {
        super({
            ...param,
            ...(IS_DEV ? ScreenSize : {
                width: ScreenSize.height,
                height: ScreenSize.width
            }),
            resolution: devicePixelRatio
        });
        const defaultConfig: GameConfig = {
            uiWidth: UISize.width,
            uiHeight: UISize.height,
            scenes: []
        };
        const config = { ...defaultConfig, ...param };
        if (!IS_DEV) {
            const { stage } = this;
            stage.rotation = Math.PI / 2;
            stage.position.set(ScreenSize.height, 0);
        }
        this.view.style.transformOrigin = 'top left';
        this.view.style.transform = `scale(${1 / devicePixelRatio})`;
        if (SettingConfig.isBangScreen) {
            this.view.style.paddingLeft = `${bangScreenPadding}px`;
        }

        document.body.append(this.view);
        this.stage.scale.set(ScaleRate);
        // 初始化场景信息
        if (!config.scenes.length) {
            throw new Error('no scene');
        }
        const initScenes: Game['scenes'] = {};
        this.scenes = config.scenes.reduce((dic, scene) => {
            dic[scene.sceneName] = scene;
            scene.game = this;
            return dic;
        }, initScenes);
        const firstScene = first(config.scenes)!;
        this.activeScene = firstScene;
        this.start();
    }
}
