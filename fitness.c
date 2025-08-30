#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "extern.h"

int poisson_rand(double lambda) {
    double L = exp(-lambda);
    int k = 0;
    double p = 1.0;
    do {
        k++;
        p *= (double)rand() / RAND_MAX;
    } while (p > L);
    return k - 1;
}

int binomial_rand(int n, double p) {
    int x = 0;
    for (int i = 0; i < n; i++) {
        if ((double)rand() / RAND_MAX < p) x++;
    }
    return x;
}

int calc_travel_time(int i, int j){
    double dx = spots[i].coordinate_x - spots[j].coordinate_x;
    double dy = spots[i].coordinate_y - spots[j].coordinate_y;
    double dist = sqrt(dx*dx + dy*dy);
    int t = (int)(dist / (SPEED*1000/60));  //<===============ワンちゃんダメかも
    return(t);
}


void calc_queue_range(int minutes){
    for (int k = 0; k < MAX_NODES; k++)
    {
        double lambda_per_minute = spots[k].crow / 60; //到着率　1時間当たりの平均来客者数が分かりやすい？
        double mu_per_minute = 1 / spots[k].t; //サービス率？言葉の定義あってるか不明
        int servers = spots[k].capacity;  //客席数  <=================新しいデータ項目を追加予定
        int busy = queue_range[k];   //サービス中の人数
        double p_complete = 1.0 - exp(-mu_per_minute);
        for (int t = 0; t < minutes; t++) {
            /* 到着 */
            int arrivals = poisson_rand(lambda_per_minute);
            queue_range[k] += arrivals;

            /* サービス完了 */
            int completions = binomial_rand(busy, p_complete);
            busy -= completions;

            /* 空きサーバーに待ち行列を割り当て */
            int free_servers = servers - busy;
            int to_start = (queue_range[k] < free_servers) ? queue_range[k] : free_servers;
            busy += to_start;
            queue_range[k] -= to_start;
        }   
    }
}

void calc_fitness(){
    for (int i = 0; i < POPULATION; i++)
    {
        int duration = 0;
        //初期化.
        for (int j = 0; j < MAX_NODES; j++)
        {
            queue_range[j] = spots[j].capacity;  //待ち人数の初期値として席数を入れる(満席状態からスタートも少し違和感があるが)
        }

        for (int j = 0; j < MAX_NODES; j++)
        {
            int minutes = 0;
            minutes += queue_range[j] / (spots[genes[i][j]].capacity * spots[genes[i][j]].t);    //<==============ワンちゃんバグる
            minutes += calc_travel_time(genes[i][j], genes[i][j+1]); 
            if (duration + minutes> TIMELIMIT)
            {
                duration += (calc_travel_time(genes[i][j], genes[i][MAX_NODES-1])+queue_range[j] / (spots[genes[i][j]].capacity * spots[genes[i][j]].t));
                break;
            }
            calc_queue_range(minutes);
            duration += minutes;
        }
        fitness[i] = duration;
    }
}