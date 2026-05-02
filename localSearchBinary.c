#include <stdio.h>
/*
 局所探索: Binary（3点比較による二分探索風探索）
 - 各予約観光地について，早め・中間・遅め の3候補時刻を作成し評価することで，
     最適な予約時刻に向けて範囲を二分して収束させる手法を実装している。
 - 範囲が十分小さくなるまで繰り返し，`calculate_fitness` により良い候補を選択する。
 - 速度と局所解改善のバランスを取るための軽量な局所探索アルゴリズム。
*/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "extern.h"

//局所探索の実装
//終了時刻までに訪問できる観光地の数を増やす
void local_search_binary(int start, int goal){
    printf("local search start: ");
    if (1-reserve_rate)
    {
        return;
    }
    
    //遺伝子のコピー
    for (int i = 0; i < MAX_NODES; i++)
    {
        genes[1][i].vert = genes[0][i].vert;
        genes[1][i].time = genes[0][i].time;
        genes[1][i].dest = genes[0][i].dest;
        genes[2][i].vert = genes[0][i].vert;
        genes[2][i].time = genes[0][i].time;
        genes[2][i].dest = genes[0][i].dest;
        if(i<MAX_SPOTS){
            genes_reserves[1][i].spot = genes_reserves[0][i].spot;
            genes_reserves[1][i].time = genes_reserves[0][i].time;
            genes_reserves[2][i].spot = genes_reserves[0][i].spot;
            genes_reserves[2][i].time = genes_reserves[0][i].time;
        }
    }
    //局所探索の実装
    local_search_mode = 1;
    for (int i = 0; i < MAX_SPOTS; i++)
    {
        if (genes_reserves[0][i].spot==0)
        {
            continue;
        }
        int range = TIMELIMIT/2;
        int bestTime = TIMELIMIT/2;
        while (1)
        {
            if (range<5)
            {
                break;
            }         
            genes_reserves[0][i].time = bestTime-range;   
            genes_reserves[1][i].time = bestTime;
            genes_reserves[2][i].time = bestTime+range;
            if (bestTime-range<0)
            {
                genes_reserves[0][i].time=0;
            }
            if (bestTime+range>TIMELIMIT)
            {
                genes_reserves[2][i].time=TIMELIMIT;
            }
            calculate_fitness(start, goal);
            if (fitness[0]<=fitness[1] && fitness[2]<=fitness[1])
            {   
                bestTime = bestTime;
                
            }
            else if (fitness[1]<fitness[2] && fitness[0]<fitness[2])
            {
                bestTime = bestTime+range;
            }
            else if (fitness[2]<fitness[0] && fitness[2]<fitness[0])
            {
                bestTime = bestTime-range;
            }
            range = range/2;
        }
    }
    local_search_mode = 0;
    
}