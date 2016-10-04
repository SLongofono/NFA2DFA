#######################################################################
# General Makefile for compiling c programs
#
# Author : wali@ku.edu
#######################################################################

# Specify the name of the program to build
PROG_NAME 	:= assignment_1

# Specify CFlags for the program
CFLAGS		:= -g -Wno-format-zero-length

# Specify all the source files required for this program
SRC 		:= $(wildcard *.c)

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

# Create a generalized rule for compiling cplusplus source files
%.o : %.c
	@gcc $(CFLAGS) -c $<

# An intermediate rule for just compiling the program
compile : $(PROG_NAME)
	@echo "Compilation Complete!"

# This is the clean target
clean:
	@rm -f *.o *~ $(PROG_NAME)
