# Deckbuilder Roguelike Makefile
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
SDL_CFLAGS = $(shell pkg-config --cflags sdl2 SDL2_ttf SDL2_mixer)
SDL_LIBS = $(shell pkg-config --libs sdl2 SDL2_ttf SDL2_mixer)

# Directories
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Test files (excluding test_runner.c which has its own main)
TEST_SOURCES = $(filter-out $(TEST_DIR)/test_runner.c, $(wildcard $(TEST_DIR)/test_*.c)) $(TEST_DIR)/unity.c
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.c=$(BUILD_DIR)/%.o)

# Individual test runners
TEST_RUNNERS = $(wildcard $(TEST_DIR)/test_*.c)
TEST_TARGETS = $(TEST_RUNNERS:$(TEST_DIR)/%.c=$(BUILD_DIR)/%)

# Main targets
GAME_TARGET = $(BUILD_DIR)/deckbuilder

# Default target
all: $(GAME_TARGET) $(TEST_TARGETS)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build the game
game: $(GAME_TARGET)

$(GAME_TARGET): $(BUILD_DIR) $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(SDL_LIBS)

# Build individual test targets
$(BUILD_DIR)/test_%: $(BUILD_DIR)/test_%.o $(BUILD_DIR)/unity.o $(filter-out $(BUILD_DIR)/main.o, $(OBJECTS))
	$(CC) $< $(BUILD_DIR)/unity.o $(filter-out $(BUILD_DIR)/main.o, $(OBJECTS)) -o $@

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -c $< -o $@

# Compile test files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

# Test targets
test: $(TEST_TARGETS)
	@echo "Running all tests..."
	@for test in $(TEST_TARGETS); do \
		echo "Running $$test..."; \
		./$$test; \
	done

# Run the game
run: $(GAME_TARGET)
	./$(GAME_TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Install dependencies (macOS)
install-deps:
	brew install sdl2 sdl2_ttf sdl2_mixer

# Help
help:
	@echo "Available targets:"
	@echo "  all       - Build both game and tests"
	@echo "  game      - Build the game executable"
	@echo "  test      - Build and run tests"
	@echo "  run       - Build and run the game"
	@echo "  clean     - Remove build artifacts"
	@echo "  install-deps - Install SDL2 dependencies"
	@echo "  help      - Show this help"

.PHONY: all test run clean install-deps help
