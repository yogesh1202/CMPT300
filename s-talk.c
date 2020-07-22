#include "s-talk.h"
int ending = 1;

pthread_mutex_t data_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t tx_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t print_cond = PTHREAD_COND_INITIALIZER;

void* get_data(void* data)
{
    char *buffer = (char*)malloc(BUFFER_SIZE * sizeof(char));
    const char end_seq[3] = "!\n";

    while(ending)
    {
        //printf("Me:");
        fgets(buffer, BUFFER_SIZE, stdin);
        if(strcmp(buffer, end_seq) == 0 && strlen(buffer)==2)    /* Check to see if '!' was typed */           //&& strlen(buffer)) //COME BACK TO THIS
        {
            printf("! ENTERED. S-talk will now close.\n");
            ending = 0;
            break;
        }

        pthread_mutex_lock(&data_lock);     /* Acquiring Lock in Key thread */
        List_append((List*)data, buffer);  /* Add to list */ 
        pthread_cond_signal(&tx_cond);      /* Signalling Sender thread from Key thread */
        pthread_mutex_unlock(&data_lock);   /* Unaquiring Lock in Key thread */
    }

    pthread_cond_signal(&tx_cond);
    pthread_cond_signal(&print_cond);
    free(buffer);       /* Exiting thread */
    
    
    pthread_exit(&ending); 
}

void* tx_data(void* data)remove* Waiting for signal from Key thread in Sender thread */

        if(ending)
        {
            List_last(args->pList);
            msg = (char*)List_remove(args->pList);
            sendreturn = sendto(args->filedes, msg, BUFFER_SIZE,0, args->server->ai_addr, args->server->ai_addrlen);
            if(sendreturn<0)
            {
                printf("Failure to send.\nError no: %d... exiting\n", errno);
                exit(EXIT_FAILURE);
            }
        }

        pthread_mutex_unlock(&data_lock);       /* Unaquiring lock in Sender thread */
    } 

   pthread_exit(&ending);       /* Exiting Sender thread */
}

void* print_data(void* data)
{
    while(ending)
    {
        pthread_mutex_lock(&data_lock);     /* Acquiring lock in Print message thread */

        pthread_cond_wait(&print_cond, &data_lock);     /* waiting for signal from GET_MSG thread in Print message thread */

        if(ending)      /* Recquiring lock after receiving signal */
        {
            List_first((List*)data);
            buf = (char*)List_remove((List*)data);
            //printf("\";nFriend: ");
            fputs(buf, stdout);
            //, buf); //use fputs, send maybe
        }

        
    }

    pthread_exit(&ending);


}

/*
    Thread functions that receive messages from connection
*/

void* rx_data(void* data)
{
    thread_constructor *args = (thread_constructor*)data;
    char *msg = (char*)malloc(BUFFER_SIZE*sizeof(char));
    struct sockaddr_in remote_address;
    socklen_t len_addr = sizeof(remote_address);

  //  int recvreturn;
    while(ending)
    {
        int recvreturn = recvfrom(args->filedes, msg, BUFFER_SIZE-1, 0,\
        (struct sockaddr*)&remote_address, &len_addr);

        if(recvreturn>0)
        {

            pthread_mutex_lock(&data_lock);     /* Acquiring lock in get message thread */
            List_append(args->pList, msg);      
            
            pthread_cond_signal(&print_cond);   /* Signalling Print message thread from get message thread */
            pthread_mutex_unlock(&data_lock); //can we switch these two safely?

        }

    } 
    free(msg);
    //TODO: Do we want to free the list too?
    pthread_exit(&ending); //change ending to an integer maybe?
}

int main(int argc, char *argv[])
{


    if(argc<4)
    {
        printf("Fewer than 3 arguments supplied\n");
        exit(EXIT_FAILURE);
    }

    struct hostent *host;
    char* host_name = argv[2];    
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    fcntl(socket_fd, F_SETFL, O_NONBLOCK);

    if(socket_fd==BAD_SOCKET)
    {
        printf("Error - Couldn't open socket: %d\n", errno); /* Bad socket check */
		exit(0);
    }

    host = gethostbyname(host_name); /* Setup remote address */

	if (!host) 
    {
		printf("ERROR - Couldn't reach address specified.%d\n", errno); /* Error check if remote address reachable */
		exit(0);
	}

    struct addrinfo local_hints;
    memset(&local_hints, 0, sizeof(local_hints));
    local_hints.ai_family = AF_INET;
    local_hints.ai_socktype = SOCK_DGRAM;
    //local_hints.ai_protocol = IPPROTO_UDP;
    local_hints.ai_flags = AI_PASSIVE;

    struct addrinfo *peer, *my_machine;

    if(getaddrinfo(NULL, argv[1], &local_hints, &my_machine)!=0)
    {
        printf("Failure to get address info.\nError no: %d... exiting\n", errno);
        exit(EXIT_FAILURE);
    }

    local_hints.ai_flags = 0; //find out why these two lines
    local_hints.ai_protocol = 0;

    if(bind(socket_fd, my_machine->ai_addr, my_machine->ai_addrlen)<0)  /* bind socket */
    {
        printf("Failure to bind socket.\nError no: %d... exiting\n", errno);
        exit(EXIT_FAILURE);
    }

    if(getaddrinfo(argv[2], argv[3], &local_hints, &peer)!=0)
    {
        printf("Failure to get address info for peer.\nError no: %d... exiting\n", errno);
        exit(EXIT_FAILURE);
    } 

   /* Pass arguments into a thread using
    1. A list to hold messages
    2. File Descriptor for client socket
    */

    thread_constructor args = {List_create(), peer, my_machine, socket_fd};



    pthread_t Tx, Rx, in_stream, out_stream;
    pthread_create(&Tx, NULL, tx_data,&args); //do we wanna check for errors?
    pthread_create(&Rx, NULL, rx_data, &args);
    pthread_create(&in_stream, NULL, get_data, (void*)args.pList);
    pthread_create(&out_stream, NULL, print_data, (void*)args.pList);

    pthread_join(Rx, NULL);
    pthread_join(Tx, NULL);
    pthread_join(in_stream, NULL);
    pthread_join(out_stream, NULL);
    //List_free(args->pList);
    printf("Goodbye!\n");
    pthread_cond_destroy(&tx_cond);
    pthread_cond_destroy(&print_cond);
    pthread_mutex_destroy(&data_lock);
    


    exit(0);
} 
