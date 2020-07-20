#include "s-talk.h"
static bool ending = false;

static pthread_mutex_t data_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t tx_cond = PTHREAD_COND_INITIALIZER;
static ptrhead_cond_t print_cond = PTHREAD_COND_INITIALIZER;

void* get_data(void* data)
{
  
}

void* tx_data(void* data)
{
    thread_constructor *args = (thread_constructor*)data;
    char *msg;
    int sendreturn;
    for(;;)
    {
        pthread_mutex_lock(&data_lock);

        pthread_cond_wait(&tx_cond, &data_lock);
        if(ending==false)
        {
            List_last(args->pList);
            msg = (char*)List_remove(args->pList);
            sendreturn = sendto(args->fd, msg, BUFFER_SIZE,0, args->server->ai_addr, args->server->ai_addr_len);
            if(sendreturn<0)
            {
                printf('Failure to sebnd.\nError no: %d... exiting\n', errno)
                exit(EXIT_FAILURE);
            }
        }
        pthread_mutex_unlock(&data_lock);
    } 
    pthread_exit(&ending)
}

void* print_data(void* data)
{

}

void* rx_data(void* data)
{

}

int main(int argc, char *argv[])
{
    if(argc<4)
    {
        printf(stderr, "Usage: %s host port msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Chatting using port %s\n", argv[1]);
    
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(socket_fd==BAD_SOCKET)
    {
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
    int ret = getaddrinfo(argv[2], argv[3], &local_hints, &peer);

    if(ret!=0)
    {
        printf('Failure to get address info for peer.\nError no: %d... exiting\n', errno)
        exit(EXIT_FAILURE);
    } // temporary


    ret = getaddrinfo(NULL, argv[1], &local_hints, &my_machine);

    if(ret!=0)
    {
        printf('Failure to get address info.\nError no: %d... exiting\n', errno)
        exit(EXIT_FAILURE);
    } // temporary
    ret = bind(socket_fd, my_machine->ai_addr, my_machine->ai_addrlen);
    if(ret!=0)
    {
        printf('Failure to bind socket.\nError no: %d... exiting\n', errno)
        exit(EXIT_FAILURE);
    } // temporary
    /* Pass arguments into a thread using
    1. A list to hold messages
    2. File Descriptor for client socket
    */

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