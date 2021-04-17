#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLEN 64


void interactive_mode();
void write_error();
void parse_command(char *buffer);
void shell_execute(char **args, int size);
void cd(char *buffer);
void wish_exit(char *token, char *buffer);


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
    free(buffer);
}


void parse_command(char *buffer) {
    char *token, **args, delim[1] = " ";
    int index = 0, size, maxlen = MAXLEN;

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

        while (1) {
            args[index] = token;
            index++;

            if (index >= MAXLEN) {
                maxlen += MAXLEN;
                if ((args = realloc(args, maxlen * sizeof(char*))) == NULL) {
                    write_error();
                    exit(0);
                }
            }

            if (((token = strtok(NULL, delim)) == NULL) || (strcmp(token, "&") == 0)) {
                break;
            }
        }
        
        if (token == NULL) {
            break;
        
        } else if (strcmp(token, "&")) {
            token = strtok(NULL, delim);
        }




     /*   } else if (strcmp("cd", token) == 0) {
            cd(token);
        
        } else if (strcmp("path", token) == 0) {
            printf("polkujuttuja\n");

        } else {
            printf("juhuu\n");
        }
             shell_execute(token);

        */
    }

    shell_execute(args, index);
    free(args);
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

void shell_execute(char **args, int size) {
    /*NOTE: Now expects only one command: args[] = command, arg1, arg2...*/
    int counter = 0;
    for (int i = 0; i < size; i++) {
        if (strcmp("&", args[i]) == 0){
            counter++;
        }
    }
    printf("counter:%d\n", counter);
    /*Somewhere here should be a loop to start multiple processes at once*/
    /*Need to figure out if these commands are builtins and should be ran at the main process and not in child*/
    
    /*process part starts*/
    pid_t pid, wpid;

    pid = fork();
    /*Fork returns pid of child to parent and pid of 0 to child*/
    /*So child i pid=0 and parent is something else*/
    if (pid == 0){
        /*Launch program in child process*/
        /*execvp(prgrm name, arguments vector)*/
        if (execvp(args[0],args) == -1){
            fprintf("Something happened when launching program %s", args[0]);
            perror("Shell-execute->execvp");
        }

    }else if (pid < 0){
        /*Fork has failed if process id is less than */
        perror("Shell-execute->ParentFork");
    }else{
        /*Now we are in parent process*/
        int ret_stat;
        waitpid(pid, &ret_stat, 0);
        if(ret_stat == 0){
            printf("child terminated succesfully\n");
        }
        else{
            printf("Child terminated with error");
            /*Further checks to be added here*/
        }
        
    /*process part ends and function can return to receive new calls*/






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

   
    printf("\n");
    return;

}


void wish_exit(char *token, char *buffer) {
 
}