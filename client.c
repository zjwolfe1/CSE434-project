#include "utils.h"

int main(int argc, char* argv[]) {
  int sock;
  struct sockaddr_in serverAddr;
  struct sockaddr_in fromAddr;
  unsigned short serverPort;
  unsigned int fromSize;
  char* servIP;
  char clientString[REQUEST_MAX] = "$$$$$\0";
  struct request *req;
  struct response *res;
  res = malloc(sizeof(*res));

  if (argc != 6) {
    fprintf(stderr,"Usage: %s <Name> <Client Number> <Server IP> <Port> <Script>\n", argv[0]);
    exit(1);
  }

  // setup
  req = Request(argv[3], argv[1], argv[2], "0", "0", argv[5]);
  printRequest(req);
  servIP = argv[3];
  serverPort = atoi(argv[4]);

  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    printError("socket() failed", 1);

  // server setup
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = inet_addr(servIP);
  serverAddr.sin_port = htons(serverPort);

  if (sendto(sock, req, sizeof(*req), 0, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
    printError("sendto() sent a different number of bytes than expected", 1);

  fromSize = sizeof(fromAddr);
  if (recvfrom(sock, res, sizeof(*res), 0, (struct sockaddr *) &fromAddr, &fromSize) < 0)
    printError("recvfrom() failed", 1);

  printf("================================\n");
  printf("RECIEVING RES\n");
  printf("================================\n");
  printResponse(res);

  // cleanup
  free(req);
  free(res);

  close(sock);
  exit(0);
}
