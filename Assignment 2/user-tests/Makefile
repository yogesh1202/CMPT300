# Makefile for building test applications for  QEMU system
# Inspired by Brian Fraser through the CMPT 300 Workshop
TARGET=cs300_testapp array_stats_testapp process_ancestor_testapp

CC_C = gcc

CFLAGS = -Wall -g -std=c99 -D _GNU_SOURCE -static -pthread

all: clean $(TARGET)

$(TARGET):
	$(CC_C) $(CFLAGS) $@.c -o $@

clean:
	rm -f $(TARGET)

transfer:
	scp -P 10022 $(TARGET) root@localhost:~

killqemu: killall qemu-system-x86_64
