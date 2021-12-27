CC=gcc
CCFLAGS=-Wall
OBJ = myTest.o list.o
make: list.c myTest.c
	$(CC) $(CCFLAGS) -o myTest myTest.c list.c
all: list.c myTest.c
	$(CC) $(CCFLAGS) -o myTest myTest.c list.c
clean: 
	rm -f $(OBJ) myTest *~