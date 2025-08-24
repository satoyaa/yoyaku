#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "extern.h"

Node* genes[POPULATION];
Spot spots[MAX_NODES];
double fitness[POPULATION];

int main(){
    //データの読込．
    readdata("sampledata.txt");
    printf("%d %lf %lf\n",spots[3].value , spots[3].coordinate_x, spots[3].coordinate_y);
    //初期個体生成．

    initialize(0, 29);
    //評価値計算．
    calc_fitness();
    for (int i = 0; i < 3; i++)
    {
        printf("fitness is %f\n", fitness[i]);
    }
    
    //解の保存．
    
    return(0);
}