#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extern.h"


void mutation_NewBranch3(){
    for (int i = 1; i < POPULATION; i++)
    {
        double r = (double)rand()/RAND_MAX;
        if (mutation_rateN < r)
        {
            continue;
        }
        int isReserve = 0;
        //初期化
        Gene temp[MAX_NODES];
        for (int j = 0; j < MAX_NODES; j++)
        {
            temp[j].vert = -1;
            temp[j].time = 100000;
            temp[j].dest = -1;
        }
        
        for (int j = 0; j < MAX_SPOTS; j++)
        {
            if (genes_reserves[i][j].spot == 1)
            {
                isReserve+=1;
            }
        }
        int spot = 1;
        if (isReserve == 0)
        {
            int denominator = 0;
            for (int j = 1; j < MAX_SPOTS-1; j++)
            {
                if (spots[j].reservable == 1)
                {
                    denominator+=spots[j].value;
                }
            }
            int ra = rand()%denominator;
            int current_sum = 0;
            for (int j = 1; j < MAX_SPOTS-1; j++)
            {
                if (spots[j].reservable == 1)
                {
                    current_sum += spots[j].value;
                }
                if(ra < current_sum){
                    spot=j;
                    break;
                }
            }
            genes_reserves[i][spot].spot = 1;
            genes_reserves[i][spot].time = (rand() % (TIMELIMIT/15))*15;
            
        }else{
            int ra = rand()%isReserve;
            int current_sum = 0;
            for (int j = 0; j < MAX_SPOTS; j++)
            {
                if (genes_reserves[i][j].spot==1)
                {
                    current_sum+=1;   
                }
                if(current_sum >= ra){
                    spot = j;
                    break;
                }
            }
        }
        int index = 1;
        for (int j = 0; j < MAX_NODES; j++)
        {
            if(genes[i][index].vert = -1){break;}
            if (genes[i][index].vert == spot)
            {
                temp[j].vert = genes[i][index].vert;
                temp[j].dest = genes[i][index].dest;
                temp[j].time = genes[i][index].time;
                index += 1;
                continue;
            }
            temp[j].vert = genes[i][index].vert;
            temp[j].dest = genes[i][index].dest;
            temp[j].time = genes_reserves[i][spot].time;
            index += 1;
            temp[j+1].vert = spot;
            if ((genes[i][index].vert != genes[i][index+1].vert) && (genes[i][index+1].vert != spot) && (genes[i][index+1].vert != -1))
            {
                temp[j+1].time = genes_reserves[i][spot].time - (calc_travel_time(genes[i][index].vert, genes[i][index+1].vert) + spots[genes[i][index].vert].t + calc_travel_time(genes[i][index+1].vert, spot));
            }
            temp[j+1].time = genes_reserves[i][spot].time - calc_travel_time(genes[i][index].vert, spot);
            temp[j+1].dest = j+2;
            j++;
        }
        for (int j = 0; j < MAX_NODES; j++)
        {
            genes[i][j].vert = temp[j].vert;
            genes[i][j].time = temp[j].time;
            genes[i][j].time = temp[j].time;
        }
    }
}