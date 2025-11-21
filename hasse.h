#ifndef HASSE_H
#define HASSE_H

#include "types.h"
#include "tarjan.h"
#include "graph.h"

void removeTransitiveLinks(t_link_array *p_link_array);

int *buildClassTable(TarjanPartition P, int vertexCount);

t_link_array buildHasseLinks(AdjList g, TarjanPartition P, int *classOf);

void exportHasseMermaid(TarjanPartition P, t_link_array A, const char *filename);
void printGraphCharacteristics(TarjanPartition P, t_link_array H);

#endif
