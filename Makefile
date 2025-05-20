CC = gcc
EMCC = emcc
CFLAGS = -Wall -Wextra -g
SRC_DIR = src
BUILD_DIR = build
PUBLIC_DIR = public

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/lexer.c $(SRC_DIR)/parser.c $(SRC_DIR)/codegen.c
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
TARGET = $(BUILD_DIR)/tiny-compiler

WASM_CFLAGS = -s WASM=1 -s EXPORTED_FUNCTIONS='["_compile", "_free_result", "_malloc", "_free", "_main"]' -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap", "UTF8ToString"]' -s ALLOW_MEMORY_GROWTH=1
WASM_TARGET = $(PUBLIC_DIR)/tiny-compiler.js

.PHONY: all clean wasm

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

wasm: $(WASM_TARGET)

$(WASM_TARGET): $(SRCS)
	@mkdir -p $(PUBLIC_DIR)
	$(EMCC) $(CFLAGS) $(WASM_CFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)/* $(PUBLIC_DIR)/tiny-compiler.js $(PUBLIC_DIR)/tiny-compiler.wasm