#include "list.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
  CLIENT *client;
  char *input = NULL;
  size_t ipl = 0;
  ssize_t len = 0;
  int sel, *res;
  list *lp;

  if (argc != 2) {
    printf("Syntax: %s <server address>\n", argv[0]);
    return EXIT_FAILURE;
  }

  client = clnt_create(argv[1], LIST, LIST_V1, "tcp");
  if (client == NULL) {
    printf("Failed to connect to server.\n");
    return EXIT_FAILURE;
  }

  do {
    printf("\n\n");
    printf("1 - insert element on server; 2 - delete element on server; 3 - get list from server and print\n");
    printf("4 - drop list on server; 5 - exit\n");
    printf("> ");

    len = getline(&input, &ipl, stdin);
    if (len > 1) sel = atoi(input);
    else sel = -1;

    if ((sel < 1) || (sel > 5)) {
      printf("Invalid selection.\n");
    } else {
      printf("\n\n");
      switch (sel) {
        case 1:
        case 2:
          printf("Enter key: ");
          len = getline(&input, &ipl, stdin);

          if ((len > 0) && (input[len-1] == '\n')) input[len-1] = '\0';

          if (sel == 1) {
            printf("inserting '%s'...", input); fflush(stdout);
            insert_1(&input, client);
            printf("ok.\n");
          } else {
            printf("deleting '%s'...", input); fflush(stdout);
            res = delete_1(&input, client);
            if (*res == 0) printf("ok.\n");
            else printf("key not found.\n");
          }
          break;

        case 3:
          printf("retrieving list..."); fflush(stdout);
          lp = get_1(NULL, client);
          if (lp != NULL) {
            printf("\n");

            lp = lp->next;
            while (lp != NULL) {
              printf("'%s' --> ", lp->id);
              lp = lp->next;
            }
            printf("NULL\n");
          } else {
            printf("failed.\n");
          }
          break;

        case 4:
          printf("dropping list on server..."); fflush(stdout);
          drop_1(NULL, client);
          printf("ok.\n");
          break;

        case 5:
          break;
      }
    }
  } while (sel != 5);

  free(input);

  return EXIT_SUCCESS;
}

