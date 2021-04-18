#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


FILE * open(char *name);

int main(int argc, char *argv[]) {
    FILE *file = NULL;
    file = open(argv[1]);
   

    printf("se toimi!\n");
    fclose(file);
    return 0;
}


FILE * open(char *name) {
    FILE *file;

    if ((file = freopen(name, "w", stdout)) == NULL) {
        printf("virhe");
    }
    return file;
}