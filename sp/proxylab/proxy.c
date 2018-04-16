/*
 * proxy.c - M1522.000800 System Programming: Web proxy
 *
 * Student ID: 2010-13419
 *       Name: Lee Yongjae
 * 
 * IMPORTANT: Give a high level description of your code here. You
 * must also provide a header comment at the beginning of each
 * function that describes what that function does.
 */ 

#include "csapp.h"
#include "echo.h"
#include <time.h>
#include <stdio.h>

/* The name of the proxy's log file */
#define PROXY_LOG "proxy.log"

/* Undefine this if you don't want debugging output */
#define DEBUG 

/*
 * Functions to define
 */
void *process_request(void* vargp);
int open_clientfd_ts(char *hostname, int port, sem_t *mutexp);
ssize_t Rio_readn_w(int fd, void *ptr, size_t nbytes);
ssize_t Rio_readlineb_w(rio_t *rp, void *usrbuf, size_t maxlen);
void Rio_writen_w(int fd, void *usrbuf, size_t n);
//Used for split message from client into parts
char *my_strsplit(const char *str, char delim, int count);

struct connStruct {
	struct hostent *client_hp;
  int client_connfd;
	int client_port;
	char *client_haddrp;
};

/* GLOBAL variables. FILE pointer for log file and
 * mutexes for log and lock-and-copy of gethostbyname */
FILE *logfile;
sem_t logMutex, wrapMutex;

/*
 * main - Main routine for the proxy program
 */
int main(int argc, char **argv)
{
	int port, listenfd;
	struct sockaddr_in clientaddr;
	int clientlen=sizeof(clientaddr);
	pthread_t tid;

  /* Check arguments */
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
    exit(0);
  }
	port = atoi(argv[1]);

	listenfd = Open_listenfd(port);

  //open the logfile
	logfile = fopen(PROXY_LOG, "a");
	sem_init(&logMutex, 0, 1);
	sem_init(&wrapMutex, 0, 1);

	while(1) {
		struct connStruct *oneConn = malloc(sizeof(struct connStruct));

	  oneConn->client_connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		oneConn->client_hp = Gethostbyaddr((const char*)&clientaddr.sin_addr.s_addr,
		                                    sizeof(clientaddr.sin_addr.s_addr), AF_INET);
		oneConn->client_haddrp = inet_ntoa(clientaddr.sin_addr);
		oneConn->client_port = ntohs(clientaddr.sin_port);

    Pthread_create(&tid, NULL, process_request, oneConn);
	}

  //close the logfile  
	fclose(logfile);

  exit(0);
}

void *process_request(void* vargp){
	int proxyfd;
	rio_t rioAsServer, rioAsClient;
  char bufWithClient[MAXLINE], bufWithServer[MAXLINE];
	char *hostp, *portp, *msgp;
	int port;
	int byteFromClient, byteFromServer;
	time_t rawtime;
	struct tm* my_time;
	char dateString[30];

	struct connStruct oneConn = *((struct connStruct *)vargp);
	Pthread_detach(pthread_self());
	Free(vargp);
  
  //READ from client
  Rio_readinitb(&rioAsServer, oneConn.client_connfd);
	while((byteFromClient=Rio_readlineb_w(&rioAsServer, bufWithClient, MAXLINE))!=0){
		//split the bufWithClient into 3 parts: host, port, message.
		hostp = my_strsplit(bufWithClient, ' ', 0);
		portp = my_strsplit(bufWithClient, ' ', 1);
		port = atoi(portp);
		msgp = my_strsplit(bufWithClient, ' ', 1);

		//REQUEST to server
		proxyfd = open_clientfd_ts(hostp, port, &wrapMutex);
		Rio_readinitb(&rioAsClient, proxyfd);
		Rio_writen_w(proxyfd, msgp, strlen(msgp));
		byteFromServer = Rio_readlineb_w(&rioAsClient, bufWithServer, MAXLINE);
		Close(proxyfd);
	
		//LOGGING to file
		P(&logMutex);
		rawtime=time(NULL);
		my_time = localtime(&rawtime);
		if(strftime(dateString, sizeof dateString, "%a %e %b %Y %T %Z", my_time)){
			fprintf(logfile, "%s: %s %d %d %s", 
											 dateString, hostp, oneConn.client_port, byteFromServer, bufWithServer); 
		} else {
			fprintf(logfile, "%s: %s %d %d %s", 
											 "Date calculation failed.", hostp, oneConn.client_port, byteFromServer, bufWithServer); 
		}
		fflush(logfile);
		V(&logMutex);

		//SENDING to client
		Rio_writen_w(oneConn.client_connfd, bufWithServer, byteFromServer);
	}
  Close(oneConn.client_connfd);

	return NULL;
}

/* The orginal open_clientfd function in csapp.c is thread-unsafe because it calls gethostbyaddr, 
a Class-3 thread unsafe function (textbook 12.7.1). This is thread-safe version of it using
the lock-and-copy technique (textbook 12.7.1) when it calls gethostbyname.  */
int open_clientfd_ts(char *hostname, int port, sem_t* mutexp) 
{
  int clientfd;
  struct hostent *hp;
  struct sockaddr_in serveraddr;

  if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	return -1; /* check errno for cause of error */
  
  bzero((char *)&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;

	//lock-and-copy technique for thread-safety
  P(mutexp);
  if ((hp = gethostbyname(hostname)) == NULL)
	  return -2; 
  bcopy((char *)hp->h_addr_list[0], 
	      (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
	V(mutexp);

  serveraddr.sin_port = htons(port);

  /* Establish a connection with the server */
  if (connect(clientfd, (SA *) &serveraddr, sizeof(serveraddr)) < 0)
	  return -1;

  return clientfd;
}

/* The Rio readn, Rio readlineb, and Rio writen error checking wrappers in csapp.c
are not appropriate for a realistic proxy because they terminate the process when they
encounter an error. Instead, these wrappers simply returns after printing a warning message
when I/O fails. When the wrappers detects an error, it returns 0, as though it
encountered EOF on the socket. */
void Rio_writen_w(int fd, void *usrbuf, size_t n) 
{
  if (rio_writen(fd, usrbuf, n) != n)
		perror("Rio_writen error");
}

ssize_t Rio_readn_w(int fd, void *ptr, size_t nbytes) 
{
  ssize_t n;
  
  if ((n = rio_readn(fd, ptr, nbytes)) < 0){
		perror("Rio_readn error");
		return 0;
	}

  return n;
}

ssize_t Rio_readlineb_w(rio_t *rp, void *usrbuf, size_t maxlen) 
{
  ssize_t rc;

  if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0) {
		perror("Rio_readlineb error");
		return 0;
	}

  return rc;
} 

/* it's useful to split a message from client into parts */
char *my_strsplit(const char *str, char delim, int count){
  int i=0;
	char *arr=(char *)str;
  
	while(count>0){
	  if(arr[i++]==delim) count--;
	}
  
	if(i>0) arr[i-1]='\0';

	return &arr[i];
}

