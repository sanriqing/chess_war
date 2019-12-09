emcc \
*.c \
-O3 \
-o wasm_fish.html \
-s NO_EXIT_RUNTIME=1 \
-s MODULARIZE_INSTANCE=1 \
-s INVOKE_RUN=1 \
-s ASM_JS=1 \
-s EXPORT_NAME=wasm_fish \
-s WASM=1 \
-s ALLOW_MEMORY_GROWTH=1 \


#拷贝到工作目录
cp -R /Users/fenghonghai/Documents/pixis/MyPixi/bin /Library/WebServer/Documents/wasm/fish_pixi
cp /Library/WebServer/Documents/wasm/fish/wasm_fish.js  /Library/WebServer/Documents/wasm/fish_pixi/bin
cp /Library/WebServer/Documents/wasm/fish/wasm_fish.wasm  /Library/WebServer/Documents/wasm/fish_pixi/bin
cp /Library/WebServer/Documents/wasm/fish/wasm_fish.html.mem  /Library/WebServer/Documents/wasm/fish_pixi/bin