#include "pipe_networking.h"

int main() {
  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);

  int to_client;
  int from_client;

  printf("Beginning random number generation\n");
  while(1){
    from_client = server_handshake( &to_client );
    printf("Connected with a client!\n");
    while(1){
      int randNum = abs(getRand()) % 101;
      write(to_client, &randNum, sizeof(int));
      printf("Wrote to client\n");
      sleep(1);
    }
  }
}


static void sighandler(int signo){
  if ( signo == SIGINT ){
    printf("Exiting due to SIGINT\n");
    exit(0);
  }
  if ( signo == SIGPIPE ){
    printf("Exiting due to SIGPIPE\n");
    exit(0);
  }
}
