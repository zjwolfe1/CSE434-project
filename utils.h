#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi(), exit(), and rand() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <time.h>       /* for time() */
#include <errno.h>

#ifndef HEADER_FILE
#define HEADER_FILE

#define REQUEST_MAX 255

struct request {
  char client_ip[16];
  char m[24];
  int c;
  int r;
  int i;
  char operation[80];
};

void printRequest(struct request* req);
void printError(const char *errorMessage, int die);
struct request* Request(char* client_ip, char* m, char* c, char* r, char* i, char* operation);

#endif
