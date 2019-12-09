import sound from 'pixi-sound';
import { ITextureDictionary, Loader, LoaderResource, Texture, Ticker } from 'pixi.js';
import { noop } from '~/util';
import BaseComponents, { BaseComponentsConfig } from './BaseComponent';
import Game from './Game';

const RESOURCES_SYMBOL = Symbol('resource');
const LOADER_CACHE_SYMBOL = Symbol('loader_cache');

type GetLoaderResource<S extends boolean> = S extends true ? Required<LoaderResource> : LoaderResource;

export type LoadResourcesParamConfig<S extends string = string> = {
    [k in S]: string | {
        url: string;
        cb?: Function;
    };
};

export interface BaseSceneConfig extends BaseComponentsConfig {
    ticker?: Ticker;
}

export interface ImageResourcesType<K extends string = string, S extends string = string, V extends string = string> {
    images: {
        [key in K]: Texture;
    };
    sprites: {
        [key in S]: ITextureDictionary;
    };
    voices: {
        [key in V]: sound.Sound
    };
}

export default class BaseScene<I extends string[] = string[], S extends string[] = string[], V extends string[] = string[]> extends BaseComponents {
    public get resources() {
        return this[RESOURCES_SYMBOL];
    }
    public set resources(val) {
        this[RESOURCES_SYMBOL] = val;
    }
    public game?: Game;
    public ticker: Ticker;
    public images: I = [] as any;
    public sprites: S = [] as any;
    public voices: V = [] as any;
    public sceneName = '';
    private [RESOURCES_SYMBOL]?: ImageResourcesType<I[number], S[number], V[number]>;
    private readonly [LOADER_CACHE_SYMBOL] = new Map();
    public constructor(config: BaseSceneConfig = {}) {
        super(config);
        this.ticker = config.ticker || new Ticker();
        this.on('removed', () => {
            this[LOADER_CACHE_SYMBOL].clear();
            this.ticker.destroy();
        });
    }

    public async init() {
        await this.hook_beforeInit();
        await this._initResources();
    }


    /**
     *
     * 加载资源《基础方法,需要完整路径》
     * @template S 加载的是否是精灵图
     * @template T
     * @param {T} data
     * @returns {Promise<{ [K in keyof T]: GetLoaderResource<S>  }>}
     * @memberof BaseScene
     */
    public async loadResource<S extends boolean = true, T extends LoadResourcesParamConfig = LoadResourcesParamConfig>(data: T): Promise<{ [K in keyof T]: GetLoaderResource<S> }> {
        const loader = new Loader();
        const promises = Object.entries(data).map(async ([alias, loadInfo]) => {
            const loaderCache = this[LOADER_CACHE_SYMBOL];
            let url: string;
            let cb: Function = noop;
            if (typeof loadInfo === 'string') {
                url = loadInfo;
            } else {
                url = loadInfo.url;
                cb = loadInfo.cb || noop;
            }

            const resource = loaderCache.get(alias);
            if (resource) {
                if (resource.isComplete) {
                    cb(alias, resource);
                    return resource;
                } else {
                    return new Promise<LoaderResource>((resolve, reject) => {
                        // @ts-ignore
                        resource.onAfterMiddleware.once((resource) => {
                            if (resource.error) {
                                reject(resource.error);
                                return;
                            }
                            cb(alias, loader.resources[alias]);
                            resolve(loader.resources[alias]);
                        });
                    });
                }
            }
            return new Promise((resolve, reject) => {
                loader.add({
                    name: alias,
                    url,
                    callback: (resource: LoaderResource) => {
                        if (resource.error) {
                            reject(resource.error);
                            return;
                        }
                        cb(alias, loader.resources[alias]);
                        resolve(loader.resources[alias]);
                    }
                });
                loaderCache.set(alias, loader.resources[alias]);
            });
        });
        loader.load();
        const resources = await Promise.all<GetLoaderResource<S>[]>(promises);
        loader.destroy();
        return Object.keys(data).reduce<{ [K in keyof T]: GetLoaderResource<S> }>((info, key, index) => {
            return {
                ...info,
                [key]: resources[index]
            };
        }, {} as any);
    }

