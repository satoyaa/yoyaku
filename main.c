#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "extern.h"

Reserve genes_reserves[POPULATION][MAX_SPOTS];
Gene genes[POPULATION][MAX_NODES];
Spot spots[MAX_SPOTS];
double fitness[POPULATION];
int queue_range[MAX_SPOTS];
Save save_maxroot[MAX_NODES];
Save save_minroot[MAX_NODES];
Save save_temproot[LOOPS][MAX_NODES];
int count_temproot[LOOPS];
double crossover_rate = 0.5;
double mutation_rateS = 0.05;
double mutation_rateR = 0.05;
double mutation_rateN = 0.2;
int TIMELIMIT;
double min = INFINITY; //処理の都合で今代入
double max = -INFINITY; //処理の都合で今代入
double best;
int reserve_rate;
int savemode;
int local_search_mode;
int useLocalResearch;
double saveIteration[MAX_ITERATION];
int debug;
int count = 10;
int progress;
double fitnessLog=0;
double increase_rate=0;

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
        //printf("%d\n", array[i].vert);
        //printf("%d %f %f %d %d %d\n",array[i].vert, spots[array[i].vert].coordinate_x, spots[array[i].vert].coordinate_y, array[i].time, genes_reserves[0][array[i].vert].spot, genes_reserves[0][array[i].vert].spot);
        snprintf(line, sizeof(line), "%d %f %f %d %d %d\n",array[i].vert, spots[array[i].vert].coordinate_x, spots[array[i].vert].coordinate_y, array[i].time, genes_reserves[0][array[i].vert].spot, genes_reserves[0][array[i].vert].spot);
        fprintf(fp, "%s", line);
    }
    fclose(fp);
    
}



