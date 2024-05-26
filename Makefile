# Compiler and linker
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Executables
TARGETS = encode decode

# Default target
all: $(TARGETS)

# Rules for each executable
encode: $(BUILD_DIR)/main_encode.o $(BUILD_DIR)/encode.o
	$(CXX) $^ -o $@

decode: $(BUILD_DIR)/main_decode.o $(BUILD_DIR)/decode.o
	$(CXX) $^ -o $@

# Rule for compiling source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# Clean build artifacts
clean:
	@if exist $(BUILD_DIR) (rd /s /q $(BUILD_DIR))
	@if exist encode (del /f encode.exe)
	@if exist decode (del /f decode.exe)

.PHONY: all clean
