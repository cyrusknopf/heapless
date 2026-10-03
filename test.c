#include <stdio.h>

[[heapless::attr1]]
int main() {
  puts("Compiled");
  return 0;
}
