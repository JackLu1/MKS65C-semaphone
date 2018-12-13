#include <stdio.h>
#include <stdlib.h>

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
