# Variables
CC = cc
CFLAGS = -c
EXE = ex1_fill_lines
INC = -I..

# Targets
all: $(EXE)

ex1_fill_lines: ex1_fill_lines.o
	$(CC) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INC) $< -o $@

clean:
	rm -f *.o
	rm -f $(EXE)