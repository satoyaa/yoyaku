#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extern.h"


void crossover_pmx(){
    int temp1[MAX_SPOTS];
    int temp2[MAX_SPOTS];
    int temp_reserve1[MAX_SPOTS];
    int temp_reserve2[MAX_SPOTS];
    int start = rand()%MAX_SPOTS;
    int end = rand()%MAX_SPOTS;
    if(end<start){
        int temp = end;
        end = start;
        start = temp;
    }
    for(int i = 0;i<POPULATION;i+=2){
        double r = (double)rand()/RAND_MAX;
        if (crossover_rate < r){continue;}
        //tempを初期化
        for(int j=0;j<MAX_SPOTS;j++){
            temp1[j]=-1;
            temp2[j]=-1;
            temp_reserve1[j] = -1;
            temp_reserve2[j] = -1;
        }
        for(int j=start;j<end;j++){
            temp1[j]=genes[i+1][j];//2つ目(親２)の遺伝子で一つ目(親１)を置き換え
            temp2[j]=genes[i][j];//1つ目の遺伝子で2つ目を置き換え
            temp_reserve1[j] = genes_reserves[i+1][j];
            temp_reserve2[j] = genes_reserves[i][j];
        }
        int gene1;
        int gene2;
        int j1 = start;
        int j2 = start;
        //printf("start;%d,end%d\n",start,end);
        //重複検索，置き換え
        while (j1 != end && j2 != end)
        {
            //printf("j1:%d,j2:%d\n",j1,j2);
            int found = 0;
            gene1 = temp1[j1];
            gene2 = temp2[j2];
            //範囲内にgene2と同じものがあるなら次
            for(int k=start;k<end;k++){
                if(gene2==temp1[k]){
                    j2++;
                    found = 1;
                    break;
                }
            }
            //範囲内にgene1と同じものがあるなら次
            for(int k=start;k<end;k++){
                if(gene1==temp2[k]){
                    j1++;
                    found = 1;
                    break;
                }
            }
            if (found){continue;}
            //gene2を使ってtemp1の空きスペースを親１で埋める
            //gene2は親２をコピーしたtemp1の範囲内と重複しない親１の値，つまりtemp1には存在しない値
            //親１と一致するかを判定，一致する値があれば,逆も然り
            for(int k=0;k<MAX_SPOTS;k++){
                //gene1(範囲内の親2)を範囲外の親1で発見したなら，その位置にgene2を置き換え
                //ここまでの処理でgene1は範囲内にないことが確認済み
                if(gene1 == genes[i][k]){
                    temp1[k]=gene2;
                    temp_reserve1[k]=temp_reserve2[j2];
                }
                if(gene2 == genes[i+1][k]){
                    temp2[k]=gene1;
                    temp_reserve2[k]=temp_reserve1[j1];
                }
            }
            if (j1 < end){j1++;}if(j2 < end){j2++;}
        }
        
        

        //残りのスペースを埋める
        for (int j = 0; j < MAX_SPOTS; j++) {
            if (temp1[j] == -1) {
                temp1[j] = genes[i][j];
                temp_reserve1[j] = genes_reserves[i][j];
            }
            if (temp2[j] == -1) {
                temp2[j] = genes[i+1][j];
                temp_reserve2[j] = genes_reserves[i+1][j];
            }
        }   

        //予約に整合性を付ける

        for (int j = 0; j < MAX_SPOTS; j++)
        {
            int reserve_spot = genes_reserves[i][j];
            if(reserve_spot < j){genes_reserves[i][reserve_spot]=j;genes_reserves[i][j]=-1;} 
            reserve_spot = genes_reserves[i+1][j];
            if(reserve_spot < j){genes_reserves[i+1][reserve_spot]=j;genes_reserves[i+1][j]=-1;}            
        }
        

        //tempをgeneに置き換えて終了
        for (int j = 0; j < MAX_SPOTS; j++) {
            genes[i][j] = temp1[j];
            genes[i+1][j] = temp2[j];
            genes_reserves[i][j] = temp_reserve1[j];
            genes_reserves[i+1][j] = temp_reserve2[j];
        }   
    }


}