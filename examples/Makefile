# Variables
CC = cc
INC = -I..
CFLAGS = -c $(INC)
# All files that start with 'ex' and end with '.c'
EXE = $(patsubst %.c,%,$(wildcard ex*.c))

# Targets
all: $(EXE)

%: %.o
	$(CC) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o
	rm -f $(EXE)