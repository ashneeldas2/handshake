#include "pipe_networking.h"
#include <signal.h>


void to_upper_case(char * str) {
  int i = 0; 
  while (str[i]) {
    str[i] = toupper(str[i]);
    i++;
  }
}

static void sighandler(int signo)
{
  if (signo == SIGINT) { // when we want the server to exit
    /*
     * This will most likely be at the time when WKP has been created and the
     * server is waiting for the client.
     */
    remove("WKP");
    exit(EXIT_SUCCESS);
  }
}

int main() {

  char buf[BUFFER_SIZE];
  int to_client; // descriptor for downstream pipe
  int from_client; // descriptor for upstream pipe
  // int errtest;

  signal(SIGINT, sighandler);

  while (1) { // server runs continuously
    from_client = server_handshake( &to_client ); // three-way handshake
    
    while (read(from_client, buf, sizeof(buf))) {
      /*
      if (read(from_client, buf, sizeof(buf)) == -1) {
	printf("Error: %s\n", strerror(errno));
      }
      */
    
      printf("Text received from client: %s", buf);
    
      /*
      if (!strcmp(buf, "exit\n")) {
	close(to_client);
	close(from_client);
	exit(0);
      }
      */
    
      to_upper_case(buf); // modify text
    
      if (write(to_client, buf, sizeof(buf)) == -1) {
	printf("Error: %s\n", strerror(errno));
      }
    }
  }

  return 0;
}
