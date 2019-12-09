const baseConfig = require("./base.config");
const merge = require("webpack-merge");
/**
 * @type { import('webpack').Configuration}
 */
const addonConfig = {
    mode: "production",
    optimization: {
        splitChunks: {
            chunks: "all"
        }
    }
};

module.exports = merge(baseConfig, addonConfig);
