# How to compile?
```bash
$ g++ radix_tree.cpp compressed_radix_tree.cpp rbtree.cpp ycsb.cpp -o test
```

# How to test and get the result?
```bash
$ ./test
```
the result will be shown in the terminal as following format:
```
RadixTree
workload-1
average: P50: P90: P99:
workload-2
average: P50: P90: P99:
workload-3
average: P50: P90: P99:

CompressedRadixTree
workload-1
average: P50: P90: P99:
workload-2
average: P50: P90: P99:
workload-3
average: P50: P90: P99:

RBTree
workload-1
average: P50: P90: P99:
workload-2
average: P50: P90: P99:
workload-3
average: P50: P90: P99:
```
# How to clean?
```bash
$ rm test
```