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

// for -l flag print
// Gets file info for ls -l and prints relevant information for file/dir

void file_desc(char *directory, char *fname)
{
    struct stat buffer1;
    char buffer2[NAMESIZE];
    char inodebuf[INODESIZE];
    char lbuffer[NAMESIZE];
    memset(&buffer1, 0, sizeof(struct stat));

    // Append directory name to file name. 
    memset(buffer2, 0, NAMESIZE);
    strncpy(buffer2, directory, strlen(directory));

    if (buffer2[strlen(buffer2) - 1] != '/')
    {
        strncpy(buffer2 + strlen(buffer2), "/", 1);
    }
    strncpy(buffer2 + strlen(buffer2), fname, strlen(fname));

    // Get file info using stat system call. 
    if (lstat(buffer2, &buffer1) == -1)
    {
        fprintf(stderr, "ls: %s: ", buffer2);
        perror("");
        exit(1);
    }

    // Check if we should print the inode number. 
    memset(inodebuf, 0, INODESIZE);
    if (i_flag)
    {
        snprintf(inodebuf, INODESIZE, "%-10lu ", buffer1.st_ino);
    }

    // Check if the file is a directory or symbolic link. 
    char dirChar = '-';
    memset(lbuffer, 0, NAMESIZE);
    if (S_ISDIR(buffer1.st_mode))
    {
        dirChar = 'd';
    }
    else if (S_ISREG(buffer1.st_mode))
    {
        dirChar = 'd';
    }
    else if (((buffer1.st_mode) & (S_IFMT)) == (S_IFLNK))
    {
        dirChar = 'l';
        ssize_t length = 0;
        snprintf(lbuffer, strlen(" -> ") + 1, " -> ");
        if ((length = readlink(buffer2,
                            lbuffer + strlen(lbuffer),
                            sizeof(lbuffer) - strlen(lbuffer))) == -1)
        {
            fprintf(stderr, "ls: %s: ", lbuffer);
            perror("");
            exit(1);
        }
    }

    // Parse permission string
    char *permissionString = (char *)malloc(sizeof(char) * PERMISSIONSIZE);
    
    // Build a permission string and stores it in a char pointer

    memset(permissionString, 0, PERMISSIONSIZE);
    //Source: codewiki.wikidot.com/csytem-calls:stat
    permissionString[0] = (buffer1.st_mode & S_IRUSR) ? 'r' : '-';
    permissionString[1] = (buffer1.st_mode & S_IWUSR) ? 'w' : '-';
    permissionString[2] = (buffer1.st_mode & S_IXUSR) ? 'x' : '-';
    permissionString[3] = (buffer1.st_mode & S_IRGRP) ? 'r' : '-';
    permissionString[4] = (buffer1.st_mode & S_IWGRP) ? 'w' : '-';
    permissionString[5] = (buffer1.st_mode & S_IXGRP) ? 'x' : '-';
    permissionString[6] = (buffer1.st_mode & S_IROTH) ? 'r' : '-';
    permissionString[7] = (buffer1.st_mode & S_IWOTH) ? 'w' : '-';
    permissionString[8] = (buffer1.st_mode & S_IXOTH) ? 'x' : '-';

    // Parse date string
    char *dateString = (char *)malloc(sizeof(char) * DATESIZE);
    strftime(dateString, DATESIZE, "%b %e %Y %R", localtime(&(buffer1.st_mtime)));

    // print everything for -l
    printf("%s%c%s%2lu %s %s %10lu %s",
           inodebuf,
           dirChar,
           permissionString,
           buffer1.st_nlink,
           getpwuid(buffer1.st_uid)->pw_name,
           getgrgid(buffer1.st_gid)->gr_name,
           buffer1.st_size,
           dateString);
    bool spaceexists = (strchr(fname, ' ')!=NULL);
    if(spaceexists){
        printf(" \'%s\'", fname);
    }
    else
    {
        printf(" %s",fname);
    }
    printf("%s\n", lbuffer);
    // Free allocated dynamic memory.
    free(permissionString);
    free(dateString);
}
