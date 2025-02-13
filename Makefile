# all clean and test are phony targets
# i.e. they are not files
.PHONY: all clean test run

SUBDIRS = examples

# default target is all, and it builds the test
all:
# make all subdirectories
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir all; \
	done	

run: all
	./examples/ex5_bouncing_balls

# clean all object files and executables.
clean:
	rm -f *.o
	rm -f *.exe

# clean all subdirectories
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done	