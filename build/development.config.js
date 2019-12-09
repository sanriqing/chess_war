const baseConfig = require("./base.config");
const merge = require("webpack-merge");
/**
 * @type { import('webpack').Configuration}
 */
const addonConfig = {
    mode: "development",
    devServer: {
        quiet: true,
        open: true,
        hot: true,
        overlay: true,
        clientLogLevel: "warning",
        host:'0.0.0.0'
    }
};

module.exports = merge(baseConfig, addonConfig);
