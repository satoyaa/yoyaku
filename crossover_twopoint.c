#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "extern.h"

void crossover_twopoint(){
    //予約の交叉は後回し
    Gene temp1[MAX_NODES];
    Gene temp2[MAX_NODES];
    for(int i = 0;i<POPULATION;i+=2){
        double r = (double)rand()/RAND_MAX;
        if (crossover_rate < r){continue;}
        int length1 = 0;
        int length2 = 0;
        for (int j = 0; j < MAX_NODES; j++)
        {
            if(genes[i][j].vert!=-1){length1+=1;}
            if(genes[i+1][j].vert!=-1){length2+=1;}
        }
        //printf("length1:%d length2:%d\n",length1, length2);
        //tempを初期化
        for(int j=0;j<MAX_NODES;j++){
            temp1[j].vert=-1;
            temp2[j].vert=-1;
            temp1[j].time=100000;
            temp2[j].time=-1;
            temp1[j].dest=100000;
            temp2[j].dest=-1;
        }

        int start1 = 1;
        int end1 = 1;
        int start2 = 1;
        int end2 = 1;
        if(length1!=1){
            start1 = rand()%(length1-1)+1;
            end1 = rand()%(length1-1)+1;
        }
        if(length2!=1){
            start2 = rand()%(length2-1)+1;
            end2 = rand()%(length2-1)+1;
        }
        int index1 = 0;
        int index2 = 0;
        if(end1<start1){int temp = end1;end1 = start1;start1 = temp;}
        if(end2<start2){int temp = end2;end2 = start2;start2 = temp;}
        for (int j = 0; j < start1; j++)
        {
            temp1[j].vert=genes[i][j].vert;
            temp1[j].time=genes[i][j].time;
            temp1[j].dest=genes[i][j].dest;
            index1+=1;
        }
        for (int j = 0; j < start2; j++)
        {
            temp2[j].vert=genes[i+1][j].vert;
            temp2[j].time=genes[i+1][j].time;
            temp2[j].dest=genes[i+1][j].dest;
            index2+=1;
        }
        for (int j = start2; j < end2; j++)
        {
            if(index1>=MAX_NODES){ break;}
            temp1[index1].vert = genes[i][j].vert;
            temp1[index1].time = genes[i][j].time;
            temp1[index1].dest = genes[i][j].dest;
            index1+=1;
        }
        for (int j = start1; j < end1; j++)
        {
            if(index2>=MAX_NODES){break;}
            temp2[index2].vert = genes[i+1][j].vert;
            temp2[index2].time = genes[i+1][j].time;
            temp2[index2].dest = genes[i+1][j].dest;
            index2+=1;
        }
        for (int j = end1; j < MAX_NODES; j++)
        {
            if (index1>=MAX_NODES){break;}
            temp1[index1].vert = genes[i][j].vert;
            temp1[index1].time = genes[i][j].time;
            temp1[index1].dest = genes[i][j].dest;
            index1+=1;
        }
        for (int j = end2; j < MAX_NODES; j++)
        {
            if (index2>=MAX_NODES){break;}
            temp2[index2].vert = genes[i+1][j].vert;
            temp2[index2].time = genes[i+1][j].time;
            temp2[index2].dest = genes[i+1][j].dest;
            index2+=1;
        }
        //範囲外への分岐を修正
        for (int j = 0; j < MAX_NODES; j++)
        {
            if (temp1[j].dest>index1){temp1[j].dest=index1;}
            if (temp2[j].dest>index2){temp2[j].dest=index2;}        
        }
        
        //tempをgeneに置き換えて終了
        for (int j = 0; j < MAX_NODES; j++) {
            genes[i][j].vert = temp1[j].vert;
            genes[i][j].time = temp1[j].time;
            genes[i][j].dest = temp1[j].dest;
            genes[i+1][j].vert = temp2[j].vert;
            genes[i+1][j].time = temp2[j].time;
            genes[i+1][j].dest = temp2[j].dest;
            //printf("%d %d %d %d\n",temp_reserve1[0][j].spot, temp_reserve1[0][j].time, temp_reserve2[0][j].spot,temp_reserve2[0][j].time);
        }
    }
}