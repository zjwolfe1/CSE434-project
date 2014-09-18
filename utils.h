#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#ifndef UTILS_H
#define UTILS_H

#define REQUEST_MAX 255

struct request {
  char client_ip[16];
  char m[24];
  int c;
  int r;
  int i;
  char operation[80];
};

struct response {
  int status;
  int r;
  char body[80];
};

void printRequest(struct request* req);
void printResponse(struct response* res);
void printError(const char *errorMessage, int die);
struct request* Request(char* client_ip, char* m, char* c, char* r, char* i, char* operation);

#endif
