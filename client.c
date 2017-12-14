#include "pipe_networking.h"


int main() {

  char buf[BUFFER_SIZE];
  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  while (1) {
    printf("Enter text: ");
    fgets(buf, sizeof(buf), stdin);
    write(to_server, buf, sizeof(buf));
    if (!strcmp(buf, "exit\n")) {
      close(to_server);
      close(from_server);
      exit(0);
    }
    read(from_server, buf, sizeof(buf));
    printf("Message received from server: %s", buf);
  }
}
