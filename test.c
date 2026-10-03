#include <stdlib.h>
#include <stdio.h>

int
main() [[heapless::attr1]]
{
  puts("Compiled");
  if (rand() % 2) {
    return 1;
  }
  return 0;
}
