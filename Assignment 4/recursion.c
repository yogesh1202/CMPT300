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

// Recursion: Retraverses the directory and prints the contents of any subdirectories


void recursive_case(DIR *directory, char *tree)
{
    char FILE_BUFFER[NAMESIZE];
    // Store DIR name
    struct stat buffer;
    struct dirent *dp = NULL;

    // Traverse entire directory using rewinddir, rewinddir resets the position of the directory stream ti beginning of directory
    
    rewinddir(directory);

    //Loop reading each name in the directory and go through any directories found.
    do
    {
        errno = 0;
        if ((dp = readdir(directory)) != NULL)
        {
            // Don't include hidden file/directory names.
            if (dp->d_name[0] != '.')
            {
                // Build the base path of the file/directory name.
                memset(FILE_BUFFER, 0, NAMESIZE);
                realpath(tree, FILE_BUFFER);

                if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
                {
                    // Construct a new path using the base path we constructed earlier
                    strcpy(FILE_BUFFER, tree);
                    strcat(FILE_BUFFER, "/");
                    strcat(FILE_BUFFER, &dp->d_name[0]);
                }

                // Get lstat info and return an error incase
                if (lstat(FILE_BUFFER, &buffer) == -1)
                {
                    fprintf(stderr, "ls: %s: ", FILE_BUFFER);
                    perror("");
                    exit(1);
                }
                
                // Print the directory contents if the given path is a directory.
                if (S_ISDIR(buffer.st_mode))
                {
                    print_dir(FILE_BUFFER);
                }
            }
        }
    } while (dp != NULL);
}
