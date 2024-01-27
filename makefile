CC = gcc
CFLAGS = -Wall
SRC = $(wildcard *.c)
EXECUTABLE = astar

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(EXECUTABLE)