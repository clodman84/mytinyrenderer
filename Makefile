# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -std=c++17 -Iinclude

# Linker flags
LIBS := -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

# Directories
SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := $(BIN_DIR)/obj
ASSETS_DIR := assets

# Output binary name
TARGET := $(BIN_DIR)/mytinyrenderer

# Source and object files
SRC := $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC:.cpp=.o))
OBJ := $(OBJ:.c=.o)

# Default target
all: $(TARGET) copy_assets

# Link objects into executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LIBS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure directories exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Copy assets to bin/assets
copy_assets:
	mkdir -p $(BIN_DIR)
	cp -r $(ASSETS_DIR) $(BIN_DIR)/

# Clean build files
clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean copy_assets

