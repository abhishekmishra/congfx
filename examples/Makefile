# Variables
CC = cc
CFLAGS = -c
EXE = ex0_point ex1_fill_lines ex2_bubble ex3_bubbles
INC = -I..

# Targets
all: $(EXE)

ex0_point: ex0_point.o
	$(CC) $< -o $@

ex1_fill_lines: ex1_fill_lines.o
	$(CC) $< -o $@

ex2_bubble: ex2_bubble.o
	$(CC) $< -o $@

ex3_bubbles: ex3_bubbles.o
	$(CC) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INC) $< -o $@

clean:
	rm -f *.o
	rm -f $(EXE)