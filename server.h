#ifndef HEADER_FILE
#define HEADER_FILE

#define REQUEST_MAX 255

struct clientTable {
  char client_ip[16];
  char m[24];
  int c;
  int r;
  int i;
};

int fileLock[REQUEST_MAX];

#endif
