#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define COMMAND_MAX 255     /* Longest string to echo */

void DieWithError(const char *errorMessage) /* External error handling function */
{
    perror(errorMessage);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sock;                        /* Socket */
    struct sockaddr_in servAddr; /* Local address */
    struct sockaddr_in clientAddr; /* Client address */
    unsigned int cliAddrLen;         /* Length of incoming message */
    char commandBuffer[COMMAND_MAX];        /* Buffer for echo string */
    unsigned short port;     /* Server port */
    int reqSize;                 /* Size of received message */

    if (argc != 2)         /* Test for correct number of parameters */
    {
        fprintf(stderr,"Usage:  %s <Port>\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[1]);  /* First arg:  local port */

    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Construct local address structure */
    memset(&servAddr, 0, sizeof(servAddr));   /* Zero out structure */
    servAddr.sin_family = AF_INET;                /* Internet address family */
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    servAddr.sin_port = htons(port);      /* Local port */

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        DieWithError("bind() failed");

    printf("-------------------------------------------- \n");
    printf("Server listening on port %d \n", port);
    printf("-------------------------------------------- \n");
    for (;;) /* Run forever */
    {
        cliAddrLen = sizeof(clientAddr);

        if ((reqSize = recvfrom(sock, commandBuffer, COMMAND_MAX, 0,
            (struct sockaddr *) &clientAddr, &cliAddrLen)) < 0)
            DieWithError("recvfrom() failed");

        /* Do stuff in here i think. */
        /* printf("reqSize: %d", reqSize); */
        if (sendto(sock, commandBuffer, reqSize, 0,
             (struct sockaddr *) &clientAddr, sizeof(clientAddr)) != reqSize)
            DieWithError("sendto() sent a different number of bytes than expected");
    }
}
