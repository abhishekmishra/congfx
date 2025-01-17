# Variables
CC = cc
INC = -I..
CFLAGS = -c $(INC)
# All files that start with 'ex' and end with '.c'
EXE = $(patsubst %.c,%,$(wildcard ex*.c))

# if the os is windows, append the .exe extension to the executable
ifeq ($(OS),Windows_NT)
	EXE := $(EXE:=.exe)
endif

# Targets
all: $(EXE)

ifeq ($(OS),Windows_NT)
%.exe: %.o
	$(CC) $< -o $@
else
%: %.o
	$(CC) $< -o $@
endif

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o
	rm -f $(EXE)