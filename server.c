#include "server.h"

int main(int argc, char* argv[]) {
  char clientString[REQUEST_MAX] = "     \0";
  int sock; /* Socket */
  struct sockaddr_in clientAddr; /* Client address */
  struct sockaddr_in servAddr;
  unsigned int cliAddrLen;
  unsigned short port;
  struct request* req;
  struct response* res;

  if (argc != 2) {
    fprintf(stderr, "Usage:  %s <UDP SERVER PORT>\n", argv[0]);
    exit(1);
  }

  port = atoi(argv[1]);

  /* Create socket for sending/receiving datagrams */
  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    printError("socket() failed", 1);

  /* Construct local address structure */
  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(port);

  /* Bind to the local address */
  if (bind(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
    printError("bind() failed", 1);

  fprintf(stderr, "================================================\n");
  fprintf(stderr, "Starting server on port %d\n", port);
  fprintf(stderr, "================================================\n");
  for (;;) {
    cliAddrLen = sizeof(clientAddr);
    req = malloc(sizeof *req);

    /* printf("before recvfrom()"); */
    if (recvfrom(sock, req, sizeof(struct request), 0, (struct sockaddr *) &clientAddr, &cliAddrLen) < 0)
      printError("recvfrom() failed", 1);
    else {
      printRequest(req);
      res = handleRequest(req);
      printResponse(res);
    }

    if (sendto(sock, res, sizeof(*res), 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr)) < 0)
            printError("sendto() sent a different number of bytes than expected", 1);

    printf("========================================================\n");
    printf("Response Sent...\n");
    printf("========================================================\n");

    // cleanup
    free(req);
    free(res);
  }
}

struct response* handleRequest(struct request *req) {
  printf("\nHandling client: %s", req->client_ip);

  struct client* c;
  struct response *res;

  switch (rand() % 3) {
    case 0:
      // drop the request
      printf("\nDropping the request... \n");
      res = malloc(sizeof(*res));
      break;
    case 1:
      // handle but not send the response
      printf("\nHandling the request, but not responding... \n");
      c = findClient(req);
      res = malloc(sizeof(*res));
      handleOperation(c, req->operation);
      break;
    case 2:
      // 1. perform the file op, record the response in the
      // 2. client table, update the response number in the client
      // table
      // handle and send the response
      // return the code of the file op
      c = findClient(req);
      res = handleOperation(c, req->operation);
      printClient(c);
  }

  return res;
}

struct client* findClient(struct request *req) {
  for (int i = 0; i < clientNum; i++)
    if (isClient(req, clients[i]))
      return clients[i];

  return addClient(req);
}

struct client* addClient(struct request *req) {
  printf("\nAdding new client! \n");

  struct client *c =  Client(req->m, req->c, req->r, req->i);
  clients[clientNum] = c;
  clientNum++;

  return c;
}

// compare requets to client, just a lil helper
int isClient(struct request *req, struct client *c) {
  return (
      !strcmp(req->m, c->m) &&
      req->c == c->c &&
      req->r == c->r &&
      req->i == c->i
     ) ? 1 : 0;
}

// Constructor for client
struct client* Client(char* m, unsigned int c, unsigned int r, unsigned int i) {
  struct client *client = malloc(sizeof(*client));
  strcpy(client->m, m);
  client->c = c;
  client->r = r;
  client->i = i;

  return client;
}

void printClient(struct client *c) {
  printf("\n============== Client ================\n");
  printf("name: %s \n", c->m);
  printf("client number: %d \n", c->c);
  printf("request number: %d \n", c->r);
  printf("incarnartion number: %d \n", c->i);
  printf("=======================================\n");
}

struct response* handleOperation(struct client *c, char* operation) {
  char* file = getWord(operation, 1);
  char* options = getWord(operation, 2);
  char* fileName = prefixFilename(file, c);
  char* result;
  struct response* res = (struct response*)malloc(sizeof(struct response));
  printf("file:%s \n", fileName);
  printf("options: %s \n", options);

  if (strstr(operation, OPEN) != NULL) {
    printf("\nOpening! \n");
    res->status = open(fileName, options);
    res->r = c->r;
  }

  else if (strstr(operation, CLOSE) != NULL) {
    printf("\nclosing! \n");
    res->status = closeFile(fileName);
    res->r = c->r;
  }

  else if (strstr(operation, READ) != NULL) {
    printf("\nreading! \n");
    readFile(res->body, fileName, atoi(options));
    res->r = c->r;
    res->status = (res->body) ? 1 : 0;
  }

  else if (strstr(operation, LSEEK) != NULL)
    printf("\nlseeking! \n");

  else if (strstr(operation, WRITE) != NULL) {
    printf("\nwriting! \n");
    res->status = writeFile(fileName, options);
    res->r = c->r;
  }
  else if (strstr(operation, FAIL) != NULL)
    printf("\nfailing! \n");
  else
    printf("\n not sure what to do... \n");

  return res;
}

char* getWord(char* line, int wordNum) {
  int words = 0, index = 0, len = strlen(line);
  char* file = (char*)malloc(sizeof(len));
  char letter;

  for (int i = 0; i < len; i++) {
    letter = line[i];

    // last check is because of the quotes
    if (words == wordNum && !isspace(letter) && '"' != letter) {
      file[index] = letter;
      index++;
    }
    if (isspace(letter)) words++;
  }

  return file;
}

int open(char* file, char* permissions) {
  int index = getFile(file);
  int read = (strstr(permissions, READ) != NULL) ? 1 : 0;
  int write = (strstr(permissions, WRITE) != NULL) ? 1 : 0;
  struct file *f;

  // not found
  if (index == -1) {
    f = (struct file *)malloc(sizeof(struct file));
    f->fileName = file;
    f->file = fopen(file, "r+");

    // if not found try again
    if (!f->file) {
      f->file = fopen(file, "w+");
    }

    files[fileNum] = f;
    fileNum++;
  } else {
    f = files[index];
  }

  f->read = read;
  f->write = write;

  return 1;
}

// search for file, if found return else
// make new client and add it
int getFile(char* fileName) {
  for (int i = 0; i < fileNum; i++)
    if (strcmp(fileName, files[i]->fileName) == 0)
      return i;

  return -1;
}

void readFile(char* body, char* file, int bytes) {
  int index = getFile(file);
  struct file* f;

  if (index == -1) return;

  f = files[index];

  // check if can read
  if (f->read && f->file)
    fread(body, 1, bytes, f->file);
  else
    printf("permission denied.\n");
}

char* prefixFilename(char* file, struct client *c) {
  char buf[sizeof(int) + 1];
  sprintf(buf, "%d", c->c);
  char* fileName = strcat(c->m, buf);
  fileName = strcat(fileName, "-");
  fileName = strcat(fileName, file);

  return fileName;
}

int closeFile(char* file) {
  int index = getFile(file);
  struct file* f;

  if (index == -1) return -1;

  f = files[index];
  return fclose(f->file);
}

int writeFile(char* file, char* argument) {
  int index = getFile(file);
  struct file* f;

  if (index == -1) return -1;

  f = files[index];
  return fprintf(f->file, "%s", argument);
}
