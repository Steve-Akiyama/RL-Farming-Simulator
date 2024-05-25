# Simple Makefile for Monte Carlo PlantFarm MDP

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11

# Target executable
TARGET = monteCarloPF

# Source files
SRCS = monteCarloPF.cpp

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET):
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Clean up the build files
clean:
	rm -f $(TARGET)


