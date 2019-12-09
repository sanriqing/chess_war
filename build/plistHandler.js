const plistHandler = require("cocos_plist_to_pixi_json");
const { resolvePath } = require("./util");
plistHandler(resolvePath("src/assets/sprites"));

