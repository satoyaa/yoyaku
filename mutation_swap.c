#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extern.h"

void mutation_swap(){
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
        int start = 0;
        for (int j = 0; j < length; j++)
        {
            double r = (double)rand()/RAND_MAX;
            if (mutation_rateS < r)
            {
                start = j;
                break;
            }
        }
        
        int goal = rand()%length;
        if(goal<start){int t=goal;goal=start;start=t;}
        int temp;
        //printf("%d %d %d\n",length, goal, start);
        for (int j = 0; j < (goal-start)/2; j++)
        {
            //観光地の入れ替え
            temp = genes[i][start+j].vert;
            genes[i][start+j].vert = genes[i][goal-j].vert;
            genes[i][goal-j].vert = temp;
        }
        //予約観光地の反転
        start = rand()%MAX_SPOTS;
        goal = rand()%MAX_SPOTS;
        if(goal<start){int t=goal;goal=start;start=t;}
        //printf("%d %d %d\n",length, goal, start);
        for (int j = 0; j < (goal-start)/2; j++)
        {
            if(1-reserve_rate){break;}
            temp = genes_reserves[i][start+j].spot;
            genes_reserves[i][start+j].spot = genes_reserves[i][goal-j].spot;
            genes_reserves[i][goal-j].spot = temp;
            temp = genes_reserves[i][start+j].time;
            genes_reserves[i][start+j].time = genes_reserves[i][goal-j].time;
            genes_reserves[i][goal-j].time = temp;
            if (spots[start+j].reservable==0)
            {
                
                genes_reserves[i][start+j].spot=0;
                genes_reserves[i][start+j].time=0;
            }
            if (spots[start+j].reservable==0)
            {
                genes_reserves[i][goal-j].spot=0;
                genes_reserves[i][goal-j].time=0;
            }
        }


        /* //予約の整合性を修正する．
        for (int j = 0; j < MAX_SPOTS; j++)
        {
            //予約観光地を探す
            if(genes_reserves[i][j].spot==-1){continue;}
            int reserve_spot = genes_reserves[i][j].spot;
            //予約確認観光地より先に予約予定観光地の予約をしてる場合，予約予定観光地と予約確認観光地の関係を入れ替える
            //
            if(reserve_spot < j){
                genes_reserves[i][reserve_spot].spot=j;
                genes_reserves[i][j].spot=-1;
                genes_reserves[i][reserve_spot].time = genes_reserves[i][j].time;
                genes_reserves[i][j].time=-1;
            }       
        } */
    }
    
    
}