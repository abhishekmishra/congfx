#Disable implicit rules
.SUFFIXES:

OSFLAG :=
ifeq ($(OS),Windows_NT)
	OSFLAG = WIN32
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OSFLAG = LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		OSFLAG = OSX
	endif
endif

# Variables
CC = cc
INC = -I..
CFLAGS = -c $(INC)
# All files that start with 'ex' and end with '.c'
EXE = $(patsubst %.c,%,$(wildcard ex*.c))

# if the os is windows, append the .exe extension to the executable
ifeq ($(OSFLAG),WIN32)
	EXE := $(EXE:=.exe)
endif

# Targets
all: $(EXE)
	echo $(EXE)
	echo $(OSFLAG)

%.exe: %.o
	echo "blah" $@
	$(CC) $< -o $@

%: %.o
	echo $@
	$(CC) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o
	rm -f $(EXE)