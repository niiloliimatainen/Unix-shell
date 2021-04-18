/*
Sources:
    1. https://brennan.io/2015/01/16/write-a-shell-in-c/
    2. https://stackoverflow.com/questions/29154056/redirect-stdout-to-a-file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define MAXLEN 64


void interactive_mode();
void write_error();
void parse_command(char *buffer);
void shell_execute(char **args, int size);
void wish_cd(char **args, int size);
void wish_exit(char *token, char *buffer);
void batch_mode(FILE *file);
FILE * wish_redirect(char *fname);


/* Parsing the arguments and executing them */
int main(int argc, char *argv[]) {
    FILE *file;

    /* If no arguments given, starts an interactive mode */
    if (argc == 1) {
        interactive_mode();
    
    /* If one argument is given, executes that file with batch mode */
    } else if (argc == 2) {

        if ((file = fopen(argv[1], "r")) == NULL) {
                write_error();
			    exit(1);
        } 
        batch_mode(file);
        fclose(file);
   
    /* If more than one argument is give, exit the program */
    } else {
        return 0;
    }
    printf("\n");
    return 0;
}


/* An infinite loop that takes commands from stdin and executes them. The loop stops when "exit" is given. */
void interactive_mode() {
    char *buffer = NULL;
    size_t bufsize = 0;
    
    while (1) {
        printf("wish> ");

        if (getline(&buffer, &bufsize, stdin) == EOF) {
            exit(0);
        }

        parse_command(buffer);
    }
    free(buffer);
}


/* Reading commands from .wh type of file and executing them */
void batch_mode(FILE *file) {
    char *buffer = NULL;
    size_t bufsize = 0;

    while (getline(&buffer, &bufsize, file) != EOF) {
        parse_command(buffer);
    }
    printf("hommat toimi\n");
    free(buffer);
}


/* Inspiration for args list is taken from 1. source */
void parse_command(char *buffer) {
    char *token, **args, delim[1] = " ";
    int index = 0, size, maxlen = MAXLEN;
    FILE *file = NULL;

    /* Return if input is empty, else remove newline from the end */ 
    if ((size = strcspn(buffer, "\n")) == 0) {
        write_error();
        return;

    } else {
        buffer[size] = '\0';
    }

    token = strtok(buffer, delim);

    /* Own implementation of exit command */
    if (strcmp("exit", token) == 0) {
        if ((token = strtok(NULL, " ")) != NULL) {
            write_error();
            return;
        } else {
            free(buffer);
            exit(0);
        }
    }

    if ((args = malloc(maxlen * sizeof(char*))) == NULL) {
        write_error();
        exit(0);
    }

    while (1) {
        if (token == NULL) {
            break;

        } else if (strcmp(">", token) == 0) {

            /* If no output file, the output is written to stdout */
            if ((token = strtok(NULL, delim)) == NULL) {
                break;
            }
            file = wish_redirect(token);
            break;
        }

        args[index] = token;
        index++;

        if (index >= MAXLEN) {
            maxlen += MAXLEN;
            if ((args = realloc(args, maxlen * sizeof(char*))) == NULL) {
                write_error();
                exit(0);
            }
        }
        
        token = strtok(NULL, delim);
    }
    
    shell_execute(args, index);
    free(args);
    
    /* If there is redirect, close the file and resume standard behavior of stdout */
    if (file != NULL) {
        fclose(file);
        /* This is taken from 2. source */
        freopen("/dev/tty", "w", stdout);
    }
}


void write_error() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}


/*Get user wanted path as param: f.ex: (current path) + /oskukoodaa/niilokoodaa*/
void wish_cd(char **args, int size) {
    if (size != 1) {
        write_error();
    
    } else {
        /* Check if chdir succeeds */
        if (chdir(args[1]) == -1) {
            write_error();
        }
    }
}


void wish_path() {

}


FILE * wish_redirect(char *fname) {
    FILE *file;
    if ((file = freopen(fname, "w", stdout)) == NULL) {
        write_error();
    }
    return file;
}


void shell_execute(char **args, int size) {
    for (int i = 0; i < size; i++) {
        printf("%s", args[i]);
    }
}

