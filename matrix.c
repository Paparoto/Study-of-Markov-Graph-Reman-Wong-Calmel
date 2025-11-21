#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"

t_matrix createEmptyMatrix(int rows, int cols) {
    t_matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = malloc(rows * sizeof(float *));
    for (int i = 0; i < rows; i++) {
        mat.data[i] = calloc(cols, sizeof(float));
    }
    return mat;
}

t_matrix createTransitionMatrix(AdjList g) {
    t_matrix mat = createEmptyMatrix(g.size, g.size);
    
    for (int i = 0; i < g.size; i++) {
        Cell *curr = g.array[i].head;
        while (curr != NULL) {
            mat.data[i][curr->dest - 1] = curr->proba;
            curr = curr->next;
        }
    }
    return mat;
}

void freeMatrix(t_matrix mat) {
    for (int i = 0; i < mat.rows; i++) {
        free(mat.data[i]);
    }
    free(mat.data);
}

void displayMatrix(t_matrix mat) {
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            printf("%.2f\t", mat.data[i][j]);
        }
        printf("\n");
    }
}

void copyMatrix(t_matrix dest, t_matrix src) {
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            dest.data[i][j] = src.data[i][j];
        }
    }
}

void multiplyMatrices(t_matrix m1, t_matrix m2, t_matrix result) {
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m2.cols; j++) {
            result.data[i][j] = 0;
            for (int k = 0; k < m1.cols; k++) {
                result.data[i][j] += m1.data[i][k] * m2.data[k][j];
            }
        }
    }
}

float matrixDiff(t_matrix m1, t_matrix m2) {
    float diff = 0.0f;
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            diff += fabs(m1.data[i][j] - m2.data[i][j]);
        }
    }
    return diff;
}

t_matrix subMatrix(t_matrix matrix, TarjanPartition part, int compo_index) {
    int size = part.classes[compo_index].count;
    t_matrix sub = createEmptyMatrix(size, size);
    int *vertices = part.classes[compo_index].vertices;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int u = vertices[i] - 1;
            int v = vertices[j] - 1;
            sub.data[i][j] = matrix.data[u][v];
        }
    }
    return sub;
}
int gcd_array(int *vals, int nbvals) {
    if (nbvals == 0) return 0;
    int result = vals[0];
    for (int i = 1; i < nbvals; i++) {
        int a = result;
        int b = vals[i];
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        result = a;
    }
    return result;
}

int getPeriod(t_matrix sub_matrix) {
    int n = sub_matrix.rows;
    int *periods = malloc(n * n * sizeof(int));
    int period_count = 0;

    t_matrix power = createEmptyMatrix(n, n);
    t_matrix temp = createEmptyMatrix(n, n);

    copyMatrix(power, sub_matrix);

    for (int k = 1; k <= n + 2; k++) {
        int diag_nonzero = 0;
        for (int i = 0; i < n; i++) {
            if (power.data[i][i] > 0.00001f) {
                diag_nonzero = 1;
                break;
            }
        }

        if (diag_nonzero) {
            periods[period_count++] = k;
        }

        multiplyMatrices(power, sub_matrix, temp);
        copyMatrix(power, temp);
    }

    int d = 0;
    if (period_count > 0) {
        d = gcd_array(periods, period_count);
    }

    free(periods);
    freeMatrix(power);
    freeMatrix(temp);
    return d;
}