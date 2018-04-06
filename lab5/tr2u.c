#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int posInFrom (const char* c, size_t len, char check) {
  size_t i;
  for (i = 0; i < len; i++) {
    if (c[i] == check) {
      return i;
    }
  }
  return -1;
}

int main(int argv, char* argc[]) {
  if (argv != 3) {
    fprintf(stderr, "You need two arguments");
    exit(1);
  }
  size_t frLen = strlen(argc[1]);
  size_t tLen = strlen(argc[2]);
  if (frLen != tLen) {
    fprintf(stderr, "Arguments are different length");
    exit(1);
  }
  char* from = argc[1];
  char* to = argc[2];

  char buf[1];
  while (read(0, buf, 1) > 0) {
    int change = posInFrom(from, frLen, buf[0]);
    if (change != -1) {
      write(1, to + change, 1);
    }
    else {
      write(1, buf, 1);
    }
  }
  exit(0);
}
