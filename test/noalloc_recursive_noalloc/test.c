[[heapless::noalloc]] int recursive(int x) {
  if (x == 0) return 0;
  return 1 + recursive(x - 1);
}
