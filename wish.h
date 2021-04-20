/****************************************************************/

/* Creators: Niilo Liimantainen & Oskari Kivekäs */
/* Date: 20.04.2021 */
/* Sources: */
/*   1. https://brennan.io/2015/01/16/write-a-shell-in-c/ */
/*   2. https://stackoverflow.com/questions/29154056/redirect-stdout-to-a-file */
    
/****************************************************************/


#ifndef wish_h
#define wish_h


#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>


#define MAXLEN 64


void interactive_mode();
void write_error(int flag);
void parse_command(char *buffer);
void wish_fork_exec(char **args);
void wish_launch(char **args, int size);
void wish_cd(char **args, int size);
void wish_exit(char *buffer);
void batch_mode(FILE *file);
void wish_path(char **args, int size);
const char* check_path(char* prog_name);
FILE * wish_redirect(char *fname);


#endif