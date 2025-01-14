# Variables
CC = cc
CFLAGS = -c
OBJ = file1.o file2.o
INC = -I..

# Targets
all: $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) $< -o $@

clean:
	rm -f *.o