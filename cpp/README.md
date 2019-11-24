## C++ graph bridges founder

### Requirements

Your system should have installed:

* Clang 11
* CMake 3.15

### Running

1. Compile using CMake + gcc
2. Run compiled binary file with parameters

`./graph <MAX_NUMBER_OF_GRAPH_VERTICES> <EDGES_PROBABILITY>` where 

- `<MAX_NUMBER_OF_GRAPH_VERTICES>` - maximum amount of graph vertices
- `<EDGES_PROBABILITY>` - probability for each V, U that (V, U) is edge

This program will generate graphs with different numbers of vertices starting from **10** vertices ending `<MAX_NUMBER_OF_GRAPH_VERTICES>` with step **10**, 
i.e `{10, 20, 30, ..., 1000}` if `<MAX_NUMBER_OF_GRAPH_VERTICES>` is **1000** with `<EDGES_PROBABILITY>` 
for each V, U that (V, U) is edge

The code will generate some graphs, find bridges on them using several methods 
(deterministic, randomized using DFS, randomized using std::sort, bucket sort, radix sort) 
and create result files which are described in a section below

### Output

The following results will be given

- `edges_<NUMBER_OF_GRAPH_VERTICES>_<EDGE_GENERATION_PROBABILITY>.csv` - file which contains all generated graph edges **(\*)**
- `basic_<NUMBER_OF_GRAPH_VERTICES>_<EDGE_GENERATION_PROBABILITY>.csv` - the only those edges from **(\*)** that were given by DFS
- `one_bridges_<NUMBER_OF_GRAPH_VERTICES>_<EDGE_GENERATION_PROBABILITY>.csv` - the only those edges from **(\*)** that are 1-bridges
- `two_bridges_<NUMBER_OF_GRAPH_VERTICES>_<EDGE_GENERATION_PROBABILITY>.csv` - the only those edges from **(\*)** that are 2-bridges
- `result_<EDGE_GENERATION_PROBABILITY>.csv` - execution time by each method (deterministic, randomized using std::sort, bucket sort, radix sort)