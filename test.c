#include <stdlib.h>
#include <stdio.h>

[[heapless::attr1]] volatile void* allocating() {
  return malloc(100);
}

[[heapless::attr1]] volatile void* nonallocating() {
  return NULL;
}

int main()
{
  volatile void* bytes = allocating();
  volatile void* null = nonallocating();
  return 0;
}
