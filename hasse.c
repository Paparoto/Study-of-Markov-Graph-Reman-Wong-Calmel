//#include <malloc.h>

#include "types.h"
#include <stdlib.h>
#include "hasse.h"
#include "tarjan.h"
#include "graph.h"
#include <stdio.h>

void removeTransitiveLinks(t_link_array *p_link_array)
{
    int i = 0;
    while (i < p_link_array->log_size)
    {
        t_link link1 = p_link_array->links[i];
        int j = 0;
        int to_remove = 0;
        while (j < p_link_array->log_size && !to_remove)
        {
            if (j != i)
            {
                t_link link2 = p_link_array->links[j];
                if (link1.from == link2.from)
                {
                    // look for a link from link2.to to link1.to
                    int k = 0;
                    while (k < p_link_array->log_size && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = p_link_array->links[k];
                            if ((link3.from == link2.to) && (link3.to == link1.to))
                            {
                                to_remove = 1;
                            }
                        }
                        k++;
                    }
                }
            }
            j++;
        }
        if (to_remove)
        {
            // remove link1 by replacing it with the last link
            p_link_array->links[i] = p_link_array->links[p_link_array->log_size - 1];
            p_link_array->log_size--;
        }
        else
        {
            i++;
        }
    }
}


int *buildClassTable(TarjanPartition P, int vertexCount) {
    int *classOf = malloc((vertexCount + 1) * sizeof(int));
    for (int i = 1; i <= vertexCount; i++)
        classOf[i] = -1;

    for (int c = 0; c < P.count; c++)
        for (int j = 0; j < P.classes[c].count; j++)
            classOf[P.classes[c].vertices[j]] = c;

    return classOf;
}

t_link_array buildHasseLinks(AdjList g, TarjanPartition P, int *classOf) {
    t_link_array A;
    A.links = NULL;
    A.log_size = 0;
    A.capacity = 0;

    for (int i = 0; i < g.size; i++) {
        int Ci = classOf[i + 1];
        Cell *c = g.array[i].head;

        while (c) {
            int Cj = classOf[c->dest];
            if (Ci != Cj) {
                int exists = 0;
                for (int k = 0; k < A.log_size; k++)
                    if (A.links[k].from == Ci && A.links[k].to == Cj)
                        exists = 1;

                if (!exists) {
                    A.links = realloc(A.links, (A.log_size + 1) * sizeof(t_link));
                    A.links[A.log_size].from = Ci;
                    A.links[A.log_size].to = Cj;
                    A.log_size++;
                }
            }
            c = c->next;
        }
    }

    removeTransitiveLinks(&A);

    return A;
}

void exportHasseMermaid(TarjanPartition P, t_link_array A, const char *filename) {
    FILE *f = fopen(filename, "wt");
    if (!f) return;

    fprintf(f, "flowchart TD\n");

    for (int c = 0; c < P.count; c++) {
        fprintf(f, "%s[ ", P.classes[c].name);
        for (int j = 0; j < P.classes[c].count; j++) {
            fprintf(f, "%d", P.classes[c].vertices[j]);
            if (j < P.classes[c].count - 1) fprintf(f, ",");
        }
        fprintf(f, " ]\n");
    }

    for (int i = 0; i < A.log_size; i++) {
        int from = A.links[i].from;
        int to = A.links[i].to;
        fprintf(f, "%s --> %s\n", P.classes[from].name, P.classes[to].name);
    }

    fclose(f);
}

void printGraphCharacteristics(TarjanPartition P, t_link_array H) {
    if (P.count == 1) {
        printf("The graph is IRREDUCIBLE\n");
    } else {
        printf("The graph is NOT irreducible\n");
    }

    int *is_transient = calloc(P.count, sizeof(int));

    for (int i = 0; i < H.log_size; i++) {
        is_transient[H.links[i].from] = 1;
    }

    for (int i = 0; i < P.count; i++) {
        printf("Class %s: ", P.classes[i].name);

        if (is_transient[i]) {
            printf("Transient\n");
        } else {
            printf("Persistent");
            if (P.classes[i].count == 1) {
                printf(" (State %d is ABSORBING)", P.classes[i].vertices[0]);
            }
            printf("\n");
        }
    }

    free(is_transient);
}
