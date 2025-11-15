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
        for (int j = 0; j < MAX_NODES; j++){
            genes[i][j].vert = -1; genes[i][j].time = 100000; genes[i][j].dest = -1;
            if(j<MAX_SPOTS){
                genes_reserves[i][j].spot = -1;genes_reserves[i][j].time = -1;
            }
        }
        int end = rand()%(MAX_NODES-1)+1;     
        
        
        for (int j = 0; j < end; j++) //観光地を作成
        {
            int spot = rand()%MAX_SPOTS;
            while ((spot == start) || (spot == goal)){spot = rand()%MAX_SPOTS;} //スタートとゴール以外の観光地をランダムに選択
            genes[i][j].vert = spot;
        }
        
        int branches = BRANCHES;
        if (branches > end){branches = end;}
        
        //ブランチの作成
        for (int j = 0; j < branches; j++)
        {
            int spot = rand() % end; //スタート地点からゴールの直前までの中から観光地を選択
            int dest = rand() % (end-spot) + spot; // 選択した観光地より後ろの観光地を選択
            int time = rand() % TIMELIMIT; //出発時刻を選択
            //if(i==24){printf("%d %d %d %d\n", end, spot, dest, time);}
            genes[i][spot].dest = dest;
            genes[i][spot].time = time;
        }

        for (int j = 0; j < MAX_SPOTS; j++)
        {
            genes_reserves[i][j].spot = 0;
            genes_reserves[i][j].time = 0;
        }
        
        for (int j = 0; j < MAX_SPOTS; j++)
        {
            if(1-reserve_rate){break;}
            int r = rand() % 2;
            if (r==1 && spots[genes[i][j].vert].reservable==1)
            {
                genes_reserves[i][j].spot = 1;
                genes_reserves[i][j].time = rand() % TIMELIMIT;
            }
        }
        
        /* index = 0;
        while (index < MAX_RESERVES )
        {
            double r = (double)rand() / RAND_MAX;
            if(reserve_rate<r){index++;continue;}
            int reserving_spot = rand() % MAX_SPOTS-1; //ゴールノードだけは含まない
            for (int j = 0; j < MAX_SPOTS; j++)
            {
                if(genes_reserves[i][reserving_spot].spot == -1){break;}
                reserving_spot+=n;
                if(reserving_spot < 0){r = MAX_SPOTS-2;}
                if(MAX_SPOTS-2 < r){r = 1;}
            }
            int reserved_spot = rand() % (MAX_SPOTS-reserving_spot-1) + reserving_spot+1; //予約ノード～MAX_SPOTSの間でランダム生成したい
            genes_reserves[i][reserving_spot].spot = reserved_spot;
            int t = rand()%(TIMELIMIT);
            genes_reserves[i][reserving_spot].time = t;
            index++;
            n = n * (-1);
        } */
        //出発時間制限の導入
        // とりあえず，等分
    }    
}