#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Perofrms the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  char buf[300];
  printf("TEST\n");
  if (mkfifo("WKP", 0644) == -1) {
    printf("Error: %s\n", strerror(errno));
  }
  int pd = open("WKP", O_RDONLY);
  printf("TEST\n");
  if (pd == -1) {
    printf("Error: %s\n", strerror(errno));
  }
  if (read(pd, buf, sizeof(buf)) == -1) {
    printf("Error: %s\n", strerror(errno));
  }
  printf("pipe name received: %s\n", buf);
  *to_client = open(buf, O_WRONLY);
  remove("WKP");
  if (write(*to_client, ACK, strlen(ACK)) == -1) {
    printf("Error: %s\n", strerror(errno));
  }
  return pd;
}


/*=========================
  client_handshake
  args: int * to_server

  Perofrms the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {  
  char buf[300];
  //printf("TEST\n");
  if (mkfifo("PRIVATE", 0644) == -1) {
    printf("Error: %s\n", strerror(errno));
  }
  int pd = open("PRIVATE", O_RDONLY);
  if (pd == -1) {
    printf("Error: %s\n", strerror(errno));
  }
  *to_server = open("WKP", O_WRONLY);
  if (write(*to_server, "PRIVATE", strlen("PRIVATE")) == -1) {
    printf("Error: %s\n", strerror(errno));
  }
  read(pd, buf, sizeof(buf));
  printf("Message received: %s\n", buf);
  return pd;
}
