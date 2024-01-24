CC = gcc
CFLAGS = -Wall
SRC = main.c parser.c logic.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = astar

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)
