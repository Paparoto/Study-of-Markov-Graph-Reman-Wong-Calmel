#include <stdio.h>
#include "graph.h"

int main() {
    AdjList g = readGraph("data/exemple1.txt");
    displayAdjList(g);
    return 0;
}
