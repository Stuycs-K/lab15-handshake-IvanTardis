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
  
}
