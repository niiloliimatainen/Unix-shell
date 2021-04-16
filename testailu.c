#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLEN 1024

typedef struct arg {
    char data[MAXLEN];
    struct arg *next;
} Arg;


void interactive_mode();
void write_error();
void parse_command(char *buffer);
void free_list(Arg *head);


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
    const char delim[1] = " ";
    int length = 0;
     Arg *ptr = NULL, *head = NULL, *tmp = NULL;

    /* Removing newline from the end */ 
    buffer[strcspn(buffer, "\n")] = '\0';

    token = strtok(buffer, delim);

    while (token != NULL) {
        if ((ptr = (Arg*)malloc(sizeof(Arg))) == NULL) {
            write_error();
            exit(0);
        }
        strcpy(ptr->data, token);
        ptr->next = NULL;
        
        length++;
        

        if (head == NULL) {
            head = ptr;
            tmp = ptr;
            
        
        } else {
            tmp->next = ptr;
            tmp = ptr;
            printf("hulhju\n");
        }
        
        token = strtok(NULL, delim);
        printf("fhulhju\n");

    }



    if (strcmp("exit", head->data) == 0) {
        if (length != 0) {
            write_error();
        
        } else {
            free(buffer);
            free_list(head);
            exit(0);
        }
    
    } else if (strcmp("cd", head->data) == 0) {
        if (length > 1) {
            write_error();
        }
        printf("%s\n", head->next->data);
    
    } else if (strcmp("path", head->data) == 0) {
        printf("polkujuttuja\n");

    } else {
        printf("juhuu\n");
    }
    free_list(head);
}


void free_list(Arg *head) {
    Arg *ptr;
    ptr = head;
    while (ptr != NULL) {
        head = ptr->next;
        free(ptr);
        ptr = head;
    }
}












void write_error() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}



void batch_mode() {

}
/*Get user wanted path as param: f.ex: (current path) + /oskukoodaa/niilokoodaa*/
void cd() {
    
    if (chdir("/PATH THING VARIABLE") != 0){
        printf("Liukastuit banaaniin. Yritäs uudelleen!\n");
        perror("cd failed blaablaa");
    }


}


void path() {

}


void redirect() {

}


