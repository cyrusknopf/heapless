#include <stdlib.h>
#include <stddef.h>

[[heapless::noalloc]] void* allocating(size_t sz)
{
  return NULL;
}
