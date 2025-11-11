#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extern.h"

void mutation_swap(){
    for (int i = 1; i < POPULATION; i++)
    {
        double r = (double)rand()/RAND_MAX;
        if (mutation_rate < r)
        {
            continue;
        }
        int start = rand()%(MAX_SPOTS-2)+1;
        int goal = rand()%(MAX_SPOTS-2)+1;
        if(goal<start){int t=goal;goal=start;start=t;}
        int temp;
        for (int j = 0; j < (goal-start)/2; j++)
        {
            //観光地の入れ替え
            temp = genes[i][start+j];
            genes[i][start+j] = genes[i][goal-j];
            genes[i][goal-j] = temp;
            //tempにいちいち構造体を持たせるのがめんどくさい
            temp = genes_reserves[i][start+j].spot;
            genes_reserves[i][start+j].spot = genes_reserves[i][goal].spot;
            genes_reserves[i][goal-1].spot = temp;
            temp = genes_reserves[i][start+j].time;
            genes_reserves[i][start+j].time = genes_reserves[i][goal].time;
            genes_reserves[i][goal-1].time = temp;
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