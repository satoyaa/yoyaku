#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "extern.h"

int calc_duration(int i, int j){
    double x1 = spots[i].coordinate_x;
    double x2 = spots[j].coordinate_x;
    double y1 = spots[i].coordinate_y;
    double y2 = spots[j].coordinate_y;

    double dx = (x1 - x2);
    double dy = (y1 - y2);

    double distance = sqrt(dx*dx + dy*dy);

    double duration = distance/(SPEED*1000/60);
    return(duration);
}

void initialize(int start, int goal){
    for (int i = 0; i < POPULATION; i++)
    {
        int index = 0;
        int n = 1;
        for (int j = 0; j < MAX_NODES; j++){genes[i][j] = -1;}
        genes[i][0]=start;

        
        while (index < MAX_NODES )
        {
            if(index==start || index == goal){index++;continue;}
            int r = rand()%(MAX_NODES-2)+1;
            for (int j = 0; j < MAX_NODES; j++)
            {
                if(genes[i][r] == -1){break;}
                r+=n;
                if(r < 1){r = MAX_NODES-2;}
                if(MAX_NODES-2 < r){r = 1;}
            }
            genes[i][r]= index;
            index++;
            n = n * (-1);
        }
        genes[i][MAX_NODES-1]=goal;   
    }
    for (int i = 0; i < POPULATION; i++)
    {
        for (int j = 0; j < MAX_NODES; j++)
        {
            double r = (double)rand()/RAND_MAX;
            probs[i][j] = r;
        }
    }
    
}