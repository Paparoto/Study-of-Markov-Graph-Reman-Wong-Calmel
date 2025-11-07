#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

char *getId(int num) {
    static char id[8];
    int i = 0;
    num--;
    while (num >= 0) {
        id[i++] = 'A' + (num % 26);
        num = num / 26 - 1;
    }
    id[i] = '\0';
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = id[j];
        id[j] = id[k];
        id[k] = temp;
    }
    return id;
}

void exportMermaid(AdjList g, const char *filename) {
    FILE *f = fopen(filename, "wt");
    if (!f) {
        perror("Could not open output file");
        exit(EXIT_FAILURE);
    }

    fprintf(f, "---\nconfig:\n   layout: elk\n   theme: neo\n   look: neo\n---\n\n");
    fprintf(f, "flowchart LR\n");

    for (int i = 0; i < g.size; i++)
        fprintf(f, "%s((%d))\n", getId(i + 1), i + 1);

    for (int i = 0; i < g.size; i++) {
        Cell *tmp = g.array[i].head;
        while (tmp != NULL) {
            fprintf(f, "%s -->|%.2f|%s\n", getId(i + 1), tmp->proba, getId(tmp->dest));
            tmp = tmp->next;
        }
    }

    fclose(f);
    printf("Mermaid file exported: %s\n", filename);
}
