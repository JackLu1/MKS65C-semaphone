#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <fcntl.h>

#define SHM_KEY 0xDEADBEEF
#define SEM_KEY 12345

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO */
};

int main(){
    int semid = semget(SEM_KEY, 1, 0);
    if (semid < 0){
        perror("semid");
        printf("create semaphore using make setup arg=-c\n");
        exit(1);
    }
    union semun us;
    printf("waiting for semaphore\n");
    us.val = 1;
    while(!semctl(semid, 0, GETVAL, us));
    //while(!semctl(semid, 0 , SETVAL, us));

    int shmid = shmget(SHM_KEY, 8, 0644);
    if (shmid < 0){
        perror("shmid");
        printf("create shared memory using make setup arg=-c\n");
        exit(1);
    }


    //DOWNING
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_flg = SEM_UNDO;
    sb.sem_op = -1;
    semop(semid, &sb, 1);
    printf("got semaphore\n");

    char * data = shmat(shmid, 0, 0);
    if (*data < 0){
        perror("data");
        exit(1);
    }

    int fd = open("story.txt", O_RDONLY);
    if (fd==-1){
        printf("problem opening the story file\n");
    }
    //IMPORTANT: this gives seg fault, prob bc shared mem has not been created
    int size = -1 * *data;
    lseek(fd, size, SEEK_END);

    char last_line[100];
    int r = read(fd, last_line, 100);
    if (r==-1){
        printf("error %d: %s\n", errno, strerror(errno));
    }

    printf("The last line of this story is:\n%s\n", last_line);


    //get user input
    printf("Add a new line to the story:\n");
    char new_line[100];
    fgets(new_line, 100, stdin);
    int length = strlen(new_line);
    printf("the length of the new line is %d\n", length);

    //write to file
    fd = open("story.txt", O_WRONLY|O_APPEND);
    if (fd < 0){
        printf("problem opening the story file\n");
    }
    int w = write(fd, new_line, length);
    if(w < 0){
        printf("problem writing to story file\n");
    }
    else{
        printf("added new line to the story\n");
    }

    *data = length;
    //UPPING
    sb.sem_op = 1;
    printf("up\n");
    semop(semid, &sb, 1);

}

