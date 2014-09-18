#include "utils.h"

#ifndef HEADER_FILE
#define HEADER_FILE

#define REQUEST_MAX 255

struct client {
  char client_ip[16];
  char m[24];
  int c;
  int r;
  int i;
};

int fileLock[REQUEST_MAX];
struct client *clients[REQUEST_MAX];
int clientNum = 0;

void handleRequest(struct request *req);
struct client* findClient(struct request *req);
struct client* addClient(struct request *req);
int isClient(struct request *req, struct client *c);
struct client* Client(char* m, unsigned int c, unsigned int r, unsigned int i);
void printClient(struct client *c);

#endif
