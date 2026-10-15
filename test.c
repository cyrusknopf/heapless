#include <stdlib.h>
#include <stdio.h>

[[heapless::noalloc]] volatile void* allocating() {
  return malloc(100);
}

[[heapless::noalloc]] volatile void* nonallocating() {
  return NULL;
}

int main()
{
  volatile void* bytes = allocating();
  volatile void* null = nonallocating();
  return 0;
}
