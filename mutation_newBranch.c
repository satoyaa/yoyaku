#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extern.h"


void mutation_NewBranch(){
    for (int i = 1; i < POPULATION; i++)
    {
        double r = (double)rand()/RAND_MAX;
        if (mutation_rate < r)
        {
            continue;
        }
        //１～遺伝子長までにランダムに分岐点を決定
        int length = 0;
        for (int j = 0; j < MAX_NODES; j++)
        {
            if(genes[i][j].vert==-1){break;}
            length+=1;
        }
        if (length <= 2)
        {
            continue;
        }
        int points = rand() % length; // 分岐点を決定
        //分岐点から観光地数までからランダムにブランチsizeを決定
        int max_branch_size = MAX_SPOTS;
        if(MAX_NODES-1-points < MAX_SPOTS){
            max_branch_size = MAX_NODES-1-points;
        }
        int branchSize = rand() % max_branch_size;
        if(genes[i][points].dest!=-1){
            int isDelete = (double)rand() / RAND_MAX;
            if(isDelete < 0.5){branchSize = 0;}
            else{
                branchSize = genes[i][points].dest-points;
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
                temp[j].dest += branchSize;
            }
            
        }
        temp[points].dest = points+branchSize;
        temp[points].time = TIMELIMIT / (rand() % 6 + 1); //出発時刻を選択
        //ブランチ作成前に確率の分母を作成（予約が出やすいように）
        int denominator = 0;
        for (int j = 1; j < MAX_SPOTS-1; j++)
        {
            denominator+=genes_reserves[i][j].spot+1;
        }
        //ここに新たなブランチを作成する．ブランチsizeまで新しい遺伝子座で上書き，後ろにスライド（予約観光地が出やすくする処理）
        for (int j = points+1; j < points+branchSize+1; j++)
        {
            int spot = 1;
            int r = rand()%denominator;
            int current_sum = 0;
            for (int k = 1; k < MAX_SPOTS-1; k++)
            {
                current_sum += genes_reserves[i][k].spot+1;
                if(r < current_sum){
                    spot=k;
                    break;
                }
            }
            //while ((spot == start) || (spot == goal)){spot = rand()%MAX_SPOTS;} //スタートとゴール以外の観光地をランダムに選択
            temp[j].vert = spot;
        }
        //ブランチ以降のノードをコピー
        for (int j = points+branchSize+1; j < MAX_NODES; j++)
        {
            temp[j] = genes[i][j-branchSize];
            //printf("%d %d %d %d\n", temp[j], genes[i][j-branchSize], j, branchSize);
        }
        //最大遺伝子長を超えるブランチを修正
        for (int j = 0; j < MAX_NODES; j++)
        {
            if(temp[j].dest>MAX_NODES){
                temp[j].dest = MAX_NODES-1;
            }
        }
        //最後にコピー
        for (int j = 0; j < MAX_NODES; j++)
        {
            genes[i][j].vert = temp[j].vert;
            genes[i][j].time = temp[j].time;
            genes[i][j].dest = temp[j].dest;
        }
    }
}