#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extern.h"


void mutation_NewBranch(){
    for (int i = 1; i < POPULATION; i++)
    {
        double r = (double)rand()/RAND_MAX;
        if (mutation_rateN < r)
        {
            continue;
        }
        //１～遺伝子長までにランダムに分岐点を決定
        int length = 0;
        int branches = 0;
        for (int j = 0; j < MAX_NODES; j++)
        {
            if(genes[i][j].vert==-1){break;}
            if(genes[i][j].dest!=-1){
                branches+=1;
            }
            length+=1;
        }
        if (length <= 2)
        {
            continue;
        }
        //分岐点から観光地数までからランダムにブランチsizeを決定
        int max_branch_size = 10;
        int points = 0;
        int branchSize = 0;
        points = rand() % length; // 分岐点を決定
        //printf("points %d %d %d %d\n", genes[i][points].dest, points, branchSize, length);
        if(0 < MAX_NODES-1-points && MAX_NODES-1-points < MAX_SPOTS){
            
            max_branch_size = MAX_NODES-1-points;
            //printf("Hello %d", branchSize);
        }
        branchSize = rand() % max_branch_size;
        
        //printf("%d %d %d %d\n", genes[i][points].dest, points, branchSize, length);
        //終了地点が分岐地点以降のブランチの目的地観光地+ブランチsize
        Gene temp[MAX_NODES];
        for (int j = 0; j < MAX_NODES; j++)
        {
            temp[j].vert = -1; temp[j].time = 100000; temp[j].dest = -1;
        }
        for (int j = 0; j < points+1; j++)
        {
            temp[j].vert = genes[i][j].vert; temp[j].time = genes[i][j].time; temp[j].dest = genes[i][j].dest;
            if(-1 < genes[i][j].dest && points < genes[i][j].dest){
                temp[j].dest += branchSize;
            }
            
        }
        
        temp[points].dest = points+branchSize;
        temp[points].time = (rand() % (TIMELIMIT/15))*15;
        
        //TIMELIMIT / (rand() % 6 + 1); //出発時刻を選択
        //ブランチ作成前に確率の分母を作成（予約が出やすいように）
        int denominator = 0;
        for (int k = 1; k < MAX_SPOTS-1; k++)
        {
            if (spots[k].reservable == 1)
            {
                denominator+=spots[k].value;
            }
        }
        
        //printf("%d %d %d \n", points+1, points+branchSize+1, length+branchSize);
        //ここに新たなブランチを作成する．ブランチsizeまで新しい遺伝子座で上書き，後ろにスライド（予約観光地が出やすくする処理）
        for (int j = points+1; j < points+branchSize+1; j++)
        {   
            int spot = 1;
            int current_sum = 0;
            if (j == points+1)
            {
                int r = rand()%denominator;
                for (int k = 1; k < MAX_SPOTS-1; k++)
                {
                
                    if (spots[k].reservable == 1)
                    {
                        current_sum+=spots[k].value;
                    }
                    if(r < current_sum){
                        spot=k;
                        break;
                    }
                }   
            }else{
                spot = rand()%(MAX_SPOTS-2)+1;
            }
            
            //while ((spot == start) || (spot == goal)){spot = rand()%MAX_SPOTS;} //スタートとゴール以外の観光地をランダムに選択
            
            temp[j].vert = spot;
            if (j == points+1)
            {
                genes_reserves[i][spot].spot = 1;
                if((TIMELIMIT-(calc_travel_time(spot, MAX_SPOTS-1)+spots[spot].t))<15){
                    genes_reserves[i][spot].time = 0;
                }else{
                    genes_reserves[i][spot].time = (rand() % ((TIMELIMIT-(calc_travel_time(spot, MAX_SPOTS-1)+spots[spot].t))/15))*15;
                }
                temp[points].time = genes_reserves[i][spot].time-calc_travel_time(genes[i][points].vert, spot);
            }
            
        }
        //ブランチ以降のノードをコピー
        for (int j = points+branchSize+1; j < MAX_NODES; j++)
        {
            temp[j].vert = genes[i][j-branchSize].vert;
            temp[j].dest = genes[i][j-branchSize].dest;
            temp[j].dest = genes[i][j-branchSize].time;
            
            //printf("%d %d %d %d\n", temp[j].vert, genes[i][j-branchSize].vert, j, branchSize);
        }
        //最大遺伝子長を超えるブランチを修正
        for (int j = 0; j < MAX_NODES; j++)
        {
            if(temp[j].dest>MAX_NODES && temp[j].vert != -1){
                temp[j].dest = MAX_NODES-1;
            }
        }
        //最後にコピー
        //printf("genes : ");
        for (int j = 0; j < MAX_NODES; j++)
        {
            genes[i][j].vert = temp[j].vert;
            genes[i][j].time = temp[j].time;
            genes[i][j].dest = temp[j].dest;
            if(temp[j].dest < -1 || MAX_NODES < temp[j].dest || length+branchSize < temp[j].dest){
                genes[i][j].dest = -1;
            }
          //printf("%d ", temp[j].vert);
        }
        //printf("\n");
    }
}