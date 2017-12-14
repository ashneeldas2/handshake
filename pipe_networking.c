#include "pipe_networking.h"

/*=========================
  server_handshake
  args: int * to_client

  Perofrms the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  char buf[HANDSHAKE_BUFFER_SIZE];
  int pd; // pipe descriptor
  
  // printf("TEST\n");
  if (mkfifo("WKP", 0644) == -1) { // Well Known Pipe
    printf("Error: %s\n", strerror(errno));
  }

  printf("Waiting for client...\n");
  pd = open("WKP", O_RDONLY);
  // printf("TEST\n");
  if (pd == -1) {
    printf("Error: %s\n", strerror(errno));
  }

  if (read(pd, buf, sizeof(buf)) == -1) {
    printf("Error: %s\n", strerror(errno));
  }
  printf("pipe name received: %s\n", buf);

  // Open the pipe whose name the server received from the client:
  *to_client = open(buf, O_WRONLY);
  if (*to_client == -1) {
    printf("Error: %s\n", strerror(errno));
  }
  
  remove("WKP");
  
  if (write(*to_client, ACK, strlen(ACK)) == -1) {
    printf("Error: %s\n", strerror(errno));
  }
  
  if (read(pd, buf, sizeof(buf)) == -1) {
    printf("Error: %s\n", strerror(errno));
  }
  
  if (strncmp(buf, ACK, sizeof(buf)) == 0) {
    printf("Confirmation message received: \"%s\"\n", buf);
  } else {
    printf("Error: received message \"%s\" instead of confirmation message \"%s\".\n", buf, ACK);
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
  char buf[HANDSHAKE_BUFFER_SIZE]; // buffer to read from FIFO
  int pd;
  
  //printf("TEST\n");
  if (mkfifo("PRIVATE", 0644) == -1) {
    printf("Error: %s\n", strerror(errno));
  }
  
  *to_server = open("WKP", O_WRONLY);
  if (*to_server == -1) {
    printf("Error: %s\n", strerror(errno));
  }
  
  if (write(*to_server, "PRIVATE", strlen("PRIVATE")) == -1) {
    printf("Error: %s\n", strerror(errno));
  }
  
  pd = open("PRIVATE", O_RDONLY);
  if (pd == -1) {
    printf("Error: %s\n", strerror(errno));
  }
  
  if (read(pd, buf, sizeof(buf)) == -1) {
    printf("Error: %s\n", strerror(errno));
  }
  
  // printf("Message received: %s\n", buf);
  remove("PRIVATE");
  
  if (strncmp(buf, ACK, sizeof(buf)) == 0) {
    printf("Confirmation message received: \"%s\"\n", buf);
  } else {
    printf("Error: received message \"%s\" instead of confirmation message \"%s\".\n", buf, ACK);
  }
  if (write(*to_server, ACK, sizeof(ACK)) == -1) {
    printf("Error: %s\n", strerror(errno));
  }
  return pd;
}
