#include "disjoint_set.h"
#include <cstdlib>

// Quick and dirty "visual" demo for a single disjoint set
// of N nodes that forms a chain.
//
// Then a single Find is executed and the resulting graph
// (after the path compression) is printed as a DOT diagram.
//
// Run the demo as:
//
//  ./demo N | dot -Tpng > result.png
//
// Note: to change the compression strategy you need to modify
// the template parameter and recompile
int main(int argc, char* argv[]) {
    if (argc != 2)
        return -1;

    unsigned n = atoi(argv[1]);
    DisjointSets<LeftIntoRight, ReversalK2> sets(n);

    for (unsigned i = 0; i < n-1; ++i) {
        sets.Union(i, i+1);
    }

    sets.Find(0);

    sets.as_dot_graph(std::cout);

    return 0;
}
