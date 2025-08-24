#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extern.h"

void readdata(const char * filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("ファイルを開けませんでした");
    }

    
    int nodeCount = 0;
    char line[512];
    int reading_coords = 0;

    while (fgets(line, sizeof(line), file)) {
        // 改行除去
        line[strcspn(line, "\r\n")] = 0;

        /*
        if (strcmp(line, "NODE_COORD_SECTION") == 0) {
            printf("hello0");
            reading_coords = 1;
            continue;
        }
        if (strcmp(line, "EOF") == 0) {
            break;
        }*/
        reading_coords = 1;


        int vert;
        int value;
        int kind;
        int t;
        int crow;
        int cost;
        double coordinate_x;
        double coordinate_y;
        int reservable;
        int reserve;
        int penalty;

        int n = sscanf(line, "%d %d %d %d %d %d %lf %lf %d %d %d",
                       &vert, &value, &kind, &t, &crow, &cost,
                       &coordinate_x, &coordinate_y,
                       &reservable, &reserve, &penalty);

        if (n == 11) {
            spots[nodeCount].vert = vert;
            spots[nodeCount].value = value;
            spots[nodeCount].kind = kind;
            spots[nodeCount].t = t;
            spots[nodeCount].crow = crow;
            spots[nodeCount].cost = cost;
            spots[nodeCount].coordinate_x = coordinate_x;
            spots[nodeCount].coordinate_y = coordinate_y;
            spots[nodeCount].reservable = reservable;
            spots[nodeCount].reserve = reserve;
            spots[nodeCount].penalty = penalty;
            nodeCount++;
        }else {
            // 行フォーマットが合わない場合のデバッグ出力
            printf(" (%d): %s\n", n, line);
        }
        
    }

    fclose(file);   
}