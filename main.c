#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "tarjan.h"
#include "hasse.h"
#include "matrix.h"

int main() {
    // --- STEP 1 & 2 (METEO) ---
    char *filename = "data/exemple_meteo.txt";
    AdjList g = readGraph(filename);

    printf("--- PART 1 & 2 SUMMARY ---\n");
    isMarkovGraph(g);
    TarjanPartition P = tarjan(g);
    int *classOf = buildClassTable(P, g.size);
    t_link_array H = buildHasseLinks(g, P, classOf);
    removeTransitiveLinks(&H);
    printGraphCharacteristics(P, H);


    printf("\n--- PART 3: MATRIX CALCULATIONS ---\n");

    t_matrix M = createTransitionMatrix(g);
    t_matrix Mn = createEmptyMatrix(g.size, g.size);
    t_matrix M_pow = createEmptyMatrix(g.size, g.size);
    t_matrix M_prev = createEmptyMatrix(g.size, g.size);

    copyMatrix(Mn, M);
    int target_powers[] = {3, 7};
    int current_power = 1;

    for (int k = 0; k < 2; k++) {
        int target = target_powers[k];
        while (current_power < target) {
            multiplyMatrices(Mn, M, M_pow);
            copyMatrix(Mn, M_pow);
            current_power++;
        }
        printf("\nMatrix M^%d:\n", current_power);
        displayMatrix(Mn);
    }

    copyMatrix(Mn, M);
    current_power = 1;
    float diff = 1.0f;
    while (diff >= 0.01f && current_power < 1000) {
        copyMatrix(M_prev, Mn);
        multiplyMatrices(M_prev, M, Mn);
        current_power++;
        diff = matrixDiff(Mn, M_prev);
    }
    if (diff < 0.01f) {
        printf("\nConverged at n = %d with diff = %f\n", current_power, diff);
    }

    freeMatrix(M);
    freeMatrix(Mn);
    freeMatrix(M_pow);
    freeMatrix(M_prev);
    free(classOf);


    // --- BONUS STEP: PERIODICITY ---
    printf("\n=============================================\n");
    printf("--- PART 3 - BONUS: PERIODICITY ---\n");
    printf("(Using exemple_periodic.txt)\n");

    char *filePer = "data/exemple_periodic.txt";
    AdjList gPer = readGraph(filePer);
    TarjanPartition PPer = tarjan(gPer);
    t_matrix MPer = createTransitionMatrix(gPer);

    for (int i = 0; i < PPer.count; i++) {
        printf("\nClass %s: {", PPer.classes[i].name);
        for (int j = 0; j < PPer.classes[i].count; j++) {
            printf("%d", PPer.classes[i].vertices[j]);
        }
        printf("}\n");

        t_matrix sub = subMatrix(MPer, PPer, i);
        int period = getPeriod(sub);

        printf("Period = %d\n", period);
        if (period > 1) printf("Class is PERIODIC.\n");
        else printf("Class is APERIODIC.\n");

        freeMatrix(sub);
    }

    freeMatrix(MPer);

    return 0;
}