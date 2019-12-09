const path = require("path");
/** 处理路径为绝对路径 */
exports.resolvePath = targetPath => {
  return path.resolve(__dirname, "../", targetPath);
};
