CXX = g++
CXXFLAGS = -Wall
DEBUGFLAGS = -g  # Additional flags for debugging
SRC = $(wildcard *.cpp)
EXECUTABLE = astar

all: $(EXECUTABLE)

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(EXECUTABLE)

$(EXECUTABLE): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(EXECUTABLE)
