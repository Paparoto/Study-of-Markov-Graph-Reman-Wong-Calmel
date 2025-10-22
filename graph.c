#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graph.h"

Cell *createCell(int dest, float proba) {
    Cell *newCell = (Cell *)malloc(sizeof(Cell));
    if (!newCell) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newCell->dest = dest;
    newCell->proba = proba;
    newCell->next = NULL;
    return newCell;
}

List createEmptyList(void) {
    List l;
    l.head = NULL;
    return l;
}

void addCell(List *list, int dest, float proba) {
    Cell *newCell = createCell(dest, proba);
    newCell->next = list->head;
    list->head = newCell;
}

void displayList(List list) {
    Cell *tmp = list.head;
    while (tmp != NULL) {
        printf(" -> (%d, %.2f)", tmp->dest, tmp->proba);
        tmp = tmp->next;
    }
    printf("\n");
}

AdjList createAdjList(int size) {
    AdjList g;
    g.size = size;
    g.array = (List *)malloc(size * sizeof(List));
    if (!g.array) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size; i++) {
        g.array[i] = createEmptyList();
    }
    return g;
}

void displayAdjList(AdjList g) {
    for (int i = 0; i < g.size; i++) {
        printf("Vertex %d:", i + 1);
        displayList(g.array[i]);
    }
}

AdjList readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (!file) {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }

    int nbvert, start, end;
    float proba;
    if (fscanf(file, "%d", &nbvert) != 1) {
        perror("Error reading number of vertices");
        exit(EXIT_FAILURE);
    }

    AdjList g = createAdjList(nbvert);

    while (fscanf(file, "%d %d %f", &start, &end, &proba) == 3) {
        addCell(&g.array[start - 1], end, proba);
    }

    fclose(file);
    return g;
}

int isMarkovGraph(AdjList g) {
    int ok = 1;
    for (int i = 0; i < g.size; i++) {
        float sum = 0;
        Cell *tmp = g.array[i].head;
        while (tmp != NULL) {
            sum += tmp->proba;
            tmp = tmp->next;
        }
        if (sum < 0.99 || sum > 1.01) {
            printf("The sum of probabilities of vertex %d is %.2f\n", i + 1, sum);
            ok = 0;
        }
    }
    if (ok)
        printf("The graph is a Markov graph\n");
    else
        printf("The graph is not a Markov graph\n");
    return ok;
}
