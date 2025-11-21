#ifndef TARJAN_H
#define TARJAN_H

#include "graph.h"

typedef struct {
    int id;
    int num;
    int low;
    int onStack;
} TarjanVertex;

typedef struct {
    int *vertices;
    int count;
    char name[8];
} TarjanClass;

typedef struct {
    TarjanClass *classes;
    int count;
} TarjanPartition;

TarjanVertex *initTarjanVertices(AdjList g);
void freeTarjanVertices(TarjanVertex *v);

void push(int *stack, int *top, int value);
int pop(int *stack, int *top);
int stackContains(int *stack, int top, int value);

TarjanPartition tarjan(AdjList g);

#endif
