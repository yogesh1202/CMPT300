CC=gcc
CFLAGS=-g -Wall -Werror -I.
PROG=myls
OBJS=myls.o	print_dir.o	recursion.o	file_desc.o	inode.o	file_info.o

myls: $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $*.c

clean:
	rm *.o myls 