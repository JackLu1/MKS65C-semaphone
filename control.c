#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>

#define KEY 0xDEADBEEF

void create_shm();
void remove_shm();
void view_shm();

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO */
};

int main(int argc, char ** argv){
    char * cmd = malloc(12);
    if (argc < 2){
        printf("include -c -r -v flag in cmd line using arg=flag\n");
        exit(0);
    } else if (argc == 2){
        cmd = argv[1];
    } else {
        printf("too many args\n");
        exit(0);
    }
    //printf("flag = %s\n", cmd);
    if (strcmp(cmd, "-c") == 0) {
        printf("create\n");
        create_shm();
    } else if (strcmp(cmd, "-r") == 0) {
        printf("remove\n");
        remove_shm();
    } else if (strcmp(cmd, "-v") == 0){
        printf("view\n");
        view_shm();
    } else {
        printf("invalid command\n");
        exit(0);
    }

    return 0;
}

void create_shm(){
}
