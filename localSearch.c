#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "extern.h"

double deg2rad1(double deg) {
  return deg * M_PI / 180.0;
}

int calc_travel_time1(int i, int j){
    double x1 = spots[i].coordinate_x;
    double x2 = spots[j].coordinate_x;
    double y1 = spots[i].coordinate_y;
    double y2 = spots[j].coordinate_y;
    x1 = deg2rad1(x1);
    x2 = deg2rad1(x2);
    y1 = deg2rad1(y1);
    y2 = deg2rad1(y2);
    //double dx = spots[i].coordinate_x - spots[j].coordinate_x;
    //double dy = spots[i].coordinate_y - spots[j].coordinate_y;
    double dist = EARTH_RAD * acos(sin(y1) * sin(y2) + cos(y1) * cos(y2) * cos(x2 - x1));
    int t = (int)(dist *60 / SPEED);  //<===============ワンちゃんダメかも
    return(t);
}

double factorial(int n) {
    double result = 1.0;
    for (int i = 2; i <= n; i++) result *= i;
    return result;
}

// M/M/c モデルの平均待ち時間 Wq を計算
double mmc_waiting_time(double lambda, int c, double service_time) {
    double mu = 1.0 / service_time;   // サービス率 μ
    double rho = lambda / (c * mu);   // 利用率 ρ
    double lambda_mu = lambda / mu;   // λ/μ

    // ρ >= 1 の場合、待ち時間は無限大
    if (rho >= 1.0) {
        return INFINITY;
    }

    // --- P0 の計算 ---
    double sum = 0.0;
    for (int n = 0; n < c; n++) {
        sum += pow(lambda_mu, n) / factorial(n);
    }

    double last_term = pow(lambda_mu, c) / (factorial(c) * (1.0 - rho));
    double P0 = 1.0 / (sum + last_term);

    // --- Lq の計算 ---
    double Lq = (pow(lambda_mu, c) * rho) /
                (factorial(c) * pow(1.0 - rho, 2.0)) * P0;

    // --- 平均待ち時間 Wq ---
    double Wq = Lq / lambda;

    return Wq;
}

//局所探索の実装
//終了時刻までに訪問できる観光地の数を増やす
void local_search(int start, int goal){
    //遺伝子長を計算
    int length = 0;
    for (int j = 0; j < MAX_NODES; j++)
    {
        if(genes[0][j].vert==-1){break;}
        length+=1;
    }
    if (1-reserve_rate)
    {
        return;
    }
    
    if (length<=0)
    {
        printf("genes : ");
        for (int i = 0; i < MAX_NODES; i++)
        {
            printf("%d ", genes[0][0].vert);
        }
        printf("\n");
    }
    
    //局所探索の実装
    //予約時刻を最適化
    //遺伝子長を前から順に辿っていき，予約観光地があれば予約時刻を少しずつ変化させて評価値を計算
    int duration = 0; //時間計算用
    int dest; //満足度計算
    int done = 1;   
    int temp_dest[MAX_NODES];
    for (int j = 0; j < length; j++){
        temp_dest[j] = genes[0][j].dest;
    }
    int a = 0;
    
    
    //予約時刻の正規化
    while (done)
    {
        if(0>=length){break;}
        dest = 10000;
        duration = calc_travel_time1(start, genes[0][0].vert);
        duration += spots[start].t;
        for (int j = 0; j < length; j++){
            if(j>dest){
                for (int k = 0; k < MAX_NODES; k++)
                {
                    if(dest==temp_dest[k]){temp_dest[k]=-1; break;}
                }
                break;
            }
            if(genes[0][j].dest!=-1 && temp_dest[j]!=-1){
                dest=temp_dest[j];
                printf("destlocal %d length %d j %d\n",dest, length , j);
            }else if(genes[0][j].dest!=-1 && temp_dest[j]==-1){
                printf("break at %d %d\n",j, temp_dest[j]);
                j=genes[0][j].dest;
                printf("break at %d\n",j);
            }
            if(genes_reserves[0][genes[0][j].vert].spot != 1){
                duration += spots[genes[0][j].vert].t+mmc_waiting_time(spots[genes[0][j].vert].crow, spots[genes[0][j].vert].capacity, spots[genes[0][j].vert].t);
                duration += calc_travel_time1(genes[0][j].vert, genes[0][j+1].vert);
                if (j==length-1)
                {
                    done=0;
                }
                continue;
            } //予約観光地でなければスキップ
            genes_reserves[0][genes[0][j].vert].time = duration;
            duration += spots[genes[0][j].vert].t+mmc_waiting_time(spots[genes[0][j].vert].crow, spots[genes[0][j].vert].capacity, spots[genes[0][j].vert].t);
            duration += calc_travel_time1(genes[0][j].vert, genes[0][j+1].vert);
            if (j==length-1)
            {
                done=0;
            }
        }
    }
    //正規化した予約時刻で局所探索を計算
    //エリート個体をコピー
    for (int i = 0; i < POPULATION; i++)
    {
        for (int j = 0; j < MAX_NODES; j++)
        {
            genes[i][j].vert = genes[0][j].vert;
            genes[i][j].time = genes[0][j].time;
            genes[i][j].dest = genes[0][j].dest;
            if(j>=MAX_SPOTS){continue;}
            genes_reserves[i][j].spot = genes_reserves[0][j].spot;
            genes_reserves[i][j].time = genes_reserves[0][j].time;  
        }
    }
    for (int i = 0; i < POPULATION; i++)
    {
        for (int j = 0; j < MAX_SPOTS; j++)
        {
            if(genes_reserves[i][j].spot != 1){continue;} //予約観光地でなければスキップ
            genes_reserves[i][j].time = genes_reserves[i][j].time + (rand()%11 - 5); //-5分から+5分の範囲でランダムに変更
        }
    }
    calc_fitness(start, goal);
}