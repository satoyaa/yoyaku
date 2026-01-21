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
double crossover_rate;
double mutation_rateS = 0.05;
double mutation_rateR = 0.05;
double mutation_rateN;
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
int count;
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
    //unsigned int seed = 1766965875; //デバッグ用に固定した値を出したい 1000でバグる
    srand(seed);
    initialize(start, goal); //(start, goal)
    //printf("initialization done.\n");
    calc_fitness(start, goal);
    savemode = 0;
    local_search_mode = 0;
    //printf("initial fitness calculation done.\n");
    printf("crossover_rate:%f mutation_rate:%f ", crossover_rate, mutation_rateN);
    debug=0;
    for (int i = 0; i < MAX_ITERATION; i++)
    {   
        
        if (i==10000)
        {
            debug=1;
        }else{
            debug=0;
        }
        //printf("GA:%d/%d iteration:%d seed:%u ",progress+1, count, i, seed);
        //選択
        selection_tournament();
        //printf("selection done, fitness is %f, ", fitness[0]);
        saveIteration[i]+=fitness[0];
        increase_rate = fitness[0]-fitnessLog;
        fitnessLog = fitness[0];
        //交叉
        //crossover_pmx();
        crossover_twopoint();
        //printf("crossover done, ");
        //突然変異
        mutation_swap();
        mutation_random();
        //mutation_newpop(start, goal);
        //mutation_NewBranch();
        //mutation_NewBranch2();
        mutation_NewBranch3();
        //printf("mutation done, ");
        //評価値計算
        calc_fitness(start, goal);
        
    }
    //printf("\n");
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
    /* initialize(0, MAX_SPOTS-1);
    for (int i = 0; i < POPULATION; i++)
    {
        //printf("spot%d value is %d\n", i, spots[i].value);
    }
    
    */
    //GAのパラメータ調整

    TIMELIMIT = 240;
    
    double crossover_rates[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    double mutation_rates[] = {0.005, 0.01, 0.02, 0.05, 0.1, 0.2, 0.5, 1.0};

    double best_crossover_rate = 0;
    double best_mutation_rate = 0;
    double best_fitness = 0;
    
    useLocalResearch = 0;
    reserve_rate = 1;
    const char* filename = "Result/Adjust.txt";
    count = 30;
    FILE* fp = fopen(filename, "w"); 
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if(i==0&&j==0){continue;}
            double sum_best = 0;
            crossover_rate = crossover_rates[i];
            mutation_rateN = mutation_rates[j];
            char filename[20];        
            char line[256]; 
            double save_best[count];        
            sprintf(filename, "Result/GaResult_%2f_%2f.txt", crossover_rates[i], mutation_rates[j]);
            for (int k = 0; k < MAX_ITERATION; k++)
            {
                saveIteration[k]=0;
            }
            for (int k = 0; k < count; k++)
            {
                progress = k;
                printf("GA:%d/%d ", k, count);
                ga(0, MAX_SPOTS-1);
                sum_best += best;
                save_best[j] = best;  
            }
            printf("\n");
            if(sum_best/count > best_fitness){
                //スコアを更新
                best_fitness = sum_best/count;
                best_crossover_rate = crossover_rates[i];
                best_mutation_rate = mutation_rates[j];
            }
            double standard = 0;
            for (int j = 0; j < count; j++){
                standard += (save_best[j]-sum_best/count)*(save_best[j]-sum_best/count/count);
            }
            double s = 0;
            s = sqrt(standard/count);

            snprintf(line, sizeof(line), "%f %f %f %f\n",sum_best/count, s, crossover_rates[i], mutation_rates[j]);
            fprintf(fp, "%s", line);

            FILE *fpI = fopen(filename, "w"); // 書き込みモードで開く
            char lineI[256];
            for (int k = 0; k < MAX_ITERATION; k++)
            {
                snprintf(lineI, sizeof(lineI), "%d %f\n",k, saveIteration[k]/count);
                fprintf(fpI, "%s", lineI);
            }
            
            fclose(fpI);
        }
    }     
    char line[256]; 
    snprintf(line, sizeof(line), "best : %f, crossover rate : %f, mutation rate : %f\n", best_fitness, best_crossover_rate, best_mutation_rate);
    fprintf(fp, "%s", line);

    printf("best : %f, crossover rate : %f, mutation rate : %f", best_fitness, best_crossover_rate, best_mutation_rate);
    
    fclose(fp);
    /*
    printf("excuse: %f sec\n", (double)(time2 - time1) / CLOCKS_PER_SEC);
    printf("bye\n");
    //解の保存．
    printf("min:%f max:%f\n", min, max);
    */
    return(0);
}