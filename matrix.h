#ifndef MATRIX_H
#define MATRIX_H

#include "graph.h"
#include "tarjan.h"

typedef struct {
    int rows;
    int cols;
    float **data;
} t_matrix;

t_matrix createEmptyMatrix(int rows, int cols);
t_matrix createTransitionMatrix(AdjList g);
void freeMatrix(t_matrix mat);
void displayMatrix(t_matrix mat);
void copyMatrix(t_matrix dest, t_matrix src);
void multiplyMatrices(t_matrix m1, t_matrix m2, t_matrix result);
float matrixDiff(t_matrix m1, t_matrix m2);
t_matrix subMatrix(t_matrix matrix, TarjanPartition part, int compo_index);
int getPeriod(t_matrix sub_matrix);

#endif