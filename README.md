**heapless**

Compile-time restriction on dynamic memory allocations.

Currently only supports explicit calls to malloc.

~~~c
[[heapless::noalloc]] void* allocating(size_t sz)
{
  return malloc(sz);
}

❯ gcc -fplugin=./heapless.so -c test.c
In function ‘allocating’:
error: malloc() used in noalloc function
    4 | [[heapless::noalloc]] void* allocating(size_t sz) {
      |
~~~

**Resources and references**
- https://stephanfr.com/2013/05/19/building-gcc-plugins-part-1-c-11-generalized-attributes/

- https://github.com/rofirrim/gcc-plugins

- https://lwn.net/Articles/457543/
