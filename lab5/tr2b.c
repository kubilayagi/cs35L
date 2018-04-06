#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  char input = getchar();
  while (!feof(stdin)) {
   
    int change = posInFrom(from, frLen, input);
    if (change != -1) {
      putchar(to[change]);
    }
    else {
      putchar(input);
    }
    input = getchar();
  }

  return 0;
}
