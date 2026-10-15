#include <stdlib.h>
#include <stdio.h>

void other(int x) {
  volatile void* y = malloc(x);
}

[[heapless::noalloc]] volatile void* allocating() {
  other(10);
  return NULL;
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
