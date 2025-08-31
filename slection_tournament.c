#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extern.h"

void selection_tournament(){
    double best_fitness = 0;
    int best_index;
    int temp[POPULATION][MAX_SPOTS];
    int temp_reserve[POPULATION][MAX_SPOTS];
    for (int i = 0; i < POPULATION; i++)
    {
        if(best_fitness < fitness[i]){best_fitness = fitness[i];best_index=i;}
        int r1 = rand() % POPULATION;
        int r2 = rand() % POPULATION;
        double fitness1 = fitness[r1];
        double fitness2 = fitness[r2];
        if (fitness1 > fitness2)
        {
            for (int j = 0; j < MAX_SPOTS; j++)
            {
                temp[i][j] = genes[r1][j];
                temp_reserve[i][j] = genes_reserves[r1][j];
            }
        }
        else{
            for (int j = 0; j < MAX_SPOTS; j++)
            {
                temp[i][j] = genes[r2][j];
                temp_reserve[i][j] = genes_reserves[r2][j];
            }
        }
    }
    //エリート個体を置き換える
    for (int j = 0; j < MAX_SPOTS; j++)//あまりここの書き方良くない
    {
        genes[0][j] = genes[best_index][j];
        genes_reserves[0][j] = genes_reserves[best_index][j];
    }
    //エリート以外を置き換える
    for (int i = 1; i < POPULATION; i++)
    {
        for (int j = 0; j < MAX_SPOTS; j++)
        {
            genes[i][j] = temp[i][j];
            genes_reserves[i][j] = temp_reserve[i][j];
        }
    }
    
    
    
}