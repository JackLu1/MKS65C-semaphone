#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>

#define KEY 0xDEADBEEF


int main(int argc, char ** argv){
    char * cmd = malloc(12);
    printf("%i\n", argc);
    if (argc < 2){
        printf("include -c -r -v flag in cmd line using arg=flag\n");
        exit(0);
    } else if (argc == 2){
        cmd = argv[1];
    } else {
        printf("too many args\n");
        exit(0);
    }

    printf("flag = %s\n", cmd);

    return 0;
}
