#include <stdlib.h>
#include <stddef.h>

void* allocating(size_t sz)
{
  return malloc(sz);
}

[[heapless::noalloc]] void nonallocating(size_t sz)
{
  allocating(sz + 1);
}
