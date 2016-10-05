#######################################################################
# General Makefile for compiling c programs
#
# Author : wali@ku.edu
#
# NOTE:
# 	In order to compile the program without runnin it, please
# 	use the following command:
# 	>>make compile
#
# 	This will create 'nfa2dfa' executable which can be used
# 	for testing:
# 	>>./nfa2dfa < input-file-name
#
# 	The default make target will compile the proram and run
# 	it with default 'input.txt' file
#######################################################################

# Specify the name of the program to build
PROG_NAME 	:= nfa2dfa

# Specify CFlags for the program
CFLAGS		:= -g -Wall -I ./include/

# Specify all the source files required for this program
SRC 		:= $(wildcard *.c) $(wildcard src/*.c)

# Specify the name of the file which should be input to this program
INPUT		:= input.txt

# Target for rapid prototyping
all : execute clean

# Target for executing the program
execute : $(PROG_NAME)
	@./$< < input.txt

# Create a rule for linking the compiled files into executable object
$(PROG_NAME) : $(SRC)
	@gcc $(CFLAGS) -o $@ $^

# Create a generalized rule for compiling c-source files
%.o : %.c
	@gcc $(CFLAGS) -c $<

# An intermediate rule for just compiling the program
compile : $(PROG_NAME)
	@echo "Compilation Complete!"

# This is the clean target
clean:
	@rm -f *.o *~ $(PROG_NAME)
