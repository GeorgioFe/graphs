# Project Workflow

As mentioned in the overview, as long as (1) you implement all of the functions we declared in `Graph.h`, (2) we can compile the original starter `GraphTest.cpp` via `make`, and (3) we can run `GraphTest` as described, you are absolutely free to implement the project any way you think is best. However, here are some suggestions just in case you feel a bit lost.

# **Potential Development Process**

## *Step 1: Reading the Header Comments*

Before you do **anything**, please read **all** of the header comments in `Graph.h` **thoroughly** to make sure you understand what exactly each function is supposed to do. You should consider the following questions as you investigate each function declared in `Graph.h`:

- What are the parameters of this function? What do they mean? How are they formatted?
- Given the parameters, what is this function supposed to do? What data structure(s) and algorithm(s) would be relevant to achieve this functionality?
- What is the output of this function? What does it mean? How is it formatted?

## *Step 2: Designing Your Graph*

Before you write even a single line of code, completely map out how exactly you want to implement the graph ADT. You should consider the following questions as you think about your unique design:

- What are the graph operations you will need to support?
- What data structure(s) will you use to represent nodes?
- What data structure(s) will you use to represent edges between nodes?
- Are there any helper variables, functions, classes, etc. that you will want to add?
- What are the time complexities of various standard graph operations if you use the data structure(s) you've chosen? Can you modify your choices to speed things up?

I want you to literally **write out _every_ data structure you can use** to represent the graph, along with each data structure's **worst-case Big-O time and space complexities**, and map the functionality of these data structures to the properties of a graph. This design step is by far **the most important step**, as proper design will make your coding much easier and will make your code much faster. Be sure to refer to the ["Summaries of Data Structures" section of the _Data Structures_ Stepik text](https://stepik.org/lesson/31445) to help you brainstorm.

Once you have decided on a design for your graph implementation, you will want to add any instance variables your design will need to the `Graph` class.

## *Step 3: Writing the Constructor*

The first nontrivial code you should write will likely be the `Graph` constructor. You will need to populate the `Graph` using the edges contained within the given edge list CSV file. You should think about the following questions as you implement your constructor:

- How should you initialize each of the instance variables you added to the `Graph` class?
- Do you want to add any helper functions (or perhaps an overloaded constructor) to help you initialize your graph from the given CSV file?

## *Step 4: Implementing Basic Graph Operations*

Once you are confident that your `Graph` constructor is correct, write the functions that access basic properties of a graph: `num_nodes()`, `nodes()`, `num_edges()`, `edge_weight()`, `num_neighbors()`, and `neighbors()`. You should think about the following questions as you implement these functions:

- How can you use the instance variables you've added to the `Graph` class to perform these operations? What would be the worst-case time complexity of each operation?
- Without blowing up memory consumption, can you save the values of basic graph operations as instance variables, rather than computing them from scratch each time?

Once you have implemented all of these functions, you should be able to use the `graph_properties` test in `GraphTest`. Once you have these basic properties working, the remaining functions can be implemented in any order (they will likely be independent of one another), so feel free to deviate from our recommended order if you prefer.

However, *all* of the following steps require your basic graph functionality to work completely correctly. Thus, **do not continue until you are able to get the basic graph functionality completely working!**

## *Step 5: Finding a Unweighted Shortest Path*

In the `shortest_path_unweighted()` function, you will be finding the shortest unweighted path from a given start node to a given end node. You will want to implement this using the [Breadth-First Search (BFS)](https://en.wikipedia.org/wiki/Breadth-first_search) algorithm. Be sure to map out how exactly the algorithm will operate with the specific graph design you have chosen. You should think about the following questions as you implement this:

- What data structure(s) will help you implement BFS, and do they have built-in C++ implementations?
- How will you use the properties of your graph design to facilitate the BFS algorithm?
- What is the worst-case time complexity of your approach? Are there any optimizations you can make to speed things up?

Be sure to refer to the ["Algorithms on Graphs: Breadth First Search" section of the _Data Structures_ Stepik text](https://stepik.org/lesson/28946) for more information about BFS.

## *Step 6: Finding a Weighted Shortest Path*

In the `shortest_path_weighted()` function, you will be finding the shortest weighted path from a given start node to a given end node. You will want to implement this using [Dijkstra's Algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm). Be sure to map out how exactly the algorithm will operate with the specific graph design you have chosen. You should think about the following questions as you implement this:

- What data structure(s) will help you implement Dijkstra's Algorithm, and do they have built-in C++ implementations?
- How will you use the properties of your graph design to facilitate Dijkstra's Algorithm?
- What is the worst-case time complexity of your approach? Are there any optimizations you can make to speed things up?

Be sure to refer to the ["Dijkstra's Algorithm" section of the _Data Structures_ Stepik text](https://stepik.org/lesson/28947) for more information about Dijkstra's algorithm.

## *Step 7: Finding Connected Components*

In an undirected graph, a [connected component](https://en.wikipedia.org/wiki/Component_(graph_theory)) is a subgraph in which any two vertices are connected to each other via some path. Given a graph, you can find all of the components of the graph using the following algorithm:

- Initialize all nodes in the graph to "unvisited"
- While there are still unvisited nodes:
  - Arbitrarily choose one of the remaining unvisited nodes (call it _u_)
  - Perform BFS starting at _u_, and store all nodes visited in the BFS (including _u_) in a set _c_
  - Once BFS is complete, output _c_ as a component of the graph

In the `connected_components()` function, you will be finding all connected components in the graph, but with a small catch: given some threshold `thresh`, you will be ignoring all edges with a weight larger than `thresh`. This shouldn't impact the above algorithm very much: the only distinction is that, each time you perform BFS, you will want to only traverse edges with a weight less than or equal to `thresh`.

## *Step 8: Finding the Smallest Connecting Threshold*

In HIV transmission clustering, a natural question is the following: Given two individuals _u_ and _v_, what is the smallest threshold _d_ such that, if we were to only include edges with weight less than or equal to _d_, there would exist a path connecting _u_ and _v_?

A trivial but horrendously inefficient algorithm to solve this problem is the following:

- Start with a graph with no edges
- For each unique edge weight _w_ in increasing order:
  - Add all edges with weight _w_ to the graph
  - Perform BFS starting at _u_
  - If _v_ is visited in the BFS, return _w_ as the smallest connecting threshold
- If we get here, _u_ and _v_ were never connected, so no such threshold exists

However, we can utilize the [Disjoint Set](https://en.wikipedia.org/wiki/Disjoint-set_data_structure) data structure to speed things up:

- Create a Disjoint Set containing all nodes in the graph, each in their own set
- For each edge (_x_,_y_,_w_) between nodes _x_ and _y_ with weight _w_ in increasing order of edge weight:
  - Perform _union_(_x_,_y_)
  - If _find_(_u_) is equal to _find_(_v_) (meaning _u_ and _v_, the function arguments, are now in the same set), return _w_ as the smallest connecting threshold
- If we get here, _u_ and _v_ were never connected, so no such threshold exists

In the `smallest_connecting_threshold()` function, you will be finding the smallest connecting threshold between a given pair of nodes _u_ and _v_, and you will want to utilize the more efficient algorithm we have described to do so. As such, you will want to implement a helper Disjoint Set class that implements the optimizations discussed in class (e.g. Path Compression and Union-by-Size) to make your code as fast as possible. Be sure to refer to the ["Disjoint Sets" section of the _Data Structures_ Stepik text](https://stepik.org/lesson/30027) for more information about Disjoint Sets.