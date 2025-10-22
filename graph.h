#ifndef GRAPH_H
#define GRAPH_H

typedef struct cell {
    int dest;
    float proba;
    struct cell *next;
} Cell;

typedef struct list {
    Cell *head;
} List;

typedef struct adjacency_list {
    int size;
    List *array;
} AdjList;

Cell *createCell(int dest, float proba);
List createEmptyList(void);
void addCell(List *list, int dest, float proba);
void displayList(List list);
AdjList createAdjList(int size);
void displayAdjList(AdjList g);
AdjList readGraph(const char *filename);

#endif
