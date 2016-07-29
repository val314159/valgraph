#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#define BUF_SZ (4*1024*1024)
FILE *f = 0;
long page_no = 0;
char *buf = 0;
long offset = 0;
long state = 10000;
long line_no = 0;
char ch = 0;
void step() {
  ch = buf[offset++];
  switch(state){
  case 10000:
    ++line_no;
    switch(ch){
    case '#':
      state = 10001;
      break;
    case '<':
      state = 11001;
      break;
    default:
      printf("Unknown Character\n");
      exit(1);
    }
    break;
  case 10001:
    switch(ch){
    case '\n':
      state = 10000;
      break;
    }
    break;
  case 11001:
    switch(ch){
    case '\n':
      printf("eol err\n");
      exit(1);
    case '>':
      state = 19999;
      break;
    }
    break;
  case 19999:
    switch(ch){
    case '\n':
      printf("eol err\n");
      exit(1);
    case ' ':
      state = 20000;
      break;
    }
    break;
  case 20000:
    switch(ch){
    case '\n':
      printf("eol err\n");
      exit(1);
    case '<':
      state = 21001;
      break;
    default:
      printf("Unknown Character\n");
      exit(1);
    }
    break;
  case 21001:
    switch(ch){
    case '\n':
      printf("eol err\n");
      exit(1);
    case '>':
      state = 29999;
      break;
    }
    break;
  case 29999:
    switch(ch){
    case '\n':
      printf("eol err\n");
      exit(1);
    case ' ':
      state = 30000;
      break;
    }
    break;
  case 30000:
    switch(ch){
    case '\n':
      printf("eol err\n");
      exit(1);
    case '<':
      state = 31001;
      break;
    case '\"':
      state = 32001;
      break;
    default:
      printf("Unknown Character\n");
      exit(1);
    }
    break;
  case 32001:
    switch(ch){
    case '\n':
      printf("eol err\n");
      exit(1);
    case '\\':
      state = 32002;
      break;
    case '\"':
      state = 39000;
      break;
    }
    break;
  case 32002:
    switch(ch){
    case '\n':
      printf("eol err\n");
      exit(1);
    default:
      state = 32001;
    }
    break;
  case 39000:
    switch(ch){
    case '\n':
      printf("eol err\n");
      exit(1);
    case '@':
      state = 39001;
      break;
    default:
      printf("Unknown Character\n");
      exit(1);
    }
    break;
  case 39001:
    switch(ch){
    case '\n':
      printf("eol err\n");
      exit(1);
    case 'e':
      state = 39002;
      break;
    default:
      printf("Unknown Character\n");
      exit(1);
    }
    break;
  case 39002:
    switch(ch){
    case '\n':
      printf("eol err\n");
      exit(1);
    case 'n':
      state = 39003;
      break;
    default:
      printf("Unknown Character\n");
      exit(1);
    }
    break;
  case 39003:
    switch(ch){
    case '\n':
      printf("eol err\n");
      exit(1);
    case ' ':
      state = 39004;
      break;
    default:
      printf("Unknown Character\n");
      exit(1);
    }
    break;
  case 39004:
    switch(ch){
    case '\n':
      printf("eol err\n");
      exit(1);
    case '.':
      state = 39005;
      break;
    default:
      printf("Unknown Character\n");
      exit(1);
    }
    break;
  case 39005:
    switch(ch){
    case '\n':
      state = 10000;
      break;
    default:
      printf("Unknown Character\n");
      exit(1);
    }
    break;
  default:
    printf("you're in a baaaad state.\n");
    exit(1);
  }
}

void process(char*filename) {
  if(!buf)
    buf=malloc(BUF_SZ);
  f = fopen(filename,"rb");
  size_t sz = 0;
  while((sz = fread(buf, 1, BUF_SZ-1, f))){
    page_no++;
    printf("pg=%5ld sz=%zu line_no=%ld\n", page_no, sz, line_no);
    offset = 0;
    while(offset<sz)
      step();
  }
  printf("EOF! (state=%ld) %ld lines.\n", state, line_no);
  fclose(f);
}

int main(int argc, char *argv[]){
  if(argv[1]){
    printf("Let's do %s\n", argv[1]);
    process(argv[1]);
  } else {
    printf("Gimme a filename.\n");
  }
}
