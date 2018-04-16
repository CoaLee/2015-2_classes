#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "pipe_info.h"

int main(int argc, char *argv[])
{
  int pipefd[2];
  pid_t pid;
  ssize_t len = 0;
  size_t buflen = 0;
  char *buf = NULL;
  pipe_msg msg;

  if (pipe(pipefd) < 0) {
    printf("Cannot create pipe.\n");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if (pid < 0) {
    printf("Cannot fork.\n");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    //
    // child: consume messages sent through pipe
    //
    close(pipefd[1]);

    pid = getpid();
    printf("Hi, this is the consumer (pid = %d): waiting for messages...(Ctrl-C to abort)\n", pid);

    while (1) {
      len = read(pipefd[0], &msg, sizeof(msg));
      if (len != sizeof(msg)) {
        perror(NULL);
        //printf("Did not receive a complete message (%zd), aborting.\n", len);
        break;
      }
      printf("message: '%s'\n", msg.message);
      if (strcmp("client::quit", msg.message) == 0) break;
    }
    close(pipefd[0]);

    printf("Consumer exits.\n");

  } else {
    //
    // parent: write messages to pipe
    //

    close(pipefd[0]);

    pid = getpid();
    printf("Hi, this is the producer (pid = %d): enter messages or <Enter> to quit.\n", pid);
    while (1) {
      printf("> ");
      len = getline(&buf, &buflen, stdin);
      if ((len > 0) && (buf[len-1] == '\n')) buf[len-1] = '\0';

      if (buf[0] == '\0') break;

      strncpy(msg.message, buf, MSG_LENGTH);
      len = write(pipefd[1], &msg, sizeof(msg));
      if (len != sizeof(msg)) {
        printf("Error writing to pipe.\n");
        exit(EXIT_FAILURE);
      }
    }

    free(buf);
    close(pipefd[1]);
  }

  return EXIT_SUCCESS;
}