    /**
     *
     * 加载图片
     * @template T
     * @param {T} images
     * @param {Function} [cb]
     * @memberof BaseScene
     */
    public async loadImages<T extends string[] = string[]>(images: T, cb: Function = noop) {
        const loadInfo = images.reduce<LoadResourcesParamConfig>((infoObj, imgName) => {
            // @ts-ignore
            const url = require('~/assets/images/' + imgName);
            return {
                ...infoObj,
                [imgName]: {
                    url,
                    cb
                }
            };
        }, {});
        return this.loadResource<false, LoadResourcesParamConfig<T[number]>>(loadInfo);
    }

    /**
     * 加载音频文件
     *
     * @template V
     * @param {V} voices
     * @param {Function} [cb=noop]
     * @memberof BaseScene
     */
    public async loadVoices<V extends string[]>(voices: V, cb: Function = noop) {
        const loadInfo = voices.reduce<LoadResourcesParamConfig>((infoObj, voiceName) => {
            // @ts-ignore
            const url = require('~/assets/voices/' + voiceName);
            return {
                ...infoObj,
                [voiceName]: {
                    url,
                    cb
                }
            };
        }, {});
        const voiceInfos = await this.loadResource<false, LoadResourcesParamConfig<V[number]>>(loadInfo);
        return Object.entries(voiceInfos).reduce<{ [K in V[number]]: sound.Sound }>((info, [key, value]) => ({
            ...info,
            [key]: sound.Sound.from(value as LoaderResource)
        }), {} as any);
    }

    /**
     *
     * 加载精灵图
     * @param {string[]} sprites
     * @param {Function} [cb]
     * @memberof BaseScene
     */
    public async loadSprites<T extends string[]>(sprites: T, cb?: Function) {
        const spriteObj = sprites.reduce<LoadResourcesParamConfig>((infoObj, spriteName) => {
            const url = `/sprites/${spriteName}.json?${Date.now() * Math.random() | 0}`;
            return {
                ...infoObj,
                [spriteName]: {
                    url,
                    cb
                }
            };
        }, {});
        return this.loadResource<true, LoadResourcesParamConfig<T[number]>>(spriteObj);
    }
    /**
     *
     * 初始化资源
     * @memberof BaseScene
     */
    public async _initResources() {
        const [imageResources, addon] = await Promise.all([
            this._getPreResources(),
            this.getAddonResources()
        ]);
        this.resources = imageResources;
        this.hook_resourceReady(imageResources, addon);
    }

    /**
     *
     * 获取额外资源的方法 可覆盖添加其他资源
     * @memberof BaseScene
     */
    public getAddonResources(): any { }

    /**
     *
     * 初始化资源之前执行，
     * 从此之后game等参数已经注入
     * @memberof BaseScene
     */
    public hook_beforeInit(): any { }
    /**
     *
     * 钩子函数，当资源全部下载完成时执行
     * @param {ImageResourcesType<I[number],S[number], V[number]>} imageResources
     * @param {*} [addon]
     * @memberof BaseScene
     */
    public hook_resourceReady(imageResources: ImageResourcesType<I[number], S[number], V[number]>, addon?: any): any { }

    /**
     * 当场景设置的资源其中一项下载完成触发的钩子
     *
     * @memberof BaseScene
     */
    public hook_sceneResourceProcess(alias?: string, resources?: any) { }
    /**
     * 获取获取图片资源
     *
     * @returns {Promise<ImageResourcesType>}
     * @memberof BaseScene
     */
    private async _getPreResources(): Promise<ImageResourcesType> {
        const [imageResources, spriteResources, voices] = await Promise.all([
            this.loadImages(this.images, (alias: string, resources: any) => this.hook_sceneResourceProcess(alias, resources)),
            this.loadSprites(this.sprites, (alias: string, resources: any) => this.hook_sceneResourceProcess(alias, resources)),
            this.loadVoices(this.voices, (alias: string, resources: any) => this.hook_sceneResourceProcess(alias, resources))
        ]);
        const images = Object.entries(imageResources).reduce<ImageResourcesType['images']>((info, [key, value]) => {
            return {
                ...info,
                [key]: (value as any).texture
            };
        }, {});
        const sprites = Object.entries(spriteResources).reduce<ImageResourcesType['sprites']>((info, [key, value]) => {
            return {
                ...info,
                [key]: (value as any).textures!
            };
        }, {});
        return {
            images,
            sprites,
            voices
        };
    }
}
