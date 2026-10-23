#include <vector>

[[heapless::noalloc]] auto
main() -> int
{
    std::vector<int> ints {1, 2, 3};
}
