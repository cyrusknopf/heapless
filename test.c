#include <stdlib.h>
#include <stdio.h>

[[heapless::attr1]] int main()
{
  puts("Compiled");
  if (rand() % 2) {
    return 1;
  }
  return 0;
}
