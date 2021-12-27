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


 // Prints all the directory contents based on the flag specified.

void print_dir(char *directory)
{
    DIR *dir = NULL;
    struct dirent **nameList = NULL;
    struct stat fileInfo;

    // Keep track of which directory to print using localDir 
    char localDir[NAMESIZE];
    memset(localDir, 0, sizeof(localDir));
    strncpy(localDir, directory, strlen(directory));

    

    // This successfully identifies when the program calls to a file and calls file info 
    stat(localDir, &fileInfo);
    if (S_ISREG(fileInfo.st_mode))
    {
       // printf("%s\n", directory);
        file_info(".", localDir);
        return;
    }
    // Print the directory name if more than one is being printed  
    if (multipleDir)
    {
        printf("\n%s:\n", localDir);
    }

    // Open the current directory
    if ((dir = opendir(localDir)) == NULL)
    {
        fprintf(stderr, "ls: %s: ", localDir);
        perror("");
        exit(1);
    }

    // Handle Sorting using alphasort 
    int n = scandir(localDir, &nameList, NULL, alphasort);
    for(int i = 0; i<n; i++){
        if (nameList[i]->d_name[0] == '.')
        {
            free(nameList[i]);
            continue;
        }
        file_info(localDir, nameList[i]->d_name);
        free(nameList[i]);
    }
    free(nameList);


    multipleDir = true;

    // Handle recursion if R flag is set
    if (r_flag == true)
    {
        recursive_case(dir, localDir);
    }

    // Close the current directory
    if (closedir(dir))
    {
        fprintf(stderr, "ls: %s: ", localDir);
        perror("");
        exit(1);
    }
}
