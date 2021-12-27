#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ANCESTOR_NAME_LEN 16

struct process_info {
    long pid;                     /* Process ID */
    char name[ANCESTOR_NAME_LEN]; /* Program name of process */
    long state;                   /* Current process state */
    long uid;                     /* User ID of process owner */
    long nvcsw;                   /* # voluntary context switches */
    long nivcsw;                  /* # involuntary context switches */
    long num_children;            /* # children process has */
    long num_siblings;            /* # sibling process has */
};




#define _PROCESS_ANCESTOR 550
static long test_syscall(struct process_info *testInfo, long size,long *num_filled);
static void spawn_processes(struct process_info *testInfo, long num_spawn, long size);
int main(int argc, char *argv[])
{
    struct process_info *testInfo = malloc(10* sizeof(struct process_info));
    struct process_info *testInfo2 = malloc(10* sizeof(struct process_info));
    long num_filled = 0;
    //Working case
    test_syscall(testInfo, 3, &num_filled); 
    num_filled = -1;
    // trying with negative and zero sizes and num_filled
    test_syscall(testInfo, -1, &num_filled);
    test_syscall(testInfo, 0, &num_filled);
    // trying with bad pointers for num filled and testinfo
    test_syscall(NULL, 10, &num_filled);
    test_syscall((void*)3, 5, &num_filled);
    test_syscall(testInfo, 5, NULL);
    test_syscall(testInfo, 5, (void*)5);
    //trying with very large array and nowhere to increment num filled
    test_syscall(testInfo, (long)1024*1024, NULL);
    //working case 
    test_syscall(testInfo, 5, &num_filled);
    // spawning children now
    // note that processes are non-deterministic so the order may be jumbled, and syscall maybe called for multiple 
    spawn_processes(testInfo2, 2, 20);
    // trying to spawn no children with no size present, error case
    spawn_processes(testInfo2, 0, 0);
    spawn_processes(testInfo2, -1, -3);
    //working case with a single child spawned
    spawn_processes(testInfo2, 3, 15);
    free(testInfo);
    free(testInfo2);
}

static void spawn_processes(struct process_info *testInfo, long num_spawn, long size){
    pid_t pid;                     /* Process ID */
    int i = 1;                    /* Used as a count of how many processes to spawn */
    int state;                   /* Current process state */
    long num_filled = 0;
    if(num_spawn < 0 || size < 0)
    {
        printf("Non-positive size and/or number of child processses requested\n");
        return;
    }
    else if(num_spawn == 0 )
    {
        //if not looking to spawn any children, proceed as a normal system call
        test_syscall(testInfo, size, &num_filled);
        return;
    }
    //otherwise fork and make sure you have received the PID of the child
    pid = fork();
    if(pid<0)
    {
        perror("fork()");
    }
    else if(pid!=0)
    {
        //wait to receive a zero, indicating child
        waitpid(pid, &state, 0);
        exit(0); 
        //the exit statement makes sure we don't run system call 
        //multiple times while waiting for a process to be created
    }
    if(pid==0){
        //make new child for every child
        for(; i <num_spawn; i++)
        {
            pid = fork();
            if(pid < 0)
            {
                perror("fork()");

            }
            else if (pid != 0)
            {
                waitpid(pid, &state, 0);
                exit(0);
            }
        }
        if(pid==0)
        {
            test_syscall(testInfo, size, &num_filled);
        }
    }
    //if we're at the bottom of tree, and at the child now, call syscall
}
static long test_syscall(struct process_info *testInfo, long size,long *num_filled)
{
    int result = 1;
    int i = 0;
    int errorCode = errno;
    printf("\nDiving to kernel level\n");
    //make syscall and record result
    result = syscall(_PROCESS_ANCESTOR,testInfo,size,num_filled); 
    //if result is less than 1 we have an error, output right errorCode
    if(result<0){
        errorCode = errno;
        printf("Rising to user level with error %d\n\n", result*errorCode);
        return result*errorCode;
    }
    //otherwise continue as nornal and print out result as a table
    printf("Rising to user level with result %d\n", result*errorCode);
    printf("%ld processes filled in array of size %ld\n", *num_filled,size);
    if (testInfo!=NULL)
    {
        while (i < *num_filled)
        {
            printf("PID:%-6ld\tName:%-16s\tState:%-6ld\tUID:%-6ld\tNVCSW:%-6ld\tNIVCSW:%-6ld\t# of Children:%-6ld\t# of Siblings:%ld\n",
            (testInfo+i)->pid, 
            (testInfo+i)->name, 
            (testInfo+i)->state, 
            (testInfo+i)->uid, 
            (testInfo+i)->nvcsw, 
            (testInfo+i)->nivcsw, 
            (testInfo+i)->num_children,
            (testInfo+i)->num_siblings);
            i++;
        }
    }
    //return result;
    return result;
}