void ga(int start, int goal){
    
    unsigned int seed = (unsigned int)time(NULL); //実行毎に違うを出したい
    //unsigned int seed = 1768204878; //デバッグ用に固定した値を出したい 1000でバグる
    //printf("seed:%u\n", seed);
    srand(seed);
    initialize(start, goal); //(start, goal)
    printf("initialization done.\n");
    calc_fitness(start, goal);
    savemode = 0;
    local_search_mode = 0;
    printf("initial fitness calculation done.\n");
    debug=0;
    for (int i = 0; i < MAX_ITERATION; i++)
    {   
        if (i==10000)
        {
            debug=1;
        }else{
            debug=0;
        }
        printf("GA:%d/%d iteration:%d seed:%u ",progress+1, count, i, seed);
        //選択
        selection_tournament();
        printf("selection done, fitness is %f, ", fitness[0]);
        saveIteration[i]+=fitness[0];
        increase_rate = fitness[0]-fitnessLog;
        fitnessLog = fitness[0];
        //交叉
        //crossover_pmx();
        crossover_twopoint();
        printf("crossover done, ");
        //突然変異
        mutation_swap();
        mutation_random();
        //mutation_newpop(start, goal);
        mutation_NewBranch();
        printf("mutation done, ");
        //評価値計算
        calc_fitness(start, goal);
        printf("calculate fitness done.\n");
        
        
        
    }
    selection_tournament();
    calc_fitness(start, goal);
    //local_search(start, goal);
    selection_tournament();
    if(useLocalResearch==1){
        local_search_ultimate(start, goal);
        //local_search_binary(start, goal);
    }
    if(useLocalResearch==2){
        //local_search_ultimate(start, goal);
        local_search_binary(start, goal);
    }
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
    for (int i = 0; i < MAX_SPOTS; i++)
    {
        //printf("spot%d value is %d\n", i, spots[i].value);
    }
    
    //初期個体生成．
    
    //評価値計算．
    //printf("hello\n");
    clock_t time1, time2;
    time1 = clock();   
    //ga(0, MAX_SPOTS-1);
    //serchAll(int start)
    //calc_fitness();
    time2 = clock();
    for (int i = 0; i < POPULATION; i++)
    {
        //printf("fitness is %f\n", fitness[i]);
    }
    //自動実験プログラム
    int time[] = {60, 120, 180, 240, 300, 360};
    const char* filename = "root/Result.txt";
    FILE* fp = fopen(filename, "w"); 
    
    for (int i = 1; i < 3; i++)
    {
        reserve_rate = 1;
        useLocalResearch = 0;
        TIMELIMIT = time[i];
        char line[256];
        char filenameA[20];
        char filenameB[20];
        char filenameC[20];
        char filenameD[20];
        char filenameE[20];
        char filenameF[20];
        sprintf(filenameA, "root/max_test%d.txt", time[i]);
        sprintf(filenameB, "root/min_test%d.txt", time[i]);
        sprintf(filenameC, "root/freqent1_%d.txt", time[i]);
        sprintf(filenameD, "root/freqent2_%d.txt", time[i]);
        sprintf(filenameE, "root/freqent3_%d.txt", time[i]);
        sprintf(filenameF, "root/GaResult_%d.txt", time[i]);
        double sum_max = 0;
        double sum_min = 0;
        double sum_average = 0;
        double sum_time = 0;
        Save maxroot[MAX_NODES];
        Save minroot[MAX_NODES];
        Save bestFitness[LOOPS][MAX_NODES];
        int save_count_temproot[LOOPS];
        int save_best[count];        
        double best_max  = -INFINITY;
        double best_min = INFINITY;
        double fitness_max = -INFINITY;
        for (int j = 0; j < MAX_ITERATION; j++)
        {
            saveIteration[j]=0;
        }
        for (int j = 0; j < count; j++){
            save_best[j] = 0;
        }

        for (int j = 0; j < count; j++)
        {
            progress = j;
            printf("%d ", TIMELIMIT);
            time1 = clock();   
            ga(0, MAX_SPOTS-1);
            time2 = clock();
            save_best[j] = best;   
            sum_max += max;
            sum_min += min;
            sum_average += best;
            sum_time+=(double)(time2 - time1) / CLOCKS_PER_SEC;
            if (fitness_max < best)
            {
                fitness_max = best;
                for (int k = 0; k < MAX_NODES; k++)
                {
                    minroot[k].vert = save_minroot[k].vert;
                    minroot[k].time = save_minroot[k].time;
                }
                for (int k = 0; k < MAX_NODES; k++)
                {
                    maxroot[k].vert = save_maxroot[k].vert;
                    maxroot[k].time = save_maxroot[k].time;
                }
                for (int k = 0; k < LOOPS; k++)
                {
                    for (int l = 0; l < MAX_NODES; l++)
                    {
                        bestFitness[k][l].vert = save_temproot[k][l].vert;
                        bestFitness[k][l].time = save_temproot[k][l].time;
                    }
                    save_count_temproot[j] = count_temproot[j];
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
                printf("%d\n",save_count_temproot[j]);
            }
            if(count_max < save_count_temproot[j]){
                count_third=count_second;
                count_second=count_max;
                count_max=save_count_temproot[j];
                count_third_index=count_second_index;
                count_second_index=count_max_index;
                count_max_index=j;}
            else if (count_second < save_count_temproot[j]){
                count_third=count_second;
                count_second=save_count_temproot[j];
                count_third_index=count_second_index;
                count_second_index=j;}
            else if (count_third < save_count_temproot[j]){
                count_third=save_count_temproot[j];
                count_third_index=j;}
        }
        //ここまで
        
        double standard = 0;
        for (int j = 0; j < count; j++){
            standard += (save_best[j]-sum_average/count)*(save_best[j]-sum_average/count);
        }
        double s = 0;
        s = sqrt(standard/count);

        printf("most appeare root probabilty is %d\n",count_max);
        printf("max index is %d, second index is %d third index is %d",count_max_index, count_second_index, count_third_index);
        //printf("excuse: %f sec\n", sum_average/count);
        snprintf(line, sizeof(line), "%dX %f %f %f %f %d %d %d\n",time[i], sum_max/count, sum_min/count, sum_average/count, s, count_max, count_second, count_third);
        fprintf(fp, "%s", line);
        save_file(filenameA,maxroot,MAX_NODES);
        save_file(filenameB,minroot,MAX_NODES);
        save_file(filenameC,bestFitness[count_max_index],MAX_NODES);
        save_file(filenameD,bestFitness[count_second_index],MAX_NODES);
        save_file(filenameE,bestFitness[count_third_index],MAX_NODES);
        FILE *fpI = fopen(filenameF, "w"); // 書き込みモードで開く
        char lineI[256];
        for (int j = 0; j < MAX_ITERATION; j++)
        {
            snprintf(lineI, sizeof(lineI), "%d %f\n",j, saveIteration[j]/count);
            fprintf(fpI, "%s", lineI);
        }
        snprintf(lineI, sizeof(lineI), "%f %f %f %f\n",sum_max/count, sum_min/count, sum_average/count, s);
        fprintf(fpI, "%s", lineI);
        fclose(fpI);
    }
    for (int i = 3; i < 3; i++)
    {
        reserve_rate = 1;
        useLocalResearch = 1;
        TIMELIMIT = time[i];
        char line[256];
        char filenameA[20];
        char filenameB[20];
        char filenameC[20];
        char filenameD[20];
        char filenameE[20];
        char filenameF[20];
        sprintf(filenameA, "root/max_testL%d.txt", time[i]);
        sprintf(filenameB, "root/min_testL%d.txt", time[i]);
        sprintf(filenameC, "root/freqent1_L%d.txt", time[i]);
        sprintf(filenameD, "root/freqent2_L%d.txt", time[i]);
        sprintf(filenameE, "root/freqent3_L%d.txt", time[i]);
        sprintf(filenameF, "root/GaResult_L%d.txt", time[i]);
        double sum_max = 0;
        double sum_min = 0;
        double sum_average = 0;
        double sum_time = 0;
        Save maxroot[MAX_NODES];
        Save minroot[MAX_NODES];
        Save bestFitness[LOOPS][MAX_NODES];
        int save_count_temproot[LOOPS];
        double best_max  = -INFINITY;
        double best_min = INFINITY;
        double fitness_max = -INFINITY;
        for (int j = 0; j < MAX_ITERATION; j++)
        {
            saveIteration[j]=0;
        }
        for (int j = 0; j < count; j++)
        {
            progress = j;
            printf("%d ", TIMELIMIT);
            time1 = clock();   
            ga(0, MAX_SPOTS-1);
            time2 = clock();   
            sum_max += max;
            sum_min += min;
            sum_average += best;
            sum_time+=(double)(time2 - time1) / CLOCKS_PER_SEC;
            if (fitness_max < best)
            {
                fitness_max = best;
                for (int k = 0; k < MAX_NODES; k++)
                {
                    minroot[k].vert = save_minroot[k].vert;
                    minroot[k].time = save_minroot[k].time;
                }
                for (int k = 0; k < MAX_NODES; k++)
                {
                    maxroot[k].vert = save_maxroot[k].vert;
                    maxroot[k].time = save_maxroot[k].time;
                }
                for (int k = 0; k < LOOPS; k++)
                {
                    for (int l = 0; l < MAX_NODES; l++)
                    {
                        bestFitness[k][l].vert = save_temproot[k][l].vert;
                        bestFitness[k][l].time = save_temproot[k][l].time;
                    }
                    save_count_temproot[j] = count_temproot[j];
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
                printf("%d\n",save_count_temproot[j]);
            }
            if(count_max < save_count_temproot[j]){
                count_third=count_second;
                count_second=count_max;
                count_max=save_count_temproot[j];
                count_third_index=count_second_index;
                count_second_index=count_max_index;
                count_max_index=j;}
            else if (count_second < save_count_temproot[j]){
                count_third=count_second;
                count_second=save_count_temproot[j];
                count_third_index=count_second_index;
                count_second_index=j;}
            else if (count_third < save_count_temproot[j]){
                count_third=save_count_temproot[j];
                count_third_index=j;}
        }
        //ここまで
        
        printf("most appeare root probabilty is %d\n",count_max);
        printf("max index is %d, second index is %d third index is %d",count_max_index, count_second_index, count_third_index);
        //printf("excuse: %f sec\n", sum_average/count);
        snprintf(line, sizeof(line), "%d %f %f %f %d %d %d\n",time[i], sum_max/count, sum_min/count, sum_average/count, count_max, count_second, count_third);
        fprintf(fp, "%s", line);
        save_file(filenameA,maxroot,MAX_NODES);
        save_file(filenameB,minroot,MAX_NODES);
        save_file(filenameC,bestFitness[count_max_index],MAX_NODES);
        save_file(filenameD,bestFitness[count_second_index],MAX_NODES);
        save_file(filenameE,bestFitness[count_third_index],MAX_NODES);
        FILE *fpI = fopen(filenameF, "w"); // 書き込みモードで開く
        char lineI[256];
        for (int j = 0; j < MAX_ITERATION; j++)
        {
            snprintf(lineI, sizeof(lineI), "%d %f\n",j, saveIteration[j]/count);
            fprintf(fpI, "%s", lineI);
        }
        snprintf(lineI, sizeof(lineI), "%f %f %f\n",sum_max/count, sum_min/count, sum_average/count);
        fprintf(fpI, "%s", lineI);
        fclose(fpI);
    }
    fclose(fp);
    
    printf("excuse: %f sec\n", (double)(time2 - time1) / CLOCKS_PER_SEC);
    printf("bye\n");
    //解の保存．
    printf("min:%f max:%f\n", min, max);
    return(0);
}