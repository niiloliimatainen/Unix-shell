/****************************************************************/

/* Creators: Niilo Liimantainen & Oskari Kivekäs */
/* Date: 20.04.2021 */
/* Sources: */
/*   1. https://brennan.io/2015/01/16/write-a-shell-in-c/ */
/*   2. https://stackoverflow.com/questions/29154056/redirect-stdout-to-a-file */
    
/****************************************************************/


#include "wish.h"


/* Built-in command for cd */
void wish_cd(char **args, int size) {
    
    if (size != 2) {
        write_error(2);
    
    } else {

        static char path[PATH_MAX] = "";
        strcat(path, getenv("PATH"));
        /* Check if chdir succeeds */
        if (chdir(args[1]) == -1) {
            write_error(3);
        }
        
        /* Update path variable after chdir */
        if (putenv(path) != 0) {
            write_error(-1);
            exit(1);
        }
    }
}


/* Function takes in new path and update it to the PATH environment variable */
void wish_path(char **args, int size) {
    static char path[PATH_MAX] ="PATH=";
     int i;
    
    /*Check for illegal & command*/
    for (i=1; i < size; i++) {
        if(strcmp(args[i], "&") == 0) {
            write_error(5);
            return;
        }  
    }

    /*If no parameters, empty the path*/
    if (size == 1){

        if (putenv("PATH=") != 0) {
            write_error(-1);
            exit(1);
        }
    
        return;
    }
    
    /*Add all user provided paths to path*/
    for(i=1; i < size; i++){
        strcat(path, args[i]);
        strcat(path, "/ ");
    }
    
    /* Update path variable */
    if (putenv(path) != 0){
        write_error(-1);
        exit(1);
    }
}


/* Built-in command for redirection. Function takes in filename where stdout is redirected. */
/* Inspiration for freopen is taken from 2. source */
FILE * wish_redirect(char *fname) {
    FILE *file;

    /* Redirecting stdout to fname */
    if ((file = freopen(fname, "w", stdout)) == NULL) {
        write_error(-1);
    }
    return file;
}


/* Own implementation of exit command */
void wish_exit(char *buffer) {
    free(buffer);
    exit(1);
}


 /* Helper function to write errors */
void write_error(int flag) {
    char error_message[MAXLEN];

    /* -1 -> error occured in malloc, realloc or file opening */
    if (flag == -1) {
        strcpy(error_message, "An error has occurred\n");

    /* 0 -> Shell was invoked with more than one batch file */
    } else if (flag == 0) { 
        strcpy(error_message, "Shell can be invoked with only one batch file\n");

    /* 1 -> there was a wrong amount of arguments in exit command */
    } else if (flag == 1) {
        strcpy(error_message, "'exit' command takes no arguments\n");
        
    /* 2 -> there was 0 or >1 arguments in cd command */    
    } else if (flag == 2) {
        strcpy(error_message, "'cd' command takes precisely one argument\n");
    
    /* 3 -> can't find the directory that was given to cd */
    } else if (flag == 3) {
        strcpy(error_message, "No such directory\n");
    
    /* 4 -> there must be a new command after '&' */
    } else if (flag == 4) {
        strcpy(error_message, "No command is given after '&'\n");

    /* 5 -> there can't be '&' with built-in commands */
    } else if (flag == 5) {
        strcpy(error_message, "'&' and built-in command can be used in same statement\n");

    /* 6 -> command can't be found */
    } else if (flag == 6) {
        strcpy(error_message, "Could not identify command\n");

    /* 7 -> The batch file is invalid */
    } else if (flag == 6) {
        strcpy(error_message, "Can't open batch file\n");
    
    /* If flag is something else, write universal error message */
    } else {
        strcpy(error_message, "An error has occurred\n");
    }

    write(STDERR_FILENO, error_message, strlen(error_message));
}
