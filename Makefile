# Simple Makefile for Monte Carlo PlantFarm MDP

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11

# Target executable
TARGET = reinforcePF

# Source files
SRCS = reinforcePF.cpp

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET):
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Clean up the build files
clean:
	rm -f $(TARGET)


