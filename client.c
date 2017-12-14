#include "pipe_networking.h"


int main() {

  char buf[BUFFER_SIZE];
  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  while (1) {
    printf("Enter text: ");
    fgets(buf, sizeof(buf), stdin);
    
    if (write(to_server, buf, sizeof(buf)) == -1) {
      printf("Error: %s\n", strerror(errno));
    }
    if (!strcmp(buf, "exit\n")) {
      close(to_server);
      close(from_server);
      exit(0);
    }
    if (read(from_server, buf, sizeof(buf)) == =1) {
      printf("Error: %s\n", strerror(errno));
    }
    printf("Message received from server: %s", buf);
  }
}
