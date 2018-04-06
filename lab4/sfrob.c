#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int frobcmp(char const* a, char const* b) {
  int i = 0;
  for (i = 0;; i++) {
    if (a[i] == ' ' && b[i] == ' ') {
      return 0;
    }
    if (a[i] == ' ') {
      return -1;
    }
    else if (b[i] == ' ') {
      return 1;
    }


    if ((a[i] ^ 42) < (b[i] ^ 42)) {
      return -1;
    }
    else if ((a[i] ^ 42) > (b[i] ^ 42)) {
      return 1;
    }
  }
}

int compare(const void* a, const void* b) {
  return(frobcmp(*(const char**) a, *(const char**) b));
}

int main(int argc, char *argv[]) {
  char** wordArr;
  char* word;
  char input = getchar();
  if (ferror(stdin)) {
    fprintf(stderr,"error reading char %d\n", errno);
    exit(1);
  } 
  char checknext = getchar();
  if (ferror(stdin)) {
    fprintf(stderr,"error reading char %d\n", errno);
    exit(1);
  } 
  int letterIter = 0;
  int wordIter = 0;
  word = (char*) malloc(sizeof(char));
  if (word == NULL) {
    fprintf(stderr, "memory allocation error %d\n",errno);
    exit(1);
  }
  wordArr = (char**) malloc(sizeof(char*));
  if (wordArr == NULL) {
    fprintf(stderr, "memory allocation error %d\n", errno);
    exit(1);
  }

  int wordSize = 1;
  int lineSize = 1;

  while(!ferror(stdin) && input != EOF) {
    word[letterIter] = input;
    wordSize++;
    if (letterIter + 1 >= wordSize) {
      wordSize *= 2;
    }
    char* test = (char*) realloc(word, wordSize * sizeof(char));
    if (test == NULL) {
      free(word);
      fprintf(stderr, "memory allocation error %d\n",errno);
      exit(1);
    }
    else {
      word = test;
    }

    if (input == ' ') {
	wordArr[wordIter] = word;
	wordIter++;
	if (wordIter >= lineSize) {
	  lineSize *= 2;
	}
	char** test2 = (char**) realloc(wordArr, lineSize * sizeof(char*));
	if (wordArr == NULL) {
	  free(wordArr);
	  fprintf(stderr, "memory allocation error %d\n", errno);
	  exit(1);
	}
	else {
	  wordArr = test2;
	}
	letterIter = -1;
	word = NULL;
	word = (char*) malloc(sizeof(char*));
	if (word == NULL) {
	  fprintf(stderr, "memory allocation error %d\n",errno);
	  exit(1);
	}
    }     
    if (input == ' ' && checknext == EOF) {
      break;
    }
    else if (input == ' ' && checknext == ' ') {
      while (input == ' ') {
	input = getchar();
	if (ferror(stdin)) {
	  fprintf(stderr,"error reading char %d\n", errno);
	  exit(1);
	} 
      }
      checknext = getchar();
      if (ferror(stdin)) {
	fprintf(stderr, "error reading char %d\n", errno);
	exit(1);
      }
      letterIter++;
      continue;
    }
    else if (input != ' ' && checknext == EOF) {
      word[++letterIter] = ' ';
      if (letterIter + 1 >= wordSize) {
	wordSize *= 2;
      }
      char* test3 = (char*) realloc(word, wordSize * sizeof(char));
      if (test3 == NULL) {
	free(word);
	fprintf(stderr, "memory allocation error %d\n",errno);
	exit(1);
      }
      else {
	word = test3;
      }
      wordArr[wordIter] = word;
      wordIter++;
      if (wordIter >= lineSize) {
	lineSize *= 2;
      }
      char** test4 = (char**) realloc(wordArr, lineSize * sizeof(char*));
      if (test4 == NULL) {
	free(wordArr);
	fprintf(stderr, "memory allocation error %d\n", errno);
	exit(1);
      }
      else {
	wordArr = test4;
      }
      break;
    }
    input = checknext;
    checknext = getchar();
    if (ferror(stdin)) {
      fprintf(stderr,"error reading char %d\n", errno);
      exit(1);
    } 
    letterIter++;
  }
  
  qsort(wordArr, wordIter, sizeof(char*), compare);
  

  int i = 0;

  //print the words

  for (; i < wordIter; i++) {
    int j = 0;
    //char* temp = wordArr[i];
    while (1) {
      putchar(wordArr[i][j]);
      if (wordArr[i][j] == ' ') {
	break;
      }
      j++;
    }
  }

  //free the memory
  
  for (i = 0; i < lineSize; i++) {
    free(wordArr[i]);
  }
  
  free(wordArr);

  exit(0);
}
