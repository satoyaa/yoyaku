#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extern.h"

void mutation_random(){
    for (int i = 1; i < POPULATION; i++)
    {
        
        //遺伝子長を計算
        int length = 0;
        for (int j = 0; j < MAX_NODES; j++)
        {
            if(genes[i][j].vert==-1){break;}
            length+=1;
        }
        if (length <= 2)
        {
            continue;
        }
        //ランダム突然変異の実装
        int changes = 5;
        for (int j = 0; j < changes; j++)
        {
            if (1<=length)
            {
                break;
            }
            int index = 0;
            double r;
            if (mutation_rateR < r)
            {
                r = (double)rand()/RAND_MAX;
                continue;
            }
            int vert = rand()%MAX_SPOTS;
            r = (double)rand()/RAND_MAX;
            genes[i][index].vert = vert;
            if (mutation_rateR < r)
            {
                continue;
            }
            if(1-reserve_rate){continue;}
            int index_reserve = rand() % MAX_SPOTS;
            if (spots[genes[i][index].vert].reservable==1)
            {
                genes_reserves[i][index_reserve].spot = 1-genes_reserves[i][index_reserve].spot;
                if(genes_reserves[i][index_reserve].spot==0){genes_reserves[i][index_reserve].time=0;}
                //genes_reserves[i][index_reserve].time = TIMELIMIT / (rand() % 6 + 1); //予約時刻を選択
                genes_reserves[i][index_reserve].time = (rand() % (TIMELIMIT/15))*15;; //予約時刻を選択
            }
        }
    }
    
    
}