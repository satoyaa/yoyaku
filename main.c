#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "extern.h"

Reserve genes_reserves[POPULATION][MAX_SPOTS];
Gene genes[POPULATION][MAX_SPOTS];
int times[POPULATION][MAX_SPOTS];
Spot spots[MAX_SPOTS];
double fitness[POPULATION];
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



void ga(int start, int goal){
    savemode = 0;
    srand((unsigned int)time(NULL));//実行毎に違うを出したい
    //srand(3);//実行毎に違うを出したい
    initialize(start, goal); //(start, goal)
    printf("initialization done.\n");
    calc_fitness(start, goal);
    printf("initial fitness calculation done.\n");
    for (int i = 0; i < MAX_ITERATION; i++)
    {
        printf("iteration:%d ",i);
        //選択
        selection_tournament();
        printf("selection done, ");
        //交叉
        //crossover_pmx();
        crossover_twopoint();
        printf("crossover done, ");
        //突然変異
        mutation_swap();
        printf("mutation done, ");
        //評価値計算
        calc_fitness(start, goal);
        printf("calculate fitness done.\n");
    }
    printf("\n");
    selection_tournament();
    min=INFINITY;
    max=-INFINITY;
    savemode = 1;
    calc_fitness(start, goal);
    best = fitness[0];
    printf("best:%f\n",best);
    
    
}

int main(){
    //データの読込．
    readdata("o-sakaFoodsX.txt");
    //初期個体生成．
    TIMELIMIT = 120;
    initialize(0, MAX_SPOTS-1);
    
    //評価値計算．
    //printf("hello\n");
    clock_t time1, time2;
    time1 = clock();   
    ga(0, MAX_SPOTS-1);
    //serchAll(int start)
    //calc_fitness();
    time2 = clock();
    for (int i = 0; i < POPULATION; i++)
    {
        printf("fitness is %f\n", fitness[i]);
    }
    //自動実験プログラム
    int time[] = {60, 120, 180, 240};
    const char* filename = "root/Result.txt";
    FILE* fp = fopen(filename, "w"); 
    for (int i = 2; i < 3; i++)
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
            ga(0, MAX_SPOTS-1);
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
        //printf("excuse: %f sec\n", sum_average/count);
        snprintf(line, sizeof(line), "%d %f %f %f %d %d %d\n",time[i], sum_max/count, sum_min/count, sum_average/count, count_max, count_second, count_third);
        fprintf(fp, "%s", line);
        /*
        save_file(filenameA,maxroot,MAX_SPOTS);
        save_file(filenameB,minroot,MAX_SPOTS);
        save_file(filenameC,save_temproot[count_max_index],MAX_SPOTS);
        save_file(filenameD,save_temproot[count_second_index],MAX_SPOTS);
        save_file(filenameE,save_temproot[count_third_index],MAX_SPOTS);*/
        printf("all task done");
        
    }
    for (int i = 2; i < 3; i++)
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
        int count = 10;
        for (int j = 0; j < count; j++)
        {
            printf("%d done\n",j * 10);
            time1 = clock();   
            ga(0, MAX_SPOTS-1);
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
        //printf("excuse: %f sec\n", sum_average/count);
        snprintf(line, sizeof(line), "%dX %f %f %f %d %d %d\n",time[i], sum_max/count, sum_min/count, sum_average/count, count_max, count_second, count_third);
        fprintf(fp, "%s", line);
        /*
        save_file(filenameA,maxroot,MAX_SPOTS);
        save_file(filenameB,minroot,MAX_SPOTS);
        save_file(filenameC,save_temproot[count_max_index],MAX_SPOTS);
        save_file(filenameD,save_temproot[count_second_index],MAX_SPOTS);
        save_file(filenameE,save_temproot[count_third_index],MAX_SPOTS);*/
    }
    fclose(fp);
    
    printf("excuse: %f sec\n", (double)(time2 - time1) / CLOCKS_PER_SEC);
    printf("bye\n");
    //解の保存．
    printf("min:%f max:%f\n", min, max);
    return(0);
}