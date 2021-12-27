#include "myls.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <pwd.h>
#include <stdint.h>
#include <stdbool.h>
#include <grp.h>

// Print file name with inode number
void print_inode(char *directory, char *fname) {
    struct stat buffer1; // stat buffer
    char buffer2[NAMESIZE];  // name buffer

    // Append directory name to file name.
    memset(buffer2, 0, NAMESIZE);
    strncpy(buffer2, directory, strlen(directory));
    if (buffer2[strlen(buffer2) - 1] != '/') {
        strncpy(buffer2 + strlen(buffer2), "/", 1);
    }
    strncpy(buffer2 + strlen(buffer2), fname, strlen(fname));
            
    // Get file info using stat system call.
    if (lstat(buffer2, &buffer1) == -1) {
        fprintf(stderr, "ls: %s: ", buffer2);
        perror("");
        exit(1);
    }
    
    printf("%lu", buffer1.st_ino);
    bool spaceexists = (strchr(fname, ' ')!=NULL);
    if(spaceexists){
        printf(" \'%s\'\n", fname);
    }
    else
    {
        printf(" %s\n",fname);
    }
}
