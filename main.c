#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "extern.h"

Gene genes[MAX_SPOTS*MAX_SPOTS];
Reserve genes_reserves[MAX_SPOTS];
int genes_timelimit[MAX_SPOTS];
int times[MAX_SPOTS];
Spot spots[MAX_SPOTS];
double fitness;
int queue_range[MAX_SPOTS];
Save save_maxroot[MAX_SPOTS];
Save save_minroot[MAX_SPOTS];
Save save_temproot[LOOPS][MAX_SPOTS];
int count_temproot[LOOPS];
double crossover_rate = 0.5;
double mutation_rate = 0.5;
int TIMELIMIT;
double min = INFINITY; //処理の都合で今代入
double max = -INFINITY; //処理の都合で今代入
double best;
int reserve_rate;
int savemode;

//ファイル書き込みプログラム
void save_file(const char *filename, Save *array, size_t size) {
    FILE *fp = fopen(filename, "w"); // 書き込みモードで開く
    char line[256];
    if (fp == NULL) {
        perror("I can't open the file\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < size; i++) {
        if(array[i].vert==-1){break;}//ノード，x座標，y座標，出発予定時刻，予約有無，予約時刻
        printf("%d %f %f %d %d %d\n",array[i].vert, spots[array[i].vert].coordinate_x, spots[array[i].vert].coordinate_y, array[i].time, array[i].reserve, array[i].reservetimes);
        snprintf(line, sizeof(line), "%d %f %f %d %d %d\n",array[i].vert, spots[array[i].vert].coordinate_x, spots[array[i].vert].coordinate_y, array[i].time, array[i].reserve, array[i].reservetimes);
        fprintf(fp, "%s", line);
    }
}

void decide_gene_yoyakuTime (int index){
    if (index<MAX_SPOTS)
    {
        printf("debug: calculate fitness\n");
        calc_fitness(0, MAX_SPOTS-1);
        //最良値更新
        if(best < fitness){
            best = fitness;
        }
        printf("best:%f\n",best);
        return;
    }
    //予約時間の決定
    for (int t = 0; t < TIMELIMIT; t=t+10)
    {
        if(genes_reserves[index].spot==-1){
            break;
        }
        genes_reserves[index].time = t;
        //printf("genes_reserves[%d]: spot=%d time=%d\n",index,genes_reserves[index].spot,genes_reserves[index].time);
        decide_gene_yoyakuTime(index+1);
    }
};

void decide_gene_yoyaku (int index){
    if (index<MAX_SPOTS)
    {
        decide_gene_yoyakuTime(0);   
        return;
    }
    
    //予約の決定
    for (int r = 0; r < 2; r++)
    {
        if(spots[genes[index].vert].reservable==0||r==0){
            break;
        }
        genes_reserves[index].spot = r;
        //printf("genes_reserves[%d]: spot=%d time=%d\n",index,genes_reserves[index].spot,genes_reserves[index].time);
        decide_gene_yoyaku(index+1);
    }
    
};

void decide_gene_value (int length, int index, int *root, int gene_length){
    //printf("decide_gene_value called: length=%d index=%d gene_length=%d\n",length,index,gene_length);
    //遺伝子生成の終了条件
    if (gene_length<index){
        decide_gene_yoyaku(0);
        return;
    }
    //枝分かれの終了条件
    if (length<index){
        printf("length break %d %d\n", length, index);
        return;
    }
    
    for (int i = 0; i < MAX_SPOTS; i++)
    {
        //printf("genes[%d].vert=%d\n",index,i);
        genes[index].vert = i;
        
        int flag = 0;
        //枝狩り
        for (int j = 0; j < MAX_SPOTS; j++)
        {
            if(root[j]==i){
                
                flag = 1;
                break;
            }
        }
        if (flag)
        {
            continue;
        }
        root[index]=i;
        
        //分岐があれば再帰
        if(genes[index].dest!=-1){
            decide_gene_value(genes[index].dest, index + 1, root, gene_length);
            decide_gene_value(length, genes[index].dest, root, gene_length);
        }
        else{
            decide_gene_value(length, index + 1, root, gene_length);
        }
    }
}

void decide_gene_time (int length, int index){
    if (index<0)
    {
        int root[MAX_NODES];
        decide_gene_value(length, 0, root, length);
        return;
    }
    
    //分岐時間の決定
    for (int t = 0; t < TIMELIMIT; t=t+10)
    {
        if (genes[index].dest==-1)
        {
            continue;
        }
        
        genes[index].time = t;
        //printf("genes[%d].time=%d\n",index,genes[index].time);
        decide_gene_time(length, index-1);
    }
}

void decide_gene_structure (int length, int index){
    if (index<0)
    {
        //genesのdestをすべて表示
        /*
        printf("gene structure decided: ");
        for (int i = 0; i < length; i++)
        {
            printf("%d ", genes[i].dest);
        }
        printf("\n");*/
        
        decide_gene_time(length, index);
        return;
    }
    
    //遺伝子構造の決定
    for (int i = -1; i < length-index; i++)
    {        
        if (i != -1)
        {
            if (i==length-index-1)
            {
                continue;
            }
            genes[index].dest = index+i+1;
        }
        else{genes[index].dest = i;}
        //printf("genes[%d].dest=%d\n",index,genes[index].dest);
        decide_gene_structure(length, index-1);
    }
}




void searchAll(){
    savemode = 0;
    srand((unsigned int)time(NULL));//実行毎に違うを出したい
    min=INFINITY;
    max=-INFINITY;
    best = -INFINITY;
    //予約初期化
    for (int j = 0; j < MAX_SPOTS; j++)
        {
            genes_reserves[j].spot = -1;genes_reserves[j].time = -1;
        }
    //一重目　遺伝子長の決定
    for (int i = 0; i < MAX_NODES; i++)
    {
        //遺伝子の初期化
        for (int j = 0; j < MAX_SPOTS*MAX_SPOTS; j++){
            genes[j].vert = -1; genes[j].time = 100000; genes[j].dest = -1;
        }
        //遺伝子の構造の決定
        printf("gene length:%d\n",i);
        decide_gene_structure(i, i);
    }
    printf("best:%f\n",best);
    
    
}

int main(){
    //データの読込．
    readdata("o-sakaFoodsX.txt");
    //初期個体生成．
    /* initialize(0, MAX_SPOTS-1);
    for (int i = 0; i < POPULATION; i++)
    {
        printf("genes[%d]:",i);
        for (int j = 0; j < MAX_SPOTS; j++)
        {
            printf("%d ", genes[i][j]);
        }
        printf("\n");
        printf("reserves[%d]:",i);
        for (int j = 0; j < MAX_SPOTS; j++)
        {
            printf("%d ", genes_reserves[i][j]);
        }
        printf("\n");
        
    } */
    
    //評価値計算．
    printf("hello\n");
    clock_t time1, time2;
    time1 = clock();   
    //ga();
    //serchAll(int start)
    time2 = clock();

    //自動実験プログラム
    int time[] = {60, 120, 180, 240};
    const char* filename = "root/Result.txt";
    FILE* fp = fopen(filename, "w"); 
    for (int i = 0; i < 1; i++)
    {
        reserve_rate = 1;
        TIMELIMIT = time[i];
        char line[256];
        char filenameA[20];
        char filenameB[20];
        char filenameC[20];
        char filenameD[20];
        char filenameE[20];
        sprintf(filenameA, "root/max_test%d.txt", time[i]);
        sprintf(filenameB, "root/min_test%d.txt", time[i]);
        sprintf(filenameC, "root/freqent1_%d.txt", time[i]);
        sprintf(filenameD, "root/freqent2_%d.txt", time[i]);
        sprintf(filenameE, "root/freqent3_%d.txt", time[i]);
        double sum_max = 0;
        double sum_min = 0;
        double sum_average = 0;
        double sum_time = 0;
        Save maxroot[MAX_SPOTS];
        Save minroot[MAX_SPOTS];
        double best_max  = -INFINITY;
        double best_min = INFINITY;
        int count = 10;
        for (int j = 0; j < count; j++)
        {
            time1 = clock();   
            searchAll();
            time2 = clock();   
            sum_max += max;
            sum_min += min;
            sum_average += best;
            sum_time+=(double)(time2 - time1) / CLOCKS_PER_SEC;
            if (best_max < max)
            {
                best_max = max;
                for (int k = 0; k < MAX_SPOTS; k++)
                {
                    maxroot[k].vert = save_maxroot[k].vert;
                    maxroot[k].time = save_maxroot[k].time;
                    maxroot[k].reserve = save_maxroot[k].reserve;
                    maxroot[k].reservetimes = save_maxroot[k].reservetimes;
                }
                
            }
            if (min < best_min)
            {
                best_min = min;
                for (int k = 0; k < MAX_SPOTS; k++)
                {
                    minroot[k].vert = save_minroot[k].vert;
                    minroot[k].time = save_minroot[k].time;
                    minroot[k].reserve = save_minroot[k].reserve;
                    minroot[k].reservetimes = save_minroot[k].reservetimes;
                }
            }
            
        }
        //頻出解保存
        int count_max = -1;
        int count_second = -1;
        int count_third = -1;
        int count_max_index = 1;
        int count_second_index = 1;
        int count_third_index = 1;
        for (int j = 0; j < LOOPS; j++)
        {
            if(0 && j<10){
                printf("%d\n",count_temproot[j]);
            }
            if(count_max < count_temproot[j]){
                count_third=count_second;
                count_second=count_max;
                count_max=count_temproot[j];
                count_third_index=count_second_index;
                count_second_index=count_max_index;
                count_max_index=j;}
            else if (count_second < count_temproot[j]){
                count_third=count_second;
                count_second=count_temproot[j];
                count_third_index=count_second_index;
                count_second_index=j;}
            else if (count_third < count_temproot[j]){
                count_third=count_temproot[j];
                count_third_index=j;}
        }
        
        //ここまで
        printf("most appeare root probabilty is %d\n",count_max);
        printf("excuse: %f sec\n", sum_time/count);
        snprintf(line, sizeof(line), "%d %f %f %f %d %d %d\n",time[i], sum_max/count, sum_min/count, sum_average/count, count_max, count_second, count_third);
        fprintf(fp, "%s", line);
        //save_file(filenameA,maxroot,MAX_SPOTS);
        //save_file(filenameB,minroot,MAX_SPOTS);
        //save_file(filenameC,save_temproot[count_max_index],MAX_SPOTS);
        //save_file(filenameD,save_temproot[count_second_index],MAX_SPOTS);
        //save_file(filenameE,save_temproot[count_third_index],MAX_SPOTS);
        
    }
    for (int i = 0; i < 0; i++)
    {
        reserve_rate = 0;
        TIMELIMIT = time[i];
        char line[256];
        char filenameA[20];
        char filenameB[20];
        char filenameC[20];
        char filenameD[20];
        char filenameE[20];
        sprintf(filenameA, "root/max_testX%d.txt", time[i]);
        sprintf(filenameB, "root/min_testX%d.txt", time[i]);
        sprintf(filenameC, "root/freqent1_X%d.txt", time[i]);
        sprintf(filenameD, "root/freqent2_X%d.txt", time[i]);
        sprintf(filenameE, "root/freqent3_X%d.txt", time[i]);
        double sum_max = 0;
        double sum_min = 0;
        double sum_average = 0;
        double sum_time = 0;
        Save maxroot[MAX_SPOTS];
        Save minroot[MAX_SPOTS];
        double best_max  = -INFINITY;
        double best_min = INFINITY;
        int count = 1;
        for (int j = 0; j < count; j++)
        {
            printf("%d done\n",j * 10);
            time1 = clock();   
            searchAll();
            time2 = clock();   
            sum_max += max;
            sum_min += min;
            sum_average += best;
            sum_time+=(double)(time2 - time1) / CLOCKS_PER_SEC;
            if (best_max < max)
            {
                best_max = max;
                for (int k = 0; k < MAX_SPOTS; k++)
                {
                    maxroot[k].vert = save_maxroot[k].vert;
                    maxroot[k].time = save_maxroot[k].time;
                    maxroot[k].reserve = save_maxroot[k].reserve;
                    maxroot[k].reservetimes = save_maxroot[k].reservetimes;
                }
                
            }
            if (min < best_min)
            {
                best_min = min;
                for (int k = 0; k < MAX_SPOTS; k++)
                {
                    minroot[k].vert = save_minroot[k].vert;
                    minroot[k].time = save_minroot[k].time;
                    minroot[k].reserve = save_minroot[k].reserve;
                    minroot[k].reservetimes = save_minroot[k].reservetimes;
                }
            }
            
        }
        //頻出解保存
        int count_max = -1;
        int count_second = -1;
        int count_third = -1;
        int count_max_index = 1;
        int count_second_index = 1;
        int count_third_index = 1;
        for (int j = 0; j < LOOPS; j++)
        {
            if(0 && j<10){
                printf("%d\n",count_temproot[j]);
            }
            if(count_max < count_temproot[j]){
                count_third=count_second;
                count_second=count_max;
                count_max=count_temproot[j];
                count_third_index=count_second_index;
                count_second_index=count_max_index;
                count_max_index=j;}
            else if (count_second < count_temproot[j]){
                count_third=count_second;
                count_second=count_temproot[j];
                count_third_index=count_second_index;
                count_second_index=j;}
            else if (count_third < count_temproot[j]){
                count_third=count_temproot[j];
                count_third_index=j;}
        }
        //ここまで
        printf("most appeare root probabilty is %d\n",count_max);
        printf("excuse: %f sec\n", sum_average/count);
        snprintf(line, sizeof(line), "%dX %f %f %f %d %d %d\n",time[i], sum_max/count, sum_min/count, sum_average/count, count_max, count_second, count_third);
        fprintf(fp, "%s", line);
        //save_file(filenameA,maxroot,MAX_SPOTS);
        //save_file(filenameB,minroot,MAX_SPOTS);
        //save_file(filenameC,save_temproot[count_max_index],MAX_SPOTS);
        //save_file(filenameD,save_temproot[count_second_index],MAX_SPOTS);
        //save_file(filenameE,save_temproot[count_third_index],MAX_SPOTS);
    }
    fclose(fp);
    
    printf("excuse: %f sec\n", (double)(time2 - time1) / CLOCKS_PER_SEC);
    printf("bye\n");
    //解の保存．
    printf("min:%f max:%f\n", min, max);
    
    return(0);
}