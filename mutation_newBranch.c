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
        int isDelete = (double)rand() / RAND_MAX;
        int points = 0;
        int branchSize = 0;
        if(isDelete < 0.5 && 0 < branches ){            
            int deletePoint = rand() % branches+1;
            int deletePointCount = 0;
            int deletePointIndex = 0;
            
            for (int j = 0; j < length; j++)
            {
                
                if (genes[i][j].dest != -1)
                {
                    deletePointCount+=1;
                }
                if(deletePointCount>=deletePoint){
                    points = j;
                    //printf("points %d \n",points);
                    break;
                }
                if(j==length-1)
                {
                    if(deletePointCount!=0){
                        j=-1;
                    }else{
                        break;
                    }
                }
            }
            branchSize = 0;
        }
        else{
            points = rand() % length; // 分岐点を決定
            if(0 < MAX_NODES-1-points && MAX_NODES-1-points < MAX_SPOTS){
                max_branch_size = MAX_NODES-1-points;
                branchSize = rand() % max_branch_size;
            }
        }
        //printf("%d %d %d\n", genes[i][points].dest, points, branchSize);
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
                
                {
                    temp[j].dest += branchSize;
                }
            }
            
        }
        
        temp[points].dest = points+branchSize;
        temp[points].time = (rand() % (TIMELIMIT/15))*15;
        
        //TIMELIMIT / (rand() % 6 + 1); //出発時刻を選択
        //ブランチ作成前に確率の分母を作成（予約が出やすいように）
        int denominator = 0;
        
        //ここに新たなブランチを作成する．ブランチsizeまで新しい遺伝子座で上書き，後ろにスライド（予約観光地が出やすくする処理）
        for (int j = points+1; j < points+branchSize+1; j++)
        {
            denominator = 0;
            for (int k = 1; k < MAX_SPOTS-1; k++)
            {
                if (j == points+1)
                {
                    if (spots[j].reservable == 1)
                    {
                        denominator+=spots[k].value;
                    }
                }else{
                    denominator+=genes_reserves[i][k].spot+1;
                }
            }
            int spot = 1;
            int r = rand()%denominator;
            int current_sum = 0;
            for (int k = 1; k < MAX_SPOTS-1; k++)
            {
                if (j == points+1)
                {
                    if (spots[j].reservable == 1)
                    {
                        current_sum+=spots[k].value;
                    }
                }else{
                    current_sum += genes_reserves[i][k].spot+1;
                }
                if(r < current_sum){
                    spot=k;
                    break;
                }
                
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
        if(isDelete < 0.5 && 0 < branches ){
                for (int j = points+1; j < MAX_NODES; j++)
                {
                    if(j+genes[i][points].dest > MAX_NODES-1){
                        break;
                    }
                    temp[j].vert = genes[i][j+genes[i][points].dest].vert;
                    temp[j].dest = genes[i][j+genes[i][points].dest].dest;
                    temp[j].time = genes[i][j+genes[i][points].dest].time;
                    if(temp[j].dest > -1){
                        //printf("%d %d %d\n",temp[j].dest ,genes[i][points].dest, points);
                        temp[j].dest -= (genes[i][points].dest-points);
                    }
                    
                    
                    //printf("%d %d %d %d\n", temp[j], genes[i][j-branchSize], j, branchSize);
                }
                
        }else{
            for (int j = points+branchSize+1; j < MAX_NODES; j++)
            {
                temp[j].vert = genes[i][j-branchSize].vert;
                temp[j].dest = genes[i][j-branchSize].dest;
                temp[j].dest = genes[i][j-branchSize].time;
                
                //printf("%d %d %d %d\n", temp[j], genes[i][j-branchSize], j, branchSize);
            }
        }
        //最大遺伝子長を超えるブランチを修正
        for (int j = 0; j < MAX_NODES; j++)
        {
            if(temp[j].dest>MAX_NODES && temp[j].vert != -1){
                temp[j].dest = MAX_NODES-1;
            }
        }
        //最後にコピー
        for (int j = 0; j < MAX_NODES; j++)
        {
            genes[i][j].vert = temp[j].vert;
            genes[i][j].time = temp[j].time;
            genes[i][j].dest = temp[j].dest;
            if(temp[j].dest < -1 || MAX_NODES < temp[j].dest || length+branchSize < temp[j].dest){
                genes[i][j].dest = -1;
            }
        }
    }
}