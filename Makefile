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

# create the documentation for local use.
docs: docs-api docs-mkdocs

docs-api:
	# generate API documentation using Doxygen
	mkdir -p docs
	doxygen Doxyfile

docs-mkdocs:
	# to install mkdocs and mkdocs-material, run:
	# pip install mkdocs mkdocs-material
	mkdocs build --clean

# clean all object files and executables.
clean:
	rm -f *.o
	rm -f *.exe

# clean all subdirectories
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
