#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "extern.h"

int makenode(){
    FILE *fp;
    int rows = 30;   // 行数
    int i, j;
    int vert;
    int value;
    int kind;
    int t;
    int crow;
    int cost;
    float coordinate_x;
    float coordinate_y;
    int reservable;
    int reserve;
    int penalty;

    // 乱数の初期化
    srand((unsigned int)time(NULL));

    // ファイルを開く（書き込みモード）
    fp = fopen("output.txt", "w");
    if (fp == NULL) {
        perror("ファイルオープン失敗");
        return 1;
    }

    // データ生成＆保存
    for (i = 0; i < rows; i++) {
        vert = i;
        value = rand() % 100;
        kind = rand() % 5;
        t = (rand() % 10 + 2) * 5;
        crow = rand() % 20;
        cost = (rand() % 30) * 100;
        coordinate_x = (float)rand() / RAND_MAX * 500;
        coordinate_y = (float)rand() / RAND_MAX * 500;
        reservable = rand() % 2;
        reserve = 0;
        penalty = (rand() % 400) + 100;
        fprintf(fp, "%d %d %d %d %d %d %f %f %d %d %d", vert, value, kind, t, crow, cost, coordinate_x, coordinate_y, reservable, reserve, penalty);
        fprintf(fp, "\n");
    }

    // ファイルを閉じる
    fclose(fp);

    printf("output.txt done\n");
    return(0);
}

