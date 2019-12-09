const { resolvePath } = require("./util");
const HtmlPlugin = require("html-webpack-plugin");
const CopyPlugin = require("copy-webpack-plugin");

/**
 * @type { import('webpack').Configuration }
 */
module.exports = {
    context: resolvePath("./"),
    entry: "./src/index.ts",
    output: {
        filename: "[name][hash].js",
        chunkFilename: "[name][hash].js",
        path: resolvePath("dist"),
        libraryTarget: "umd"
    },
    module: {
        rules: [
            {
                test: /\.tsx?$/i,
                use: ["ts-loader"],
                include: [resolvePath("src")]
            },
            {
                test:/.mp3$/i,
                loader: "url-loader",
                options: {
                    limit: 0,
                    name: "[name].[hash].[ext]"
                },
                include: [resolvePath("src/assets/voices")]
            },
            {
                test: /\.(png|jpg|jpeg)$/i,
                loader: "url-loader",
                options: {
                    limit: 0,
                    name: "[name].[hash].[ext]"
                },
                include: [resolvePath("src/assets/images")]
            }
        ]
    },
    plugins: [
        new HtmlPlugin({
            template: resolvePath("src/assets/statics/index.ejs")
        }),
        new CopyPlugin([
            {
                from: "src/assets/sprites/**/*.png",
                to: "sprites/[name].png"
            },
            {
                from: "src/assets/sprites/**/*.json",
                to: "sprites/[name].json"
            },
            {
                from: "src/wasm/**/*.js",
                to: "[name].js"
            },
            {
                from: "src/wasm/**/*.wasm",
                to: "[name].wasm"
            }
        ])
    ],
    resolve: {
        alias: {
            "~": resolvePath("src")
        },
        extensions: [".ts", ".js", ".css", ".wasm"]
    }
};
