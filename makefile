CXX = g++
CXXFLAGS = -Wall
SRC = $(wildcard *.cpp)
EXECUTABLE = astar

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(EXECUTABLE)
