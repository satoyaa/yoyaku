#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "extern.h"

int mai(){
    FILE *fp;
    int rows = MAX_SPOTS;   // 行数
    int i;
    int vert;
    int value;
    int kind;
    int t; //除算を行うため0<capacity
    int crow;
    int capacity; //除算を行うため0<capacity
    int cost;
    float coordinate_x;
    float coordinate_y;
    int reservable;
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
        value = (rand() % 3 + 1)*10;
        kind = rand() % 5;
        t = (rand() % 3 + 1) * 30;
        crow = capacity + rand() % 20 - 5;           //<====ここだけ順番逆(座席数の+15～-5人)
        capacity = rand() %20 + 10;   //<====ここだけ順番逆
        cost = (rand() % 30) * 100;
        coordinate_x = (float)rand() / RAND_MAX * 500;
        coordinate_y = (float)rand() / RAND_MAX * 500;
        reservable = rand() % 2;
        if (reservable)
        {
            /* code */
        }
        
        penalty = (rand() % 40) + 10;
        if(i==0){t=1,crow=0;capacity=1;}
        fprintf(fp, "%d %d %d %d %d %d %d %f %f %d %d", vert, value, kind, t, crow, capacity, cost, coordinate_x, coordinate_y, reservable, penalty);
        fprintf(fp, "\n");
    }

    // ファイルを閉じる
    fclose(fp);

    printf("output.txt done\n");
    return(0);
}

