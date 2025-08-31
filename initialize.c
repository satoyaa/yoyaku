#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "extern.h"


void initialize(int start, int goal){
    for (int i = 0; i < POPULATION; i++)
    {
        int index = 0;
        int n = 1;
        for (int j = 0; j < MAX_SPOTS; j++){genes[i][j] = -1;genes_reserves[i][j]= -1;}
        genes[i][0]=start;

        
        while (index < MAX_SPOTS )
        {
            if(index==start || index == goal){index++;continue;}
            int r = rand()%(MAX_SPOTS-2)+1;
            for (int j = 0; j < MAX_SPOTS; j++)
            {
                if(genes[i][r] == -1){break;}
                r+=n;
                if(r < 1){r = MAX_SPOTS-2;}
                if(MAX_SPOTS-2 < r){r = 1;}
            }
            genes[i][r]= index;
            index++;
            n = n * (-1);
        }
        genes[i][MAX_SPOTS-1]=goal;   
        // 予約確認ノードの導入
        index = 0;
        while (index < MAX_RESERVES )
        {
            double r = (double)rand() / RAND_MAX;
            if(reserve_rate<r){index++;continue;}
            int reserving_spot = rand() % MAX_SPOTS-1; //ゴールノードだけは含まない
            for (int j = 0; j < MAX_SPOTS; j++)
            {
                if(genes_reserves[i][reserving_spot] == -1){break;}
                reserving_spot+=n;
                if(reserving_spot < 0){r = MAX_SPOTS-2;}
                if(MAX_SPOTS-2 < r){r = 1;}
            }
            int reserved_spot = rand() % (MAX_SPOTS-reserving_spot) + reserving_spot + 1; //予約ノード～MAX_SPOTSの間でランダム生成したい
            genes_reserves[i][reserving_spot] = reserved_spot;
            index++;
            n = n * (-1);
        }
    }    
}