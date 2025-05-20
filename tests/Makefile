CC = gcc
CFLAGS = -Wall -Wextra -g

SRC_DIR = ../src
TEST_DIR = .
BUILD_DIR = build

# Source files
SRC_FILES = $(SRC_DIR)/parser.c $(SRC_DIR)/lexer.c

# Test executables
TEST_PARSER = $(BUILD_DIR)/test_parser
TEST_LEXER = $(BUILD_DIR)/test_lexer

all: $(TEST_PARSER) $(TEST_LEXER)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TEST_PARSER): $(SRC_FILES) $(TEST_DIR)/test_parser.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(TEST_LEXER): $(SRC_DIR)/lexer.c $(TEST_DIR)/test_lexer.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

test: test_parser test_lexer

test_parser: $(TEST_PARSER)
	./$(TEST_PARSER)

test_lexer: $(TEST_LEXER)
	./$(TEST_LEXER)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all test test_parser test_lexer clean 