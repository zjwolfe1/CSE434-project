#include <stdio.h>      /* for printf(), fprintf(), and remove() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi(), exit(), and rand() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <time.h>       /* for time() */
#include <fcntl.h>              /* for fcntl() */

#define DEBUG 1 /* Boolean to Enable/Disable Debugging Output */
#define STR_SIZE 6 /* Length of String to represent States */
#define TIMEOUT_SEC 1 /* Length of the UDP recvfrom() Timeout (ms) */
#define TIMEOUT_MSEC 0 /* Length of the UDP recvfrom() Timeout (ms) */
#define TRUE 1 /* Boolean TRUE value */
#define FALSE 0 /* Boolean FALSE value */
#define INC_FILE "./inc.txt" /* Name of Text File to store Incarnation */

struct request {
  char client_ip[16];
  char m[24];
  int c;
  int r;
  int i;
  char operation[80];
};

void printError(const char *errorMessage, int die) {
  /* External error handling function */
  perror(errorMessage);
  if (die == TRUE) exit(1);
}

struct request* Request(char* client_ip, char* m, char* c, char* r, char* i, char* operation) {
  struct request* req = malloc(sizeof *req);
  strcpy(req->client_ip, client_ip);
  strcpy(req->m, m);
  req->c = atoi(c);
  req->r = atoi(r);
  req->i = atoi(i);
  strcpy(req->operation, operation);
  return req;
}

/**
 * getIncarnationNumber Method
 *
 * Searches stable storage for the previous incarnations of the request.
 * If found increment number and return. Else return 0 and store request.
 */
/* int getIncarnationNumber(int failure) { */
/*         FILE* fp; */
/*         int inc = 1; */

/*         fp = fopen(INC_FILE, "r+"); */

/*         if (fp != NULL) { */
/*                 flock(fileno(fp), LOCK_SH); */
/*                 fscanf(fp, "%d", &inc); */

/*                 /1* Check for Client Failure *1/ */
/*                 if (failure == TRUE) { */
/*                         /1* Rewind file pointer and increment counter *1/ */
/*                         rewind(fp); */
/*                         fprintf(fp, "%d", (inc + 1)); */
/*                 } */

/*                 flock(fileno(fp), LOCK_UN); */
/*                 fclose(fp); */
/*         } else { */
/*                 createIncarnationFile(); */
/*         } */

/*         return inc; */
/* } */

/* void removeIncarnation() { */
/*         remove("./inc.txt"); */
/* } */

void printRequest(struct request* req) {
  printf("\n");
  printf("-----------Request-------------------\n");
  printf("client_ip: %s\n", req->client_ip);
  printf("name: %s\n", req->m);
  printf("client number: %d\n", req->c);
  printf("request number: %d\n", req->r);
  printf("incarnation number: %d\n", req->i);
  printf("operation: %s\n", req->operation);
  printf("-------------------------------------\n");
}

int main(int argc, char* argv[]) {
  int sock; /* Socket descriptor */
  struct sockaddr_in serverAddr; /* Echo server address */
  struct sockaddr_in fromAddr; /* Source address of echo */
  unsigned short serverPort; /* Echo server port */
  unsigned int fromSize; /* In-out of address size for recvfrom() */
  char* servIP; /* IP address of server */
  char clientString[STR_SIZE] = "$$$$$\0"; /* Buffer for receiving Client String */
  struct request* req; /* Pointer fornFile Random Request Structure */
  struct timeval tv; /* Structure to Modify UDP Socket Timeout */
  int packetSent; /* Boolean to determine if Server sent response */
  int i; /* Declare Counter for Loops */

  if (argc != 6) {
    fprintf(stderr,"Usage: %s <Name> <Client Number> <Server IP> <Port> <Script>\n", argv[0]);
    exit(1);
  }

  req = Request(argv[3], argv[1], argv[2], "0", "0", "hi");

  printRequest(req);

  /* Server IP anFileddress (dotted quad) */
  servIP = argv[3];

  /* Use given port */
  serverPort = atoi(argv[4]);

  /* Create a datagram/UDP socket */
  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
          printError("socket() failed", TRUE);

  /* Construct the server address structure */
  memset(&serverAddr, 0, sizeof(serverAddr)); /* Zero out structure */
  serverAddr.sin_family = AF_INET; /* Internet addr family */
  serverAddr.sin_addr.s_addr = inet_addr(servIP); /* Server IP address */
  serverAddr.sin_port = htons(serverPort); /* Server port */

  if (sendto(sock, req, sizeof(*req), 0, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
  printError("sendto() sent a different number of bytes than expected", TRUE);

  /* Recv a response */
  fromSize = sizeof(fromAddr);

  if (recvfrom(sock, clientString, STR_SIZE, 0, (struct sockaddr *) &fromAddr, &fromSize) != STR_SIZE)
          printError("recvfrom() failed", FALSE);

  /* null-terminate the received data */
  printf("\n");
  printf("Received: %s\n", clientString); /* Print the Client String */
  printf("\n");
  printf("========================================================\n");

  /* Deallocate Memory of Pointers */
  free(req);

  close(sock);
  exit(0);
}
