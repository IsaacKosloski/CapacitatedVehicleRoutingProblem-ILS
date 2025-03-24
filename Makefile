CXX = g++
CXXFLAGS = -Wall -std=c++20 -g -O0

SRC_FILES = src/main.cpp src/Node.cpp src/Component.cpp src/Vehicle.cpp src/Scanner.cpp src/CVRP.cpp src/Solution.cpp  src/Functions.cpp src/Solver.cpp
TARGET = bin/cvrp_solver

all:
	$(CXX) $(CXXFLAGS) $(SRC_FILES) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
