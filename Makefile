# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11

# Target executable
TARGET = PlantFarmApp

# Source files
SOURCES = Main.cpp PlantFarm.cpp MonteCarlo.cpp TerminalUI.cpp 

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile the source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(TARGET) $(OBJECTS)



