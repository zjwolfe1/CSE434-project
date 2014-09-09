#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define OPERATION_MAX 80     /* Longest string to echo */

void DieWithError(const char *errorMessage) /* External error handling function */
{
    perror(errorMessage);
    exit(1);
}

typedef struct requestStruct {
  char *client_ip;
  char *m;
  unsigned int c;
  unsigned int r;
  unsigned int i;
  char *operation;
} request;

request* makeRequest(char* client_ip, char* m, unsigned int c, unsigned int r, unsigned int i, char* operation) {
  request* req = (request *)malloc(sizeof(request));
  req->client_ip = client_ip;
  req->m = m;
  req->c = c;
  req->r = r;
  req->i = i;
  req->operation = operation;

  return req;
}

void printRequest(request* req) {
    printf("---------- Sending Request --------------------\n");
    printf("req->client_ip: %s \n", req->m);
    printf("req->m: %s \n", req->m);
    printf("req->c: %d \n", req->c);
    printf("req->r: %d \n", req->r);
    printf("req->i: %d \n", req->i);
    printf("req->operation: %s \n", req->operation);
    printf("------------------------------\n");
}

int main(int argc, char *argv[])
{
    if (argc != 6) {
        fprintf(stderr,"Usage: %s <Name> <Client Number> <Server IP> <Port> [<Arguments>]\n", argv[0]);
        exit(1);
    }

    int sock;
    struct sockaddr_in servAddr;
    struct sockaddr_in fromAddr;
    unsigned int fromSize;

    // request = <client_ip> <m> <c> <r> [<operation>]
    request* req = makeRequest(argv[4], argv[1], 0, 0, 0, argv[5]);

    printRequest(req);

    char operationBuffer[OPERATION_MAX+1];
    int operationLen;
    int resLen;

    if ((operationLen = strlen(req->operation)) > OPERATION_MAX)  /* Check input length */
        DieWithError("Echo word too long");

    /* Create a datagram/UDP socket */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&servAddr, 0, sizeof(servAddr));    /* Zero out structure */
    servAddr.sin_family = AF_INET;                 /* Internet addr family */
    servAddr.sin_addr.s_addr = inet_addr(req->client_ip);  /* Server IP address */
    servAddr.sin_port   = htons(atoi(argv[4]));     /* Server port */

    /* Send the string to the server */
    sendto(sock, req, sizeof(req), 0, (struct sockaddr *) &servAddr, sizeof(servAddr));

    /* Recv a response */
    fromSize = sizeof(fromAddr);
    if ((resLen = recvfrom(sock, operationBuffer, OPERATION_MAX, 0,
         (struct sockaddr *) &fromAddr, &fromSize)) != operationLen)
        DieWithError("recvfrom() failed");

    /* null-terminate the received data */
    operationBuffer[resLen] = '\0';
    printf("Received: %s\n", operationBuffer);    /* Print the echoed arg */

    close(sock);
    exit(0);
}
