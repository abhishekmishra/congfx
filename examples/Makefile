# Variables
CC = cc
CFLAGS = -c
EXE = ex1_fill_lines ex2_bubble
INC = -I..

# Targets
all: $(EXE)

ex1_fill_lines: ex1_fill_lines.o
	$(CC) $< -o $@

ex2_bubble: ex2_bubble.o
	$(CC) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INC) $< -o $@

clean:
	rm -f *.o
	rm -f $(EXE)