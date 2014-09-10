#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

struct request* Request(char* client_ip, char* m, char* c, char* r, char* i, char* operation) {
  struct request* req = malloc(sizeof *req);
  strcpy(req->client_ip, client_ip);
  strcpy(req->m, m);
  req->c = atoi(c);
  req->r = atoi(r);
  req->i = atoi(i);
  strcpy(req->operation, operation);
  return req;
}

void printRequest(struct request* req) {
  printf("\n");
  printf("===========Request===================\n");
  printf("client_ip: %s\n", req->client_ip);
  printf("name: %s\n", req->m);
  printf("client number: %d\n", req->c);
  printf("request number: %d\n", req->r);
  printf("incarnation number: %d\n", req->i);
  printf("operation: %s\n", req->operation);
  printf("=====================================\n");
}

void printError(const char *errorMessage, int die) {
  perror(errorMessage);
  if (die == 1) exit(1);
}
