#include <stdlib.h>
#include <stdio.h>

[[heapless::attr1]] int main()
{
  volatile void* bytes = malloc(10);
  return 0;
}
