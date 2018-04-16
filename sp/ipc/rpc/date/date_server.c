#include "date.h"
#include  <time.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

long* getdate_1_svc(void *v, struct svc_req *r)
{
  static time_t t;

  char *client_ip = inet_ntoa(svc_getcaller(r->rq_xprt)->sin_addr);
  printf("RPC server: getdate() called from %s.\n", client_ip);

  t = time(NULL);

  return (long*)&t;
}
