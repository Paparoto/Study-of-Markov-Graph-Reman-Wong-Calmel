#include <stdio.h>
#include "graph.h"

int main() {
    AdjList g = readGraph("data/exemple3.txt");
    displayAdjList(g);
    isMarkovGraph(g);
    return 0;
}
