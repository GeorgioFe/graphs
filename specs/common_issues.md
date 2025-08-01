# Common Issues

## Code is Too Slow

The most likely culprit for code that is too slow is the use of inefficient data structures, either within your implementations of the member functions or even within your `Graph` class design itself. One way to help pinpoint the specific slow parts of your code is to use `gprof` (see the "How to Debug" part of this write-up for usage information). In general, there is no "one size fits all" way to speed up your code: for each operation or algorithm your `Graph` class implements, think of each step of your code, then think about the actual data structures and algorithms that your code is employing, and consider the time complexity of each step.

## Infinite Loop in Graph Traversal

Remember that the edges in your `Graph` class are undirected edges, meaning an edge from *u* to *v* can also be traversed from *v* to *u*. As such, one common mistake is to accidentally re-add nodes you've already visited back into the container you're using to keep track of your graph traversal (e.g., a queue for Breadth-First Search or a priority queue for Dijkstra's Algorithm). You can check for this by stepping through your code (e.g., using `gdb`) and seeing if you accidentally visit any nodes multiple times.

## Valgrind Reports Memory Leak with `make gprof`

If your code was timing out because the runtime was too long, you likely investigated the runtime of the parts of your code using `gprof` (which is excellent!), and in doing so, you likely compiled your code using `make gprof`, which adds the `-pg` compilation flag to the compilation command (the `-pg` compilation flag is required in order to use `gprof`).

However, for reasons out-of-scope of this class, Valgrind doesn't play nicely with the `-pg` flag, and it will (potentially) incorrectly report a memory leak even if your code doesn't actually have a memory leak. Because of this, you will want to remove the `-pg` flag from your compilation command before checking for memory leaks (and before submitting your code, as the grader will be checking for memory leaks). For more information about Valgrind and `-pg`, see [this post](https://stackoverflow.com/a/14565503/2134991).

## Code Works on Example Datasets but Fails Grader

The small example datasets we provide are by no means exhaustive, nor are they intended to be. If your code seems to be working correctly on all of the example datasets we provide but fails the grader, that means that there is a bug in your code that needs to be fixed, and the only way to find the bug is to **design your own test datasets** to try to cover any corner cases you can think of in order to trigger the bug. The ability to design appropriate test datasets is one of the intended learning outcomes of this assignment.