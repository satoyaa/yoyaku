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
        int end = rand()%(MAX_INITIALIZE_NODES-1)+1;     
        
        
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
            if(end<=1){break;}
            
            
            int spot = rand() % (end-1); //スタート地点からゴールの直前までの中から観光地を選択
            int branchEnd = end;
            for (int k = 0; k < spot; k++)
            {
                if (genes[i][k].dest == -1)
                {
                    continue;
                }
                branchEnd = genes[i][k].dest;
            }
            if(branchEnd-spot < 3){continue;}
            //printf("%d %d %d %d\n", branchEnd, spot);
            int dest = rand() % (branchEnd-(spot+1)) + (spot + 1); // 選択した観光地より後ろの観光地を選択
            int time = TIMELIMIT / (rand() % 6 + 1); //出発時刻を選択
            //printf("%d %d %d %d\n", end, spot, dest, time);
            genes[i][spot].dest = dest;
            genes[i][spot].time = time;
        }

        for (int j = 0; j < MAX_SPOTS; j++)
        {
            genes_reserves[i][j].spot = 0;
            genes_reserves[i][j].time = 0;
        }
        if(1-reserve_rate){break;}
        int r = rand() % MAX_SPOTS;
        if (spots[r].reservable==1)
        {
            genes_reserves[i][r].spot = 1;
            genes_reserves[i][r].time = TIMELIMIT / (rand() % 20 + 1); //予約時刻を選択
        }
        /*
        for (int j = 0; j < MAX_SPOTS; j++)
        {
            if(1-reserve_rate){break;}
            int r = rand() % 2;
            if (r==1 && spots[genes[i][j].vert].reservable==1)
            {
                genes_reserves[i][j].spot = 1;
                genes_reserves[i][j].time = TIMELIMIT / (rand() % 20 + 1); //予約時刻を選択
            }
        }*/
        
    }    
}