#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "extern.h"

int genes[POPULATION][MAX_SPOTS];
int genes_reserves[POPULATION][MAX_SPOTS];
Spot spots[MAX_SPOTS];
double fitness[POPULATION];
int queue_range[MAX_SPOTS];
double crossover_rate = 1;

void ga(){
    initialize(0,29); //(start, goal)
    calc_fitness();
    for (int i = 0; i < MAX_ITERATION; i++)
    {
        //選択
        selection_tournament();
        //交叉
        //crossover_pmx();
        //突然変異

        //評価値計算
        calc_fitness();
    }
}

int main(){
    //データの読込．
    readdata("output.txt");
    //初期個体生成．

    initialize(0, 29);
    for (int i = 0; i < 3; i++)
    {
        printf("genes[%d]:",i);
        for (int j = 0; j < MAX_SPOTS; j++)
        {
            printf("%d ", genes[i][j]);
        }
        printf("\n");
        printf("reserves[%d]:",i);
        for (int j = 0; j < MAX_SPOTS; j++)
        {
            printf("%d ", genes_reserves[i][j]);
        }
        printf("\n");
        
    }
    
    //評価値計算．
    printf("hello\n");
    //calc_fitness();
    ga();
    for (int i = 0; i < 3; i++)
    {
        printf("fitness is %f\n", fitness[i]);
    }
    
    printf("bye\n");
    //解の保存．
    
    return(0);
}