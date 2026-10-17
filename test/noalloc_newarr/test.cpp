[[heapless::noalloc]] auto
allocating() -> void
{
    volatile int* x = new int[10];
}

int main() {}
