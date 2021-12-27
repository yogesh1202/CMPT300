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

// Prints the files and directories depending on the flags set

void file_info(char *tree, char *fname) {
    // Formatting for l flag = false.
    if (l_flag == false) {
        if (i_flag == true) {
            // Print simple name with inode number.
            print_inode(tree, fname); 
        } else {
            // Print simple name. 
            bool spaceexists = (strchr(fname, ' ')!=NULL);
            if(spaceexists){
                printf("\'%s\'\n", fname);
            }
            else
            {
                printf("%s\n",fname);
            }   
        }
    } else { // Formatting for l flag = true. 
        // Print long format. 
        file_desc(tree, fname);        
    }
}
