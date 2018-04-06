#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <getopt.h>

long long int comparison = 0;

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
  comparison++;
  return(frobcmp(*(const char**) a, *(const char**) b));
}

int fcompare(const void* a, const void* b) {
  comparison++;
  const char* a2 = *(const char**) a;
  const char* b2 = *(const char**) b;

  int i;
  for (i = 0;; i++) {
    if (a2[i] == ' ' && b2[i] == ' ') {
      return 0;
    }
    if (a2[i] == ' ') {
      return -1;
    }
    else if (b2[i] == ' ') {
      return 1;
    }
    
    unsigned char acmp = toupper((unsigned char)(a2[i]^42));
    unsigned char bcmp = toupper((unsigned char)(b2[i]^42));
    
    if (acmp < bcmp) {
      return -1;
    }
    else if (acmp > bcmp) {
      return 1;
    }
  }
}

int main(int argc, char *argv[]) {
  
  int optf;
  if (getopt(argc, argv, "f") != -1) {
    optf = 1;
  }
  else {
    optf = 0;
  }
  
  struct stat fileStat;
  if(fstat(0,&fileStat) < 0)
    {
      fprintf(stderr, "Error with file");
      exit(1);
    }

  char** wordArr;
  char* inputstream = (char*) malloc(sizeof(char));
  int arrSlots = 0;
  int charSlots = 0;
  int wordIter = 0;

 
  if (S_ISREG(fileStat.st_mode)) {
    // printf("huah");
    inputstream = (char*) realloc(inputstream, (fileStat.st_size+1) * sizeof(char));  
    if (inputstream == NULL) {
      fprintf(stderr, "memory allocation error");
      exit(1);
    }

    if (read(0, inputstream, fileStat.st_size) < 0) {
      fprintf(stderr, "read error");
      exit(1);
    }
    int i;
    for (i = 0; i < fileStat.st_size; i++) {
      if (i == fileStat.st_size - 1) {
	inputstream[i+1] = ' ';
      }    
      int j = i;
      if (inputstream[i] == ' ') {
	while (inputstream[j] == ' ') {
	  j++;
	}
	i = j - 1;
	arrSlots++;
      }
    }
    
    wordArr = (char**) malloc(arrSlots * sizeof(char*));
    int cursize = 1;
    int charIter = 0;
    for (i = 0; i < fileStat.st_size; i++) {
      if (inputstream[i] != ' ') {
	wordArr[wordIter] = &inputstream[i];
	wordIter++;
	while (inputstream[i] != ' ') {
	  i++;
	}
      }
    }
    //    printf("%d", wordIter);
  }
  else {
    wordArr =  (char**) malloc(sizeof(char*));
    if (wordArr == NULL) {
      fprintf(stderr, "memory allocation error ");
      exit(1);
    }
  }
  int initWordNum = wordIter;
  int letterIter = 0;
  int wordSize = 1;
  char input[1];
  int iread;
  char next[1];
  int nread;
  char* word = (char*) malloc(sizeof(char));

  // fstat(0, &file);
  //  if (!S_ISREG(file.st_mode) || file.st_size > size) {
  iread =  read(0, input, sizeof(char));
  if (iread < 0) {
    fprintf(stderr, "read error 1");
    exit(1);
  }
  nread = read(0, next, sizeof(char));
  if (nread < 0) {
    fprintf(stderr, "read error 2");
    exit(1);
  }

  while (iread > 0) {
    word[letterIter] = input[0];
    letterIter++;

    char* testWord = (char*) realloc(word, (letterIter + 2) * sizeof(char));
    if (testWord == NULL) {
      free(word);
      fprintf(stderr, "memory allocation error 1");
      exit(1);
    }
    else {
      word = testWord;
      testWord = NULL;
    }
      
    if (input[0] == ' ') {
      wordArr[wordIter] = word;
      wordIter++;


      char** testWordArr = (char**) realloc(wordArr, (wordIter +2) * sizeof(char*));
      if (testWordArr == NULL) {
	free(wordArr);
	fprintf(stderr, "memory allocation error 2");
	exit(1);
      }
      else {
	wordArr = testWordArr;
	testWordArr = NULL;
	word = NULL;
	letterIter = 0;
	testWord = (char*) malloc(sizeof(char));
	if (testWord == NULL) {
	  fprintf(stderr, "memory allocation error 1");
	  exit(1);
	}
	else {
	  word = testWord;
	  testWord = NULL;
	} 	
      }

      if (nread && next[0] == ' ') {
	while (iread > 0 && input[0] == ' ') {
	  iread = read(0, input, sizeof(char));
	  if (iread < 0) {
	    fprintf(stderr, "read error 3");
	    exit(1);
	  }
	}
	if (iread == 0) {
	  break;
	}
	letterIter = 0;
	nread = read(0, next, sizeof(char));
	continue;
      }

    }

    if (!nread) {
      if (input[0] != ' ') {
	input[0] = ' ';
	continue;
      }
      else {
	break;
      }
    }

 


    input[0] = next[0];
    nread = read(0, next, sizeof(char));

  }//while iread > 0
  // }//!isreg
  
  if (optf) {
   qsort(wordArr, wordIter, sizeof(char*), fcompare);
  }
  else {
    qsort(wordArr, wordIter, sizeof(char*), compare);
  }

  int i;
  char output[1];
  for (i=0; i < wordIter; i++) {
    int j = 0;
    //char* temp = wordArr[i];                                                                   
    while (1) {
      output[0] = wordArr[i][j];
      write(1, output, sizeof(char));
      if (wordArr[i][j] == ' ') {
        break;
      }
      j++;
    }
  }
  
  int x;
  for(x=initWordNum;x<wordIter;x++) {
    //    printf("value of x: %d\n",x);
    if(wordArr[x]!=NULL) {
      free(wordArr[x]);
      wordArr[x] = NULL;
    }
    }
  free(wordArr);
  free(word);
  free(inputstream);
  exit(0);
}
