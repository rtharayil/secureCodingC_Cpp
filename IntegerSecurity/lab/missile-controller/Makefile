# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -Wconversion -std=c++17 -g

# Include and source directories
INCDIR = include
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Target executable
TARGET = $(BINDIR)/missile_controller

# Source files
SRCS = $(wildcard $(SRCDIR)/*.cpp)

# Object files
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

# Default target
all: dirs $(TARGET)

# Create necessary directories
dirs:
	mkdir -p $(OBJDIR) $(BINDIR)

# Link objects to create executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $^ -o $@

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJDIR)/*.o $(TARGET)

.PHONY: all clean dirs
