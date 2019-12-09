import { TweenLite } from 'gsap';
import defaultsDeep from 'lodash/defaultsDeep';
import { DisplayObject, ITextureDictionary, Sprite, Texture, TilingSprite } from 'pixi.js';
import BaseComponents from '~/components/BaseComponent';
import Button from '~/components/Button';
import { GameSize } from '~/constants';
import BaseRoundRect from './BaseRoundRect';

export interface BaseModalConfig<P extends object = any> {
    cloakClose?: boolean;
    cloak?: boolean;
    backGroundTexture?: Texture;
    titleTexture?: Texture;
    contentStyle?: {
        width?: number;
        height?: number;
        fillColor?: number;
    };
    resources: ITextureDictionary;
    contentBackgroundAlpha?: number;
    hideHeader?: boolean;
    closeBtnHide?: boolean;
    param?: P;
}

const contentContainerSymbol = Symbol('contentContainer');

type RealConfigType = Omit<Required<BaseModalConfig>, 'backGroundTexture' | 'titleTexture'> & { backGroundTexture?: Texture; titleTexture?: Texture };


export default class BaseModal<P extends object = any> extends BaseComponents {
    public resources: ITextureDictionary;
    public readonly containerBoxWidth: number;
    public readonly containerBoxHeight: number;
    private [contentContainerSymbol]: BaseRoundRect;
    public constructor(config: BaseModalConfig<P>) {
        super();
        this.sortableChildren = true;
        const defaultConfig = {
            contentStyle: {
                width: GameSize.width * 0.6,
                height: GameSize.height * 0.6,
                fillColor: 0xFFFFFF
            },
            cloakClose: false,
            cloak: true,
            contentBackgroundAlpha: 1,
            hideHeader: false
        };
        const realConfig = defaultsDeep({}, config || {}, defaultConfig);
        this.resources = realConfig.resources;
        this.containerBoxWidth = realConfig.contentStyle.width!;
        this.containerBoxHeight = realConfig.contentStyle.height!;


        if (realConfig.cloak) {
            const cloak = this.renderCloak();
            cloak.on('tap', () => {
                this.emit('tap.cloak');
                if (realConfig.cloakClose) {
                    this.emit('close');
                }
            });
            this.addChild(cloak);
        }

        const containerBoxPosition = {
            x: (GameSize.width - realConfig.contentStyle.width!) / 2,
            y: (GameSize.height - realConfig.contentStyle.height!) / 2
        };
        const containerBgBox = this.renderContainerBgBgBox(realConfig, containerBoxPosition);


        let children: DisplayObject[] = [containerBgBox];
        let headerHeight = 0;
        if (!realConfig.hideHeader) {
            const headerItems = this.renderHeaderItems(realConfig, containerBoxPosition);
            const { header, title } = headerItems;
            children.push(header);
            if (title) {
                children.push(title);
            }
            headerHeight = header.height;
        }

        const content = this.renderContent(containerBgBox.width, containerBgBox.height - headerHeight, realConfig.param);
        if (content) {
            content.x = containerBoxPosition.x;
            content.y = containerBoxPosition.y + headerHeight;
            children.push(content);
        }

        if (!realConfig.closeBtnHide) {
            const closeBtn = new Button(realConfig.resources['close.png']);
            closeBtn.x = containerBoxPosition.x + realConfig.contentStyle.width!;
            closeBtn.y = containerBoxPosition.y + 30;
            closeBtn.on('tap.button', () => {
                this.emit('close');
            });
            children.push(closeBtn);
        }


        const contentContainer = this.renderContentContainer(realConfig, containerBoxPosition);
        this.visible = false;
        this.zIndex = 9999;
        contentContainer.addChild(...children);
        contentContainer.scale.set(0, 0);
        this[contentContainerSymbol] = contentContainer;

        this.addChild(contentContainer);
        this.on('close', () => {
            this.close();
        });
    }
    public renderContent(containerWidth?: number, containerHeight?: number, param?: P): DisplayObject | undefined {
        return;
    }
    /**
     * 打开弹窗
     *
     * @memberof BaseModal
     */
    public open() {
        this.visible = true;
        const animate = TweenLite.to(this[contentContainerSymbol].scale, 0.3, {
            x: 1,
            y: 1,
            onComplete: () => {
                animate.kill();
            }
        }).play();
        return this;
    }
    /**
     * 关闭弹窗
     *
     * @memberof BaseModal
     */
    public close() {
        const animate = TweenLite.to(this[contentContainerSymbol].scale, 0.3, {
            x: 0,
            y: 0,
            onComplete: () => {
                animate.kill();
                this.visible = false;
                this.emit('afterClose');
            }
        }).play();
        return this;
    }

