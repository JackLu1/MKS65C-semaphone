#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>

#define SHM_KEY 0xDEADBEEF
#define SEM_KEY 12345

void create();
void delete();
void view();

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
        create();
    } else if (strcmp(cmd, "-r") == 0) {
        delete();
    } else if (strcmp(cmd, "-v") == 0){
        view();
    } else {
        printf("invalid command\n");
        exit(0);
    }

    return 0;
}

void create(){
    //shared memory
    int shmid = shmget(SHM_KEY, 32, 0644 | IPC_CREAT | IPC_EXCL);
    if (shmid < 0){
        perror("shmid");
    }

    //file
    int f = open("story.txt", O_CREAT | O_TRUNC | O_EXCL | O_RDWR);
    if (f < 0){
        perror("file");
    }

    //semaphore
    int semid = semget(SEM_KEY, 1, IPC_EXCL | IPC_CREAT | 0644);
    if (semid < 0){
        perror("semid");
    }
    union semun s;
    s.val = 1;
    semctl(semid, 0, SETVAL, s);

}

void delete(){
    //shared memory
    int shmid = shmget(SHM_KEY, 32, 0644 );
    if (shmid < 0){
        perror("error");
    }
    shmid = shmctl(shmid, IPC_RMID, NULL);
    if (shmid < 0){
        perror("error");
    }

    //semaphore
    int semid = semget(SEM_KEY, 1, 0644);
    if (semid < 0){
        perror("semid");
    }

    struct sembuf s;
    //down
    s.sem_op = -1;
    s.sem_num = 0;
    s.sem_flg = SEM_UNDO;
    printf("waiting for semaphore\n");
    semop(semid, &s, 1);
    printf("got semaphore\n");
    semctl(semid, 0, IPC_RMID, NULL);

    //file
    int f = open("story.txt", O_RDONLY);
    view();
    if (remove("story.txt") < 0){
        perror("remove");
    }
}

void view(){
    int f = open("story.txt", O_RDONLY);
    if (f < 0){
        perror("file");
    }
    int size = lseek(f, 0 , SEEK_END);
    char * content = malloc(size + 1);
    lseek(f, 0, SEEK_SET);
    size = read(f, content, size);

    printf("--------------------STORY---------------\n");
    printf("%s", content);
    printf("----------------------------------------\n");
    
}
