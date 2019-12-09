import { DisplayObject } from 'pixi.js';
const cacheSymbol = Symbol('cache');
class DisplayObjectRecyclingFactory {
    private readonly [cacheSymbol] = new Map<string, DisplayObject[]>();
    /**
     * @param {number} [maxLen=10] 每个类型的实例最多存多少个
     * @memberof DisplayObjectRecyclingFactory
     */
    public constructor(private maxLen = 10) {

    }
    public recycle<T extends DisplayObject>(cacheKey: string, inst: T) {
        const cache = this[cacheSymbol];
        const cacheArr = cache.get(cacheKey);
        if (cacheArr) {
            if (cacheArr.length < this.maxLen) {
                cacheArr.push(inst);
            }
        } else {
            cache.set(cacheKey, [inst]);
        }
        return this;
    }
    public reUse<T extends DisplayObject>(cacheKey: string): T | undefined {
        const cache = this[cacheSymbol];
        return ((cache.get(cacheKey) || []) as any as T[]).pop();
    }
    public clear(cacheKey?: string) {
        const cache = this[cacheSymbol];
        if (cacheKey) {
            cache.set(cacheKey, []);
        } else {
            cache.clear();
        }
        return this;
    }
}
export default DisplayObjectRecyclingFactory;
