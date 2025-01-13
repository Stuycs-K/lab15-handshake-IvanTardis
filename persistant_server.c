#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include "pipe_networking.h"

int to_client;
int from_client;

int main() {
  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);

  printf("Beginning random number generation\n");
  while(1){
    from_client = server_handshake( &to_client );
    printf("Connected with a client!\n");
    //int x = open("/dev/random", O_RDONLY, 0300);
    //int rand;
    //read(x, &rand, sizeof(int));
    int randNum = abs(getRand()) % 101;
    while(1){
      printf("Looping again, so this should be right before a new random number.\n");
      randNum = abs(getRand()) % 101;
      //read(x, &rand, sizeof(int));
      printf("Right after random number, and it is... %d\n", randNum);
      if(write(to_client, &randNum, sizeof(int)) == -1){
        printf("Error writing to client\n");
        close(to_client);
        break;
      }
      printf("Wrote %d to client\n", randNum);
      sleep(1);
    }
  }
}


static void sighandler(int signo){
  if ( signo == SIGINT ){
    printf("Exiting due to SIGINT\n");
    close(from_client);
    close(to_client);
    exit(0);
  }
  if ( signo == SIGPIPE ){
    printf("Caught a SIGPIPE so a client disconnected\n");
    close(from_client);
    close(to_client);
  }
}
