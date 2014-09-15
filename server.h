#include "utils.h"

#ifndef HEADER_FILE
#define HEADER_FILE

#define REQUEST_MAX 255
#define OPEN "open"
#define CLOSE "close"
#define READ "read"
#define LSEEK "lseek"
#define FAIL "fail"
#define WRITE "write"

struct client {
  char client_ip[16];
  char m[24];
  int c;
  int r;
  int i;
};

struct file {
  FILE *file;
  char *client;
  int lock;
};

struct response {
  int status;
  int r;
  char *body;
};

int clientNum = 0;
int lastRequest;
struct client *clients[REQUEST_MAX];
struct file *files[REQUEST_MAX];

char* handleOperation(struct client *c, char* operation);
int isClient(struct request *req, struct client *c);
struct client* Client(char* m, unsigned int c, unsigned int r, unsigned int i);
struct client* addClient(struct request *req);
struct client* findClient(struct request *req);
struct response* handleRequest(struct request *req);
void printClient(struct client *c);
char* getWord(char* line, int wordNum);


#endif
