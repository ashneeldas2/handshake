#include "pipe_networking.h"


int main() {

  char buf[BUFFER_SIZE];
  int to_server; // descriptor for upstream pipe
  int from_server; // descriptor for downstream pipe

  from_server = client_handshake( &to_server ); // three-way handshake

  while (1) { // client runs until user inputs "exit\n"
    printf("Enter text: ");
    fgets(buf, sizeof(buf), stdin);
    
    if (!strcmp(buf, "exit\n")) {
      close(to_server);
      close(from_server);
      exit(EXIT_SUCCESS);
    }
    
    if (write(to_server, buf, sizeof(buf)) == -1) {
      printf("Error: %s\n", strerror(errno));
    }
    
    if (read(from_server, buf, sizeof(buf)) == -1) {
      printf("Error: %s\n", strerror(errno));
    }
    printf("Message received from server: %s", buf);
  }

  return 0;
}
