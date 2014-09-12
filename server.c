#include "server.h"
#include "utils.h"


int main(int argc, char* argv[]) {
  char clientString[REQUEST_MAX] = "     \0";
  int sock; /* Socket */
  struct sockaddr_in clientAddr; /* Client address */
  struct sockaddr_in servAddr;
  unsigned int cliAddrLen;
  unsigned short port;
  struct request* req;

  fileLock[0] = 1;

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
    fprintf(stderr, "before recvfrom()\n");
    if (recvfrom(sock, req, sizeof(struct request), 0, (struct sockaddr *) &clientAddr, &cliAddrLen) < 0)
      printError("recvfrom() failed", 1);
    else
      printRequest(req);
    fprintf(stderr, "after recvfrom()\n");

    if (sendto(sock, req, sizeof(*req), 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr)) < 0)
            printError("sendto() sent a different number of bytes than expected", 1);

    printf("========================================================\n");
    printf("Request Sent...\n");
    printf("========================================================\n");

    // cleanup
    free(req);
  }
}