    private renderHeaderItems(realConfig: RealConfigType, containerBoxPosition: { x: number; y: number }) {
        const header = new Sprite(realConfig.resources['title_bar.png']);
        header.width = realConfig.contentStyle.width!;
        header.x = containerBoxPosition.x;
        header.y = containerBoxPosition.y;

        let outPut: {
            header: Sprite;
            title?: Sprite;
        } = {
            header
        };
        if (realConfig.titleTexture) {
            const title = new Sprite(realConfig.titleTexture);
            title.x = containerBoxPosition.x + (realConfig.contentStyle.width! - title.width) / 2;
            title.y = containerBoxPosition.y + header.height / 2 - 20;
            outPut.title = title;
        }
        return outPut;
    }

    /**
     *
     * 渲染遮罩层
     * @private
     * @returns
     * @memberof BaseModal
     */
    private renderCloak() {
        const cloak = new BaseRoundRect({
            boxStyle: {
                width: GameSize.width,
                height: GameSize.height,
                fillColor: 0x000000,
                radius: 0
            }
        });
        cloak.alpha = 0.5;
        cloak.interactive = true;
        return cloak;
    }


    // private render
    /**
     * 渲染内容容器盒子
     *
     * @private
     * @param {RealConfigType} realConfig
     * @param {{x: number; y: number}} containerBoxPosition
     * @memberof BaseModal
     */
    private renderContentContainer(realConfig: RealConfigType, containerBoxPosition: { x: number; y: number }) {
        const contentContainer = new BaseRoundRect({
            boxStyle: {
                x: 0,
                y: 0,
                width: realConfig.contentStyle.width!,
                height: realConfig.contentStyle.height!,
                fillAlpha: 0
            }
        });
        contentContainer.pivot.x = realConfig.contentStyle.width! / 2 + containerBoxPosition.x;
        contentContainer.pivot.y = realConfig.contentStyle.height! / 2 + containerBoxPosition.y;
        contentContainer.x = GameSize.width / 2;
        contentContainer.y = GameSize.height / 2;
        return contentContainer;
    }
    /**
     *
     * 创建内容背景盒子
     * @private
     * @param {RealConfigType} realConfig
     * @param {{x: number; y: number}} containerBoxPosition
     * @memberof BaseModal
     */
    private renderContainerBgBgBox(realConfig: RealConfigType, containerBoxPosition: { x: number; y: number }) {
        let containerBox: BaseRoundRect | TilingSprite;
        if (realConfig.backGroundTexture) {
            containerBox = new TilingSprite(realConfig.backGroundTexture, realConfig.contentStyle.width, realConfig.contentStyle.height);
            containerBox.x = containerBoxPosition.x;
            containerBox.y = containerBoxPosition.y;
        } else {
            containerBox = new BaseRoundRect({
                boxStyle: {
                    ...containerBoxPosition,
                    width: realConfig.contentStyle.width,
                    height: realConfig.contentStyle.height,
                    fillColor: realConfig.contentStyle.fillColor,
                    radius: 8
                }
            });
        }
        containerBox.alpha = realConfig.contentBackgroundAlpha;
        return containerBox;
    }
}
