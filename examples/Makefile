# Variables
CC = cc
CFLAGS = -c
EXE = ex1_fill_lines ex2_bubbles
INC = -I..

# Targets
all: $(EXE)

ex1_fill_lines: ex1_fill_lines.o
	$(CC) $< -o $@

ex2_bubbles: ex2_bubbles.o
	$(CC) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INC) $< -o $@

clean:
	rm -f *.o
	rm -f $(EXE)