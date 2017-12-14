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
    read(from_client, buf, sizeof(buf));
    printf("Text received from client: %s", buf);
    to_upper_case(buf);
    write(to_client, buf, sizeof(buf));
  }
}
