#include "pipe_networking.h"
//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  mkfifo(WKP, 0666);
  if(open(WKP, O_RDONLY) == -1){
    printf("Opening named pipe does not work.\n");
    exit(1);
  }
  // char line_buff[256];
  int from_client = open(WKP, O_RDONLY, 0666);
  printf("Named pipe open.\n");
  // read( namedPipe, line_buff, 255 );

  // int from_client = 0;
  remove(WKP);
  printf("Removed name pipe\n");
  return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  printf("SERVER HANDSHAKE HERE\n");
  int from_client = server_setup();
  *to_client = server_connect(from_client);
  int randNum = getRand();
  write(*to_client, &randNum, sizeof(int));
  printf("Sent random number %d to client\n", randNum);

  int getBack;
  read(from_client, &getBack, sizeof(int));
  printf("Client sent back %d\n", getBack);

  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char myName[256];
  sprintf(myName, "%d", getpid());
  mkfifo(myName, 0666);
  printf("Client makes pipe with pid name.\n");

  printf("PRE opeing WKP\n");
  *to_server = open(WKP, O_WRONLY, 0666);
  printf("POST opening WKP\n");
  printf("WKP connection formed\n");
  write(*to_server, myName, strlen(myName));

  int from_server = open(myName, O_RDONLY, 0666);
  int message;
  read(from_server, &message, sizeof(int));
  printf("Read the number %d from parent.\n", message);

  remove(myName);
  message++;
  write(*to_server, &message, sizeof(int));
  printf("Sent back %d to server.\n", message);

  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client  = 0;
  char readName[BUFFER_SIZE];
  read(from_client, readName, sizeof(readName));
  printf("Server got \"%s\" as the fifo name from client\n", readName);
  to_client = open(readName, O_WRONLY, 0666);
  return to_client;
}

int getRand(){
  int x = open("dev/urandom", O_RDONLY);
  int rand;
  read(x, &rand, sizeof(int));
  return rand;
}
