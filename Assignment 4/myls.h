#include <stdbool.h>
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
#include <stdint.h>
#define INODESIZE 14
#define DATESIZE 22
#define NAMESIZE 400
#define PERMISSIONSIZE 12
bool multipleDir;
bool l_flag;
bool i_flag;
bool r_flag;
void print_dir(char *directory);
void recursive_case(DIR *directory, char *tree);
void parse_args(char *argString);
void print_inode(char *directory, char *fname);
void file_info(char *tree, char *fname);
void file_desc(char *directory, char *fname);
