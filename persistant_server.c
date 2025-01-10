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
    int randNum = abs(getRand()) % 101;
    while(write(to_client, &randNum, sizeof(int)) != -1){
      randNum = abs(getRand()) % 101;
      printf("Wrote to client\n");
      sleep(1);
    }
    close(from_client);
    close(to_client);
  }
}


static void sighandler(int signo){
  if ( signo == SIGINT ){
    printf("Exiting due to SIGINT\n");
    exit(0);
  }
  if ( signo == SIGPIPE ){
    printf("Exiting due to SIGPIPE\n");
    close(from_client);
    close(to_client);
    exit(0);
  }
}
