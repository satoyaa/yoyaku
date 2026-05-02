#include <stdio.h>
/*
 突然変異: swap 変異
 - 個体ごとに確率に応じて突然変異を実行する。
 - 遺伝子列内の任意区間を選び、その区間内の観光地（頂点）を反転または入れ替える。
 - 予約情報の整合性を保つための処理（必要に応じて有効化可能）が含まれている。
 - 長さが短い個体や確率判定でスキップされる場合がある。
*/
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
        int start = rand()%length;
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