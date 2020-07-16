#include "list.h"
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#define BUFFER_SIZE 512
#define BAD_SOCKET  -1
typdef struct thread_constructor{
    List *pList;
    struct addrinfo *server;
    struct addrinfo *client;
    int filedes;
} thread_constructor;

void* get_data(void* data);

void* tx_data(void* data);

void* print_data(void* data);

void* rx_data(void* data);

int main(int argc, char *argv[]);
