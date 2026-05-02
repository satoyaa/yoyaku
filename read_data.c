#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extern.h"

void read_data(const char * filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("I can't open such a file");
    }

    int nodeCount = 0;
    char line[512];

    while (fgets(line, sizeof(line), file)) {
        // 改行除去
        line[strcspn(line, "\r\n")] = 0;

        int vert;
        int value;
        int kind;
        int t;
        int crow;
        int capacaity;
        int cost;
        double coordinate_x;
        double coordinate_y;
        int reservable;
        int penalty;

        int n = sscanf(line, "%d %d %d %d %d %d %d %lf %lf %d %d",
                       &vert, &value, &kind, &t, &crow, &capacaity, &cost,
                       &coordinate_x, &coordinate_y,
                       &reservable, &penalty);

        if (n == 11) {
            spots[nodeCount].vert = vert;
            spots[nodeCount].value = value;
            spots[nodeCount].kind = kind;
            spots[nodeCount].t = t;
            spots[nodeCount].crow = crow;
            spots[nodeCount].capacity = capacaity;
            spots[nodeCount].cost = cost;
            spots[nodeCount].coordinate_x = coordinate_x;
            spots[nodeCount].coordinate_y = coordinate_y;
            spots[nodeCount].reservable = reservable;
            spots[nodeCount].penalty = penalty;
            nodeCount++;
            if (MAX_SPOTS<=nodeCount)
            {
                break;
            }
        }else {
            // 行フォーマットが合わない場合のデバッグ出力
            printf(" (%d): %s\n", n, line);
        }
    }

    fclose(file);   
}
