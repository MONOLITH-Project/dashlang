# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -std=c89
DEBUG_FLAGS := -g -O0 -DDEBUG
RELEASE_FLAGS := -O2
INCLUDE_DIRS := -I./libs/Unity/src -I./include
TEST_INCLUDE_DIRS := -I./test
TEST_CFLAGS := $(CFLAGS) $(DEBUG_FLAGS)

# Directories
SRC_DIR := src
BUILD_DIR := build
TEST_DIR := test
LIB_DIR := libs
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj
TEST_BIN_DIR := $(BUILD_DIR)/test_bin
TEST_OBJ_DIR := $(BUILD_DIR)/test_obj

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
MAIN_OBJ := $(OBJ_DIR)/main.o

# Test files
LEXER_TEST_SRC := $(wildcard $(TEST_DIR)/lexer_tests/*.c) libs/Unity/src/unity.c
LEXER_TEST_OBJ := $(patsubst $(TEST_DIR)/lexer_tests/%.c, $(TEST_OBJ_DIR)/lexer_tests/%.o, $(LEXER_TEST_SRC))
LEXER_TEST_BIN := $(TEST_BIN_DIR)/lexer_tests

EMITTER_TEST_SRC := $(wildcard $(TEST_DIR)/emitter_tests/*.c)
EMITTER_TEST_OBJ := $(patsubst $(TEST_DIR)/emitter_tests/%.c, $(TEST_OBJ_DIR)/emitter_tests/%.o, $(EMITTER_TEST_SRC))
EMITTER_TEST_BIN := $(TEST_BIN_DIR)/emitter_tests

ARENA_TEST_SRC := $(wildcard $(TEST_DIR)/arena_tests/*.c) libs/Unity/src/unity.c
ARENA_TEST_OBJ := $(patsubst $(TEST_DIR)/arena_tests/%.c, $(TEST_OBJ_DIR)/arena_tests/%.o, $(ARENA_TEST_SRC))
ARENA_TEST_BIN := $(TEST_BIN_DIR)/arena_tests

# Output binary
TARGET := $(BIN_DIR)/dash

# Phony targets
.PHONY: all clean debug release test dirs

# Default target
all: release

# Create necessary directories
dirs:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR) $(TEST_BIN_DIR) $(TEST_OBJ_DIR) $(TEST_OBJ_DIR)/lexer_tests $(TEST_OBJ_DIR)/emitter_tests $(TEST_OBJ_DIR)/arena_tests

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: dirs $(TARGET)

# Release build
release: CFLAGS += $(RELEASE_FLAGS)
release: dirs $(TARGET)

# Build the main executable
$(TARGET): $(OBJ_FILES) $(MAIN_OBJ)
	@echo "Linking $(TARGET)..."
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "Build successful!"

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# Compile main.c
$(MAIN_OBJ): main.c
	@echo "Compiling main.c..."
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# Test targets
test: test_lexer test_emitter test_arena
	@echo "All tests completed."

test_lexer: dirs $(LEXER_TEST_BIN)
	@echo "Running lexer tests..."
	@$(LEXER_TEST_BIN)

test_emitter: dirs $(EMITTER_TEST_BIN)
	@echo "Running emitter tests..."
	@if [ -x "$(EMITTER_TEST_BIN)" ]; then $(EMITTER_TEST_BIN); else echo "No emitter tests to run"; fi

test_arena: dirs $(ARENA_TEST_BIN)
	@echo "Running arena tests..."
	@$(ARENA_TEST_BIN)

# Build lexer tests
$(LEXER_TEST_BIN): $(filter-out $(OBJ_DIR)/main.o, $(OBJ_FILES)) $(LEXER_TEST_OBJ)
	@echo "Linking lexer tests..."
	@$(CC) $(TEST_CFLAGS) $^ -o $@

# Build emitter tests
$(EMITTER_TEST_BIN): $(filter-out $(OBJ_DIR)/main.o, $(OBJ_FILES)) $(EMITTER_TEST_OBJ)
	@echo "Linking emitter tests..."
	@if [ -n "$(EMITTER_TEST_OBJ)" ]; then $(CC) $(TEST_CFLAGS) $^ -o $@; fi

# Build arena tests
$(ARENA_TEST_BIN): $(filter-out $(OBJ_DIR)/main.o, $(OBJ_FILES)) $(ARENA_TEST_OBJ)
	@echo "Linking arena tests..."
	@$(CC) $(TEST_CFLAGS) $^ -o $@

# Compile lexer test files
$(TEST_OBJ_DIR)/lexer_tests/%.o: $(TEST_DIR)/lexer_tests/%.c
	@echo "Compiling test $<..."
	@$(CC) $(TEST_CFLAGS) $(INCLUDE_DIRS) $(TEST_INCLUDE_DIRS) -c $< -o $@

# Compile emitter test files
$(TEST_OBJ_DIR)/emitter_tests/%.o: $(TEST_DIR)/emitter_tests/%.c
	@$(CC) $(TEST_CFLAGS) $(INCLUDE_DIRS) $(TEST_INCLUDE_DIRS) -c $< -o $@

# Compile arena test files
$(TEST_OBJ_DIR)/arena_tests/%.o: $(TEST_DIR)/arena_tests/%.c
	@echo "Compiling test $<..."
	@$(CC) $(TEST_CFLAGS) $(INCLUDE_DIRS) $(TEST_INCLUDE_DIRS) -c $< -o $@

# Clean build files
clean:
	@echo "Cleaning build files..."
	@rm -rf $(BUILD_DIR)
	@echo "Clean complete."

# Help target
help:
	@echo "Dash Language Project Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all        - Build the project in release mode (default)"
	@echo "  debug      - Build the project with debug symbols"
	@echo "  release    - Build the project with optimizations"
	@echo "  test       - Build and run all tests"
	@echo "  test_lexer - Build and run lexer tests only"
	@echo "  test_emitter - Build and run emitter tests only"
	@echo "  test_arena  - Build and run arena tests only"
	@echo "  clean      - Remove all build artifacts"
	@echo "  help       - Display this help message"
