import get from 'lodash/get';
import { GetConstruction } from '~/util/typeUtil';
import BaseComponents from './BaseComponent';
import BaseScene, { BaseSceneConfig } from './BaseScene';
import CommonLoading from './CommonLoading';

export interface AsyncComponentConfig<T extends BaseScene> extends BaseSceneConfig {
    component: Promise<GetConstruction<T>>;
    loading?: BaseComponents;
    sceneName: string;
}

/**
 * 异步组件
 *
 * @export
 * @class AsyncComponent
 * @extends {BaseScene}
 * @template T
 */
export default class AsyncComponent<T extends BaseScene = BaseScene> extends BaseScene {
    private readonly loading: BaseComponents;
    private readonly scene: Promise<{ default: GetConstruction<T> } | GetConstruction<T>>;
    private readonly passConfig: Omit<BaseSceneConfig, 'component' | 'loading'>;

    public constructor(config: AsyncComponentConfig<T>) {
        super(config);
        const { loading = new CommonLoading(), component, sceneName, ...subConfig } = config;
        this.loading = loading;
        this.scene = component;
        this.passConfig = subConfig;
        this.sceneName = sceneName;
    }
    public async hook_beforeInit() {}
    public async init() {
        this.addChild(this.loading);
        const promiseRes = await this.scene;
        const Scene = get<GetConstruction<T>>(promiseRes as any, 'default', promiseRes as any);
        this.removeChild(this.loading);
        this.loading.destroy();
        const scene = new Scene({
            ticker: this.ticker,
            ...this.passConfig
        });
        scene.game = this.game;
        scene.init();
        this.addChild(scene);
    }
}
