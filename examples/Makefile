# Variables
CC = cc
CFLAGS = -c
# All files that start with 'ex' and end with '.c'
EXE = $(patsubst %.c,%,$(wildcard ex*.c))
INC = -I..

# Targets
all: $(EXE)

%: %.o
	$(CC) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INC) $< -o $@

clean:
	rm -f *.o
	rm -f $(EXE)