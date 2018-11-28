CC= gcc
CFLAGS= -c -Wvla -Wall -Wextra
CODEFILES= ex2.tar extension.pdf MyString.c MyString.h LineSeparator.c LineSeparator.h Makefile valdbg_MyString valdbg_LineSeparator
IOFILEPATH= /cs/stud/snirsh/ClionProjects/ex2/inputOutput/


# All Target
all: MyString LineSeparator

# Exceutables
MyString: MyString.o
	$(CC) MyString.o -o MyString

LineSeparator: LineSeparator.o
	$(CC) LineSeparator.o -o LineSeparator

# Object Files
LineSeparator.o: LineSeparator.c LineSeparator.h
	$(CC) $(CFLAGS) LineSeparator.c -o LineSeparator.o
	 
MyString.o: MyString.c MyString.h
	$(CC) $(CFLAGS) MyString.c -o MyString.o

# tar
tar:
	tar -cf $(CODEFILES)

# test
test: LineSeparator
	LineSeparator LineSeparator.in > mlinesep.txt
	diff linesep.txt mlinesep.txt

# Other Targets
clean:
	-rm -f *.o MyString LineSeparator

# VG
valgrind:
	valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --undef-value-errors=yes --log-file="valdbg_LineSeparator" LineSeparator LineSeparator.in
	valgrind --leak-check=full --show-possibly-lost=yes --show-reachable=yes --undef-value-errors=yes --log-file="valdbg_MyString" MyString MyStringExample.c