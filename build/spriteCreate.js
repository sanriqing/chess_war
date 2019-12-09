const SpriteGenerator = require("pixi_sprite_generator");
const { resolvePath } = require("./util");

new SpriteGenerator({
    baseDir: resolvePath("./src/assets/createSprites"),
    configPath: resolvePath("./src/assets/createSprites/config.json"),
    destDir: resolvePath("./src/assets/sprites/selfCreate"),
    spritesmithParams: {
        padding: 10
    }
});
