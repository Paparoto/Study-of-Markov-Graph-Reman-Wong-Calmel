#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tarjan.h"

TarjanVertex *initTarjanVertices(AdjList g) {
    TarjanVertex *v = malloc(g.size * sizeof(TarjanVertex));
    for (int i = 0; i < g.size; i++) {
        v[i].id = i + 1;
        v[i].num = -1;
        v[i].low = -1;
        v[i].onStack = 0;
    }
    return v;
}

void push(int *stack, int *top, int value) {
    stack[++(*top)] = value;
}

int pop(int *stack, int *top) {
    return stack[(*top)--];
}

int stackContains(int *stack, int top, int value) {
    for (int i = 0; i <= top; i++)
        if (stack[i] == value)
            return 1;
    return 0;
}

void parcours(int u, AdjList g, TarjanVertex *tv, int *index,
              int *stack, int *top, TarjanPartition *P) {

    tv[u].num = *index;
    tv[u].low = *index;
    (*index)++;

    push(stack, top, u);
    tv[u].onStack = 1;

    Cell *c = g.array[u].head;
    while (c) {
        int v = c->dest - 1;

        if (tv[v].num == -1) {
            parcours(v, g, tv, index, stack, top, P);
            if (tv[v].low < tv[u].low)
                tv[u].low = tv[v].low;
        }
        else if (tv[v].onStack && tv[v].num < tv[u].low)
            tv[u].low = tv[v].num;

        c = c->next;
    }

    if (tv[u].low == tv[u].num) {
        P->classes = realloc(P->classes, (P->count + 1) * sizeof(TarjanClass));
        TarjanClass *cls = &P->classes[P->count];
        P->count++;

        cls->count = 0;
        cls->vertices = NULL;
        sprintf(cls->name, "C%d", P->count);

        int v;
        do {
            v = pop(stack, top);
            tv[v].onStack = 0;

            cls->vertices = realloc(cls->vertices, (cls->count + 1) * sizeof(int));
            cls->vertices[cls->count++] = v + 1;

        } while (v != u);
    }
}

TarjanPartition tarjan(AdjList g) {
    TarjanPartition P;
    P.classes = NULL;
    P.count = 0;

    TarjanVertex *tv = initTarjanVertices(g);
    int index = 0;

    int *stack = malloc(g.size * sizeof(int));
    int top = -1;

    for (int i = 0; i < g.size; i++)
        if (tv[i].num == -1)
            parcours(i, g, tv, &index, stack, &top, &P);

    free(stack);
    free(tv);
    return P;
}
