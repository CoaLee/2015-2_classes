#include "list.h"
#include <string.h>
#include <stdio.h>

static list dummy = { "dummy", NULL };
static list *lp = NULL;

void dump(void)
{
  list *cur = lp;

  printf("list:\n");
  while (cur != NULL) {
    printf("'%s' --> ", cur->id);
    cur = cur->next;
  }
  printf("NULL\n");
}

void* insert_1_svc(char **c, struct svc_req *req)
{
  static void *null = NULL;
  list *cur, *prev, *elem;

  printf("insert('%s')\n", *c);

  elem = (list*)calloc(1, sizeof(list));
  elem->id = strdup(*c);

  prev = NULL;
  cur = lp;

  while ((cur != NULL) && (strcmp(cur->id, elem->id) < 0)) {
    prev = cur;
    cur = cur->next;
  }

  if (prev == NULL) {
    elem->next = lp;
    lp = elem;
  } else {
    prev->next = elem;
    elem->next = cur;
  }

  dump();

  return &null;
}

int* delete_1_svc(char **c, struct svc_req *req)
{
  static int res;
  list *cur, *prev;

  printf("delete('%s')\n", *c);

  prev = NULL;
  cur = lp;

  while ((cur != NULL) && (strcmp(cur->id, *c) < 0)) {
    prev = cur;
    cur = cur->next;
  }

  if ((cur != NULL) && (strcmp(cur->id, *c) == 0)) {
    if (prev == NULL) lp = cur->next;
    else prev->next = cur->next;
    free(cur->id);
    free(cur);
    res = 0;
  } else {
    res = -1;
  }

  dump();

  return &res;
}

list* get_1_svc(void *v, struct svc_req *req)
{
  printf("get()\n");

  dummy.next = lp;

  return &dummy;
}

void* drop_1_svc(void *v, struct svc_req *req)
{
  static void *null = NULL;
  list *cur = lp;

  printf("drop()\n");

  while (cur != NULL) {
    lp = cur->next;
    free(cur->id);
    free(cur);
    cur = lp;
  }

  return &null;
}
