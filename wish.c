#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void interactive_mode();
void write_error();
void parse_command(char *buffer);
void cd(char *buffer);


int main(int argc, char *argv[]) {

    if (argc == 1) {
        interactive_mode();
    
    } else if (argc == 2) {
        printf("juttuja\n");
   
    } else {
        return 0;
    }
    printf("\n");
    return 0;
}


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
}


void parse_command(char *buffer) {
    char *token;
    int index;

    /* Return if input is empty, else remove newline from the end */ 
    if ((index = strcspn(buffer, "\n")) == 0) {
        write_error();
        return;

    } else {
        buffer[index] = '\0';
    }

    token = strtok(buffer, " ");

    if (strcmp("exit", token) == 0) {
        if ((token = strtok(NULL, " ")) != NULL) {
            write_error();
        
        } else {
            free(buffer);
            exit(0);
        }

    } 
    
    /*else if (strcmp("cd", token) == 0) {
        cd(token);
    
    } else if (strcmp("path", token) == 0) {
        printf("polkujuttuja\n");

    } else {
        printf("juhuu\n");
    }*/
}



void write_error() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}


void batch_mode() {

}
/*Get user wanted path as param: f.ex: (current path) + /oskukoodaa/niilokoodaa*/
void cd(char *token) {
    int count = 0;
    
    while (1) {
        if ((token = strtok(NULL, " ")) == NULL) {
            break;
        }
        count++;
    }

    printf("%d\n", count);

    if (count != 1) {
        write_error();
    
    } else {
        printf("siirtoi ja sillee\n");
    }
}


void path() {

}


void redirect() {

}

void shell_execute(char *args[]){
    /*CASE INTERACTIVE*/
    /* Fork() current process to create a copy
       to know which process is which, we look at the process id's
       -> parent has child_pid of child and child has child_pid of 0
       
       use exec variant execvp() (or something) to run wanted program in child process created by fork
       exec + v(takes vector of parameters) p (finds program by its name)

       parent waits until child has completed
       add a shitton of if's and error checks and validate that child process has ended properly
       after that we can go on and start doing other stuff
    */
   /*CASE BATCH*/
   /*For i in commands i*/

}
