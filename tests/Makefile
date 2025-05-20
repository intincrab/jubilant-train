CC = gcc
CFLAGS = -Wall -Wextra -g

SRC_DIR = ../src
TEST_DIR = .
BUILD_DIR = build

# Source files for the parser test
SRC_FILES = $(SRC_DIR)/parser.c $(SRC_DIR)/lexer.c
TEST_FILES = $(TEST_DIR)/test_parser.c

# Target executable
TEST_PARSER = $(BUILD_DIR)/test_parser

all: $(TEST_PARSER)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TEST_PARSER): $(SRC_FILES) $(TEST_FILES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

test: $(TEST_PARSER)
	./$(TEST_PARSER)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all test clean 