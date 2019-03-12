// Basic test

#include <stdio.h>
#include <malloc.h>

int main(){

  int* data = (int*)malloc(1024);

  free(data);

  return 0;
}
