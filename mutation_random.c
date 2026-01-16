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
        int changes = rand() % length; // 変化させる遺伝子の数を決定
        for (int j = 0; j < changes; j++)
        {
            if (1<=length)
            {
                break;
            }
            int index = 0;
            double r;
            for (int k = 0; k < length-1; k++)
            {
                r = (double)rand()/RAND_MAX;
                if (mutation_rateR < r)
                {
                    index = k;
                    break;
                }
            }
            int vert = 1;
            for (int k = 1; k < MAX_SPOTS-1; k++)
            {
                r = (double)rand()/RAND_MAX;
                if (mutation_rateR < r)
                {
                    vert = k;
                    break;
                }
            }
            genes[i][index].vert = vert;
            if(1-reserve_rate){continue;}
            int random = rand() % 2;
            int index_reserve = rand() % MAX_SPOTS;
            if (random==1 && spots[genes[i][index].vert].reservable==1)
            {
                genes_reserves[i][index_reserve].spot = 1-genes_reserves[i][index_reserve].spot;
                if(genes_reserves[i][index_reserve].spot==0){genes_reserves[i][index_reserve].time=0;}
                genes_reserves[i][index_reserve].time = TIMELIMIT / (rand() % 6 + 1); //予約時刻を選択
            }
        }
    }
    
    
}