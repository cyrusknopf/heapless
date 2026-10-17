**heapless**

Compile-time restriction on dynamic memory allocations.

See supported cases in `test/`.

**example usage**

~~~c
❯ cat test.c
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
~~~
```bash
❯ gcc -fplugin=./heapless.so -c test.c
test.c: In function ‘nonallocating’:
test.c:6:10: error: malloc() used in noalloc function
    6 |   return malloc(sz);
      |          ^~~~~~~~~~
test.c:11:3: note: allocation occurs in function called here
   11 |   allocating(sz + 1);
      |   ^~~~~~~~~~~~~~~~~~
```

**building**

`make` produces top-level `heapless.so`

`make test` runs expect-tests in `test/`


**Resources and references**
- https://stephanfr.com/2013/05/19/building-gcc-plugins-part-1-c-11-generalized-attributes/

- https://github.com/rofirrim/gcc-plugins

- https://lwn.net/Articles/457543/
