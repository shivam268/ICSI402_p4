#Parikh Shivam
#ICSI 402 Assignment 2
#UNIX ID: sp191221

#The following rule tells make about possible suffixes
#(extensions) of file names.

.SUFFIXES: .c .o

#The following definition of CC ensures that
#gcc will be used to compile the C source files.

CC = gcc

#The following definition of CFLAGS ensures that
#the debugger can be used with the executable file (sample)
#created by running make.

CFLAGS = -g

#The following rule tells make how a ".o" file should
#be created from the corresponding ".c" file.
#Note that the "-c" option must be used here since we are
#compiling source files separately. (Note that the line
#following the ".c.o:" line begins with the "tab" character.)

.c.o:
	$(CC) $(CFLAGS) -c $<

#Dependency rule for the default target and how the
#default target is to be created. (Note that the line
#following the dependency rule begins with the "tab"
#character.)

p4:   main.o  funcFirst.o funcSecond.o
	gcc main.o funcFirst.o funcSecond.o -o p4

#Dependency rules for other targets. (We don't need to
#specify how these targets are created since we have already
#given a general rule for creating a ".o" file from the
#corresponding ".c" file.)

main.o:  globals.h constants.h prototypes.h struct.h

funcFirst.o:  externs.h constants.h struct.h prototypes.h

funcSecond.o: externs.h constants.h prototypes.h struct.h

#Target for removing unnecessary files.

clean:
	rm -f *.o core
