#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define COMMAND_MAX 255     /* Longest string to echo */

void DieWithError(const char *errorMessage) /* External error handling function */
{
    perror(errorMessage);
    exit(1);
}

struct request{
  char client_ip[16];
  char m[24];
  int c;
  int r;
  int i;
  char operation[80];
};

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in servAddr;
    struct sockaddr_in fromAddr;
    unsigned int fromSize;

    // Client args
    char *name;
    int client;
    char *servIP;
    int port;
    char *commandArgs;

    char commandBuffer[COMMAND_MAX+1];
    int commandArgsLen;
    int resLen;

    if (argc != 6)    /* Test for correct number of arguments */
    {
        fprintf(stderr,"Usage: %s <Name> <Client Number> <Server IP> <Port> [<Arguments>]\n", argv[0]);
        exit(1);
    }

    name = argv[1];
    client = atoi(argv[2]);
    servIP = argv[3];
    port = atoi(argv[4]);
    commandArgs = argv[5];

    if ((commandArgsLen = strlen(commandArgs)) > COMMAND_MAX)  /* Check input length */
        DieWithError("Echo word too long");

    /* Create a datagram/UDP socket */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&servAddr, 0, sizeof(servAddr));    /* Zero out structure */
    servAddr.sin_family = AF_INET;                 /* Internet addr family */
    servAddr.sin_addr.s_addr = inet_addr(servIP);  /* Server IP address */
    servAddr.sin_port   = htons(port);     /* Server port */

    /* Send the string to the server */
    if (sendto(sock, commandArgs, commandArgsLen, 0, (struct sockaddr *)
               &servAddr, sizeof(servAddr)) != commandArgsLen)
        DieWithError("sendto() sent a different number of bytes than expected");

    /* Recv a response */
    fromSize = sizeof(fromAddr);
    if ((resLen = recvfrom(sock, commandBuffer, COMMAND_MAX, 0,
         (struct sockaddr *) &fromAddr, &fromSize)) != commandArgsLen)
        DieWithError("recvfrom() failed");

    /* null-terminate the received data */
    commandBuffer[resLen] = '\0';
    printf("Received: %s\n", commandBuffer);    /* Print the echoed arg */

    close(sock);
    exit(0);
}
