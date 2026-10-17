#include <stdlib.h>

[[heapless::noalloc]] int recursive(int x) {
  if (x == 0) {
    return 0;
  }
  volatile void* y = malloc(x);
  return 1 + recursive(x - 1);
}
