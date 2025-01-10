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

int to_server;
int from_server;

int main() {
  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);
  from_server = client_handshake(&to_server);

  while(1){
    int reader;
    read(from_server, &reader, sizeof(int));
    printf("Client reads random number %d from server.", reader);
  }
}

static void sighandler(int signo){
  if ( signo == SIGINT ){
    printf("Exiting due to SIGINT\n");
    close(from_server);
    close(to_server);
    exit(0);
  }
  if ( signo == SIGPIPE ){
    printf("Exiting due to SIGPIPE\n");
    close(from_server);
    close(to_server);
    exit(0);
  }
}
