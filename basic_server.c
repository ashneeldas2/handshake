#include "pipe_networking.h"


void to_upper_case(char * str) {
  int i = 0; 
  while (str[i]) {
    str[i] = toupper(str[i]);
    i++;
  }
}

int main() {

  char buf[BUFFER_SIZE];
  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );

  while (1) {
    if (read(from_client, buf, sizeof(buf)) == -1) {
      printf("Error: %s\n", strerror(errno));
    }
    printf("Text received from client: %s", buf);
    if (!strcmp(buf, "exit\n")) {
      close(to_client);
      close(from_client);
      exit(0);
    }
    to_upper_case(buf);
    if (write(to_client, buf, sizeof(buf)) == -1) {
      printf("Error: %s\n", strerror(errno));
    }
  }
}
