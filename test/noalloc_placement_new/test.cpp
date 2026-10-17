#include <new>

[[heapless::noalloc]] auto
allocating() -> void
{
    int* ptr = nullptr;
    volatile int* x = new (ptr) int;
}

int main() {}
