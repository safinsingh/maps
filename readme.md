# maps

`hash_map.c`: c hashmap implementation aimed at providing efficient lookups, low collision rates, and efficient memory usage by maintaining a load factor of ~0.7. uses djb2a for hashing and separate-chaining for collision handling.

`bst_map.c`: binary search tree map with O(log n) amortized lookups and insertion. implements breadth-first traversal to print tree and bfs for top-down search.

`queue.h`: simple linked-list based FIFO queue

`stack.h`: self-resizing vector-based LIFO stack
