# all clean and test are phony targets
# i.e. they are not files
.PHONY: all clean test run

SUBDIRS = examples

# default target is all, and it builds the test
all: test
# make all subdirectories
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir all; \
	done	

run: all
	./test0

# depends on all test files
test: test0

test0: test0.o
	cc -lm test0.o -o test0

test0.o: test0.c
	cc -c test0.c

# clean all object files and executables.
clean:
	rm -f *.o
	rm -f *.exe
	rm -f test0

# clean all subdirectories
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done	