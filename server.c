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

  for (;;) {
    cliAddrLen = sizeof(clientAddr);
    req = malloc(sizeof *req);

    /* printf("before recvfrom()"); */
    fprintf(stderr, "================================================\n");
    fprintf(stderr, "Starting server on port %d\n", port);
    fprintf(stderr, "================================================\n");
    if (recvfrom(sock, req, sizeof(struct request), 0, (struct sockaddr *) &clientAddr, &cliAddrLen) < 0)
      printError("recvfrom() failed", 1);
    else {
      printRequest(req);
      handleRequest(req);
    }

    if (sendto(sock, req, sizeof(*req), 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr)) < 0)
            printError("sendto() sent a different number of bytes than expected", 1);

    printf("========================================================\n");
    printf("Request Sent...\n");
    printf("========================================================\n");

    // cleanup
    free(req);
  }
}

struct response* handleRequest(struct request *req) {
  printf("\n Handling client: %s", req->client_ip);

  struct client* c;
  struct response *res = malloc(sizeof(*res));

  switch (rand() % 3) {
    case 0:
      // drop the request
      printf("\n Dropping the request... \n");
      break;
    case 1:
      // handle but not send the response
      printf("\n Handling the request, but not responding... \n");
      c = findClient(req);
      handleOperation(c, req->operation);
      break;
    case 2:
      // 1. perform the file op, record the response in the
      // 2. client table, update the response number in the client
      // table
      // handle and send the response
      // return the code of the file op
      c = findClient(req);
      handleOperation(c, req->operation);
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
  printf("\n Adding new client! \n");

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

char* handleOperation(struct client *c, char* operation) {
  char* file = getWord(operation, 1);
  char* result;
  printf("\n file: %s \n", file);

  if (strstr(operation, OPEN) != NULL) {
    printf("\n Opening! \n");
  }
  else if (strstr(operation, CLOSE) != NULL)
    printf("\n closing! \n");
  else if (strstr(operation, READ) != NULL)
    printf("\n reading! \n");
  else if (strstr(operation, LSEEK) != NULL)
    printf("\n lseeking! \n");
  else if (strstr(operation, WRITE) != NULL)
    printf("\n writing! \n");
  else if (strstr(operation, FAIL) != NULL)
    printf("\n failing! \n");
  else
    printf("\n not sure what to do... \n");

  return "hi";
}

char* getWord(char* line, int wordNum) {
  int words = 0, index = 0, len = strlen(line);
  char* file = (char*)malloc(sizeof(len));
  char letter;

  for (int i = 0; i < len; i++) {
    letter = line[i];

    if (isspace(letter)) words++;
    if (words == wordNum) {
      file[index] = letter;
      index++;
    }
  }

  file[index] = '\0';
  return file;
}
