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

#define KEY 0xDEADBEEF
#define SHMID 12345

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO */
};


int main() {
  int sem_id = semget(KEY, 1, 0);
  union semun us;
  printf("Please wait while others finish their edits.\n");
  while(!semctl(sem_id, 0, GETVAL, us));
  int * data = (int *)shmat(SHMID, (void *)0, 0);

  //DOWNING
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  semop(sem_id, &sb, 1);

  printf("got the semaphore!\n");
  sleep(1);

  int fd = open("story.txt", O_RDONLY);
  if (fd==-1){
    printf("problem opening the story file\n");
  }
  
  //IMPORTANT: this gives seg fault, prob bc shared mem has not been created
  lseek(fd, *data, SEEK_END);
  
  char last_line[100];
  int r = read(fd, last_line, 100);
  if (r==-1){
    printf("error %d: %s\n", errno, strerror(errno));
  }

  printf("The last line of this story is:\n%s\n", last_line);

  printf("Add a new line to the story:\n");
  char new_line[100];
  fgets(new_line, 100, stdin);
  int length = strlen(new_line);
  printf("the length of the new line is %d\n", length);

  fd = open("story.txt", O_WRONLY|O_APPEND);
  if (fd==-1){
    printf("problem opening the story file\n");
  }
  int w = write(fd, new_line, length);
  if(w==-1){
    printf("problem writing to story file\n");
  }
  else{
    printf("added new line to the story\n");
  }

  *data = length;

  //UPPING
  sb.sem_op = 1;
  semop(sem_id, &sb, 1);

  return 0;
}
