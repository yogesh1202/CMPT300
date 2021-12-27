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

// Store the directory name in FILE_BUFFER
char FILE_BUFFER[NAMESIZE];
// Store the inode number in INODE_BUFFER
char INODE_BUFFER[INODESIZE];

int main(int argc, char *argv[])
{
    l_flag = false;
    i_flag = false;
    r_flag = false;
    uint8_t argIndex = 1;

    // Parse all the arguments that start with a '-'
    char *currentArg = argv[argIndex];
    while (argIndex < argc && currentArg[0] == '-')
    {
        parse_args(argv[argIndex]);
        currentArg = argv[++argIndex];
    }

    // Check if there are multiple directories to be printed
    if (argc - argIndex > 1)
    {
        multipleDir = true;
    }
    
    // print contents of "."
    if (argIndex >= argc)
    {
        print_dir(".");
    }
    else
    {
        // Print directory contents for the remaining arguments
        for (int i = argIndex; i < argc; i++)
        {
            currentArg = argv[i];
            print_dir(currentArg);
        }
    }

    exit(0);
}

void parse_args(char *argString)
{
    // Check which flags are set
    if (i_flag == true && l_flag == true && r_flag == true)
    {
        return;
    }

    // Parse characters for flags
    for (size_t i = 1; i < strlen(argString); i++)
    {
        switch (argString[i])
        {
        case 'i':
            i_flag = true;
            break;
        case 'l':
            l_flag = true;
            break;
        case 'R':
            r_flag = true;
            break;

        default:
            // Error case
            printf("ls: Invalid argument specified. Only i, l, or R allowed.\n");
            exit(1);
        }
    }
}
