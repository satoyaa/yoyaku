#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extern.h"


void selection_tournament(){
    double best_fitness = fitness[0];
    int best_index = 0;
    Gene temp[POPULATION][MAX_NODES];
    Reserve temp_reserve[POPULATION][MAX_SPOTS];
    double temp_fitness[POPULATION];
    //一応初期化
    for (int i = 0; i < POPULATION; i++)
    {
        for (int j = 0; j < MAX_NODES; j++)
        {
            temp[i][j].vert=-1;
            temp[i][j].time=-1;
            temp[i][j].dest=-1;
            if(j<MAX_SPOTS){
                temp_reserve[i][j].spot=-1;
                temp_reserve[i][j].time=-1;
            } 
        }
        temp_fitness[i]=0;
    }
    for (int i = 0; i < POPULATION; i++)
    {
        if(best_fitness < fitness[i]){
            best_fitness = fitness[i];
            //printf("best is changed %d=>%d\n", best_index, i);
            best_index=i;
        }
        int r1 = rand() % POPULATION;
        int r2 = rand() % POPULATION;
        double fitness1 = fitness[r1];
        double fitness2 = fitness[r2];
        if (fitness1 > fitness2)
        {
            for (int j = 0; j < MAX_NODES; j++)
            {
                temp[i][j].vert = genes[r1][j].vert;
                temp[i][j].time = genes[r1][j].time;
                temp[i][j].dest = genes[r1][j].dest;
                if(j<MAX_SPOTS){
                    temp_reserve[i][j].spot = genes_reserves[r1][j].spot;
                    temp_reserve[i][j].time = genes_reserves[r1][j].time;
                }
            }
            temp_fitness[i]=fitness1;
        }
        else{
            for (int j = 0; j < MAX_NODES; j++)
            {
                temp[i][j].vert = genes[r2][j].vert;
                temp[i][j].time = genes[r2][j].time;
                temp[i][j].dest = genes[r2][j].dest;
                if (j<MAX_SPOTS)
                {
                    temp_reserve[i][j].spot = genes_reserves[r2][j].spot;
                    temp_reserve[i][j].time = genes_reserves[r2][j].time;
                }  
            }
            temp_fitness[i]=fitness2;
        }
    }
    //エリート個体を置き換える
    for (int j = 0; j < MAX_NODES; j++)//あまりここの書き方良くない
    {
        genes[0][j].vert = genes[best_index][j].vert;
        genes[0][j].time = genes[best_index][j].time;
        genes[0][j].dest = genes[best_index][j].dest;
        if(j<MAX_SPOTS){
            genes_reserves[0][j].spot = genes_reserves[best_index][j].spot;
            genes_reserves[0][j].time = genes_reserves[best_index][j].time;
        } 
    }
    fitness[0]=fitness[best_index];
    //エリート以外を置き換える
    for (int i = 1; i < POPULATION; i++)
    {
        for (int j = 0; j < MAX_NODES; j++)
        {
            genes[i][j].vert = temp[i][j].vert;
            genes[i][j].time = temp[i][j].time;
            genes[i][j].dest = temp[i][j].dest;
            if(j<MAX_SPOTS){
                genes_reserves[i][j].spot = temp_reserve[i][j].spot;  
                genes_reserves[i][j].time = temp_reserve[i][j].time;    
            } 
        }
        fitness[i]=temp_fitness[i];
    }
    
}