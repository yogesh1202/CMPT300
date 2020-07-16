#include "s-talk.h"
bool ending = false;
void* get_data(void* data){
  
}


void* tx_data(void* data){
    
}

void* print_data(void* data){

}

void* rx_data(void* data){

}

int main(int argc, char *argv[]){
    if(argc<4){
        printf(stderr, "Usage: %s host port msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    printf("Chatting using port %s\n", argv[1]);
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_fd==BAD_SOCKET){
        //do something for bad socket
    }
    struct addrinfo local_hints;
    memset(&local_hints, 0, sizeof(local_hints));
    
    local_hints.ai_family = AF_INET;
    local_hints.ai_socktype = SOCK_DGRAM;
    local_hints.ai_protocol = IPPROTO_UDP;
    local_hints.ai_flags = AI_PASSIVE;
    struct *peer, *my_machine;
    hints.ai_flags = 0; //find out why these two lines
    hints.ai_protocol = 0;
    int errno = getaddrinfo(argv[2], argv[3], &local_hints, &peer);
    if(errno!=0){
        printf('Failure to get address info for peer.\nError no: %d... exiting\n', errno)
        exit(EXIT_FAILURE);
    } // temporary
    errno = getaddrinfo(NULL, argv[1], &local_hints, &my_machine);
    if(errno!=0){
        printf('Failure to get address info.\nError no: %d... exiting\n', errno)
        exit(EXIT_FAILURE);
    } // temporary
    errno = bind(socket_fd, my_machine->ai_addr, my_machine->ai_addrlen);
    if(errno!=0){
            printf('Failure to bind socket.\nError no: %d... exiting\n', errno)
            exit(EXIT_FAILURE);
    } // temporary
    struct thread_constructor args(ListCreate(), peer, my_machine, socket_fd);

    pthread_t Tx, Rx, in_stream, out_stream;
    pthread_create(&Tx, NULL, tx_data,&args); //do we wanna check for errors?
    pthread_create(&Rx, NULL, rx_data, &args);
    pthread_create(&in_stream, NULL, get_data, (void*)args.pList);
    pthread_create(&out_stream, NULL, print_data, (void*)args.pList);

    pthread_join(Rx, NULL);
    pthread_join(Tx, NULL);
    pthread_join(in_stream, NULL);
    pthread_join(out_stream, NULL);
    return 0;
}