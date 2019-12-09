
const stackSymbol = Symbol('stack');
const activeIndexSymbol = Symbol('activeIndex');
export class Route {
    public constructor(public readonly name: string, public readonly router: BaseRouter, public readonly param?: any) { }
}
export class BaseRouter {
    private [stackSymbol]: Route[];
    private [activeIndexSymbol]: number;
    public get stack() {
        return [...this[stackSymbol]];
    }
    public constructor() {
        this[stackSymbol] = [];
        this[activeIndexSymbol] = -1;
    }
    public replace(name: string, param: any = {}) {
        const route = new Route(name, this, param);
        this[stackSymbol].splice(-1, 1, route);
        this[activeIndexSymbol] = this[stackSymbol].length - 1;
        return route;
    }
    public to(name: string, param: any = {}) {
        const route = new Route(name, this, param);
        this[stackSymbol].push(route);
        this[activeIndexSymbol] = this[stackSymbol].length - 1;
        return route;
    }
    public go(num: number) {
        let activeIndex = this[activeIndexSymbol] + num;
        const route = this[stackSymbol][activeIndex];
        if (!route) {
            console.log('stack overflow');
            return;
        }
        this[activeIndexSymbol]  = activeIndex;
        return route;
    }
}
