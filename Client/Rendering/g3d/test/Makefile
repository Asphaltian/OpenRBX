# source/test/Makefile
#
# Make the Test project
#
# The compiled executable will be copied to source/test

IC = ../bin/icompile

all:
	@echo "Building Test Project"; \
	chmod u+x $(IC); \
	$(IC); \
	cp -f 'build/install/testd' ./

