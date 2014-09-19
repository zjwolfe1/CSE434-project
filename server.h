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
  char* fileName;
  FILE* file;
  int read;
  int write;
};

int clientNum = 0;
int fileNum = 0;
int lastRequest;
struct client *clients[REQUEST_MAX];
struct file *files[REQUEST_MAX];

struct response* handleOperation(struct client *c, char* operation);
int isClient(struct request *req, struct client *c);
struct client* Client(char* m, unsigned int c, unsigned int r, unsigned int i);
struct client* addClient(struct request *req);
struct client* findClient(struct request *req);
struct response* handleRequest(struct request *req);
void printClient(struct client *c);
char* getWord(char* line, int wordNum);
int open(char* file, char* permissions);
int closeFile(char* file);
int getFile(char* fileName);
char* prefixFilename(char* file, struct client* clientName);
void readFile(char* body, char* file, int bytes);
/* int write(struct file *file, char* argument); */

#endif
