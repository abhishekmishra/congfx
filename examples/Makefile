# Variables
CC = cc
CFLAGS = -c
OBJ = file1.o file2.o

# Targets
all: $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o