#include <stdio.h>
#include "graph.h"

int main() {
    AdjList g = readGraph("data/exemple1.txt");
    displayAdjList(g);
    isMarkovGraph(g);
    exportMermaid(g, "data/exemple1_mermaid.txt");
    return 0;
}
