#include "date.h"
#include <stdlib.h>
#include <time.h>

#define BUFSIZE 64

int main(int argc, char *argv[])
{
  CLIENT *client;
  time_t *t;
  struct tm ts;
  char tstr[BUFSIZE];

  if (argc != 2) {
    printf("Syntax: %s <server address>\n", argv[0]);
    return EXIT_FAILURE;
  }

  client = clnt_create(argv[1], DATE, DATE_V1, "tcp");
  if (client == NULL) {
    printf("Failed to connect to server.\n");
    return EXIT_FAILURE;
  }

  t = (time_t*)getdate_1(NULL, client);
  if (t == NULL) {
    printf("RPC call failed.\n");
    return EXIT_FAILURE;
  }


  gmtime_r(t, &ts);
  strftime(tstr, BUFSIZE, "%A, %B %d, %Y %H:%M:%S", &ts);
  printf("Date and time on server: %s.\n", tstr);

  return EXIT_SUCCESS;
}
