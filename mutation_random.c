#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extern.h"

void mutation_random(){
    for (int i = 1; i < POPULATION; i++)
    {
        double r = (double)rand()/RAND_MAX;
        if (mutation_rate < r)
        {
            continue;
        }
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
            int index = rand() % (length-1);
            int vert = rand() % MAX_SPOTS;
            int time = TIMELIMIT / (rand() % 6 + 1); //分岐時刻を選択
            int dest = rand() % (length-(index+1)) + (index+1); // 選択した観光地より後ろの観光地を選択
            genes[i][index].vert = vert;
            genes[i][index].time = time;
            genes[i][index].dest = dest;
            if(1-reserve_rate){continue;}
            int r = rand() % 2;
            int index_reserve = rand() % MAX_SPOTS;
            if (r==1 && spots[genes[i][index].vert].reservable==1)
            {
                genes_reserves[i][index_reserve].spot = 1-genes_reserves[i][index_reserve].spot;
                genes_reserves[i][index_reserve].time = TIMELIMIT / (rand() % 6 + 1); //予約時刻を選択
            }
        }
    }
    
    
}