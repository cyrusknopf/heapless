[[heapless::noalloc]] auto
allocating() -> void
{
    volatile int* x = new int;
}

int main() {}
