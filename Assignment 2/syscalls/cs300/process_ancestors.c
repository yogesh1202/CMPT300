#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/cred.h>
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

SYSCALL_DEFINE3(process_ancestors,                /* syscall name for macro */
struct process_info*, info_array, /* array of process info strct */
long, size,                       /* size of the array */
long*, num_filled)                /* # elements written to array */
{
    //Firstly set up a local copy of everything in process info
    int i = 0;
    long counter = 0; // used to count how many elements in array were filled
    bool atInit = false; // used to see how close we are to the swapper
    struct process_info localInfo; 
    struct list_head *child;
    struct list_head *sibling;
    //get current process as the local task
    struct task_struct *localTask = current; 
    localInfo.pid = 0;                      
    localInfo.state = 0;                     
    localInfo.uid = 0;                      
    localInfo.nvcsw = 0;                     
    localInfo.nivcsw = 0;                   
    localInfo.num_children = 0;              
    localInfo.num_siblings = 0;
    if (size <= 0)
    {
        printk("Size<=0, EINVAL");
        return -EINVAL;
    }
    do
    {  
        if (localTask == localTask->parent)
        {
            atInit = true;
        }
        //Assigning process attributes to local info struct 
        localInfo.pid = (long) localTask->pid;
        localInfo.state = (long) localTask->state;                     
        localInfo.uid = (long) localTask->cred->uid.val;                       
        localInfo.nvcsw = (long) localTask->nvcsw;                     
        localInfo.nivcsw = (long) localTask->nivcsw;
        //used to copy 16-bit name from localTask->comm
        memcpy(localInfo.name,localTask->comm,sizeof(localInfo.name));
        //Using list_head structure to traverse through children and sibling list and increment count
        list_for_each(child,&localTask->children)
        {
            localInfo.num_children++;
        }
        list_for_each(sibling,&localTask->sibling)
        {
            localInfo.num_siblings++;
        }
        //if copy_to_user fails to copy the struct back to user level return EFAULT
        if (copy_to_user(&info_array[i], &localInfo, sizeof(localInfo))!=0)
        {
            return -EFAULT;
        }
        i++;
        counter++;
        //if copy_to_user fails to copy the num_filled value back to user
        if (copy_to_user(num_filled, &counter, sizeof(long))!=0)
        {
            return -EFAULT;
        }
        //move onto the current process parent
        localTask = localTask->parent;
    } while (counter < size && atInit == false);

    return 0;

}