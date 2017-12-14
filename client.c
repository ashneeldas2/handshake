#include "pipe_networking.h"


int main() {

  char buf[BUFFER_SIZE];
  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  while (1) {
    printf("Enter text: ");
    fgets(buf, sizeof(buf), stdin);
    if (!strcmp(buf, "exit\n")) {
      exit(0);
    }
    write(to_server, buf, sizeof(buf));
    read(from_server, buf, sizeof(buf));
    printf("Message received from server: %s", buf);
  }
}
