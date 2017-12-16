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
    exit(EXIT_FAILURE);
  }

  printf("Waiting for client...\n");
  pd = open("WKP", O_RDONLY);
  // printf("TEST\n");
  if (pd == -1) {
    printf("Error: %s\n", strerror(errno));
    remove("WKP");
    exit(EXIT_FAILURE);
  }

  if (read(pd, buf, sizeof(buf)) == -1) {
    printf("Error: %s\n", strerror(errno));
    remove("WKP");
    close(pd);
    exit(EXIT_FAILURE);
  }
  printf("pipe name received: %s\n", buf);

  // Open the pipe whose name the server received from the client:
  *to_client = open(buf, O_WRONLY);
  if (*to_client == -1) {
    printf("Error: %s\n", strerror(errno));
    remove("WKP");
    close(pd);
    exit(EXIT_FAILURE);
  }
  
  remove("WKP");
  
  if (write(*to_client, ACK, sizeof(ACK)) == -1) {
    printf("Error: %s\n", strerror(errno));
    close(pd);
    close(*to_client);
    exit(EXIT_FAILURE);
  }
  
  if (read(pd, buf, sizeof(buf)) == -1) {
    printf("Error: %s\n", strerror(errno));
    close(pd);
    close(*to_client);
    exit(EXIT_FAILURE);
  }
  
  if (strncmp(buf, ACK, sizeof(buf)) == 0) {
    printf("Confirmation message received: \"%s\"\n", buf);
  } else {
    printf("Error: received message \"%s\" instead of confirmation message \"%s\".\n", buf, ACK);
    close(pd);
    close(*to_client);
    exit(EXIT_FAILURE);
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
    exit(EXIT_FAILURE);
  }
  
  *to_server = open("WKP", O_WRONLY);
  if (*to_server == -1) {
    printf("Error: %s\n", strerror(errno));
    remove("PRIVATE");
    exit(EXIT_FAILURE);
  }
  
  if (write(*to_server, "PRIVATE", sizeof("PRIVATE")) == -1) {
    printf("Error: %s\n", strerror(errno));
    remove("PRIVATE");
    close(*to_server);
    exit(EXIT_FAILURE);
  }
  
  pd = open("PRIVATE", O_RDONLY);
  if (pd == -1) {
    printf("Error: %s\n", strerror(errno));
    remove("PRIVATE");
    close(*to_server);
    exit(EXIT_FAILURE);
  }
  
  if (read(pd, buf, sizeof(buf)) == -1) {
    printf("Error: %s\n", strerror(errno));
    remove("PRIVATE");
    close(*to_server);
    close(pd);
    exit(EXIT_FAILURE);
  }
  
  // printf("Message received: %s\n", buf);
  remove("PRIVATE");
  
  if (strncmp(buf, ACK, sizeof(buf)) == 0) {
    printf("Confirmation message received: \"%s\"\n", buf);
  } else {
    printf("Error: received message \"%s\" instead of confirmation message \"%s\".\n", buf, ACK);
    close(*to_server);
    close(pd);
    exit(EXIT_FAILURE);
    
  }
  if (write(*to_server, ACK, sizeof(ACK)) == -1) {
    printf("Error: %s\n", strerror(errno));
    close(*to_server);
    close(pd);
    exit(EXIT_FAILURE);
  }
  return pd;
}
