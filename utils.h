#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h
#include <string.h>
#include <unistd.h> 
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
