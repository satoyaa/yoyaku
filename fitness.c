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
    for (int k = 0; k < MAX_SPOTS; k++)
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
        //初期化.
        int sum = 0;
        int sum_duration = 0;  //デバッグ用
        int sum_satisdy = 0;   //デバッグ用
        for (int j = 0; j < LOOPS; j++)
        {
            int duration = 0;
            int satisfy = 0;
            float limit = INFINITY;
            int reserve_spot = -1;
            srand((unsigned int)time(NULL));
            for (int k = 0; k < MAX_SPOTS; k++)
            {
                queue_range[k] = spots[k].capacity;  //待ち人数の初期値として席数を入れる(満席状態からスタートも少し違和感があるが)
            }
            for (int k = 0; k < MAX_SPOTS-1; k++)
            {
                //if(genes_reserves[i][k]!=-1){reserve_spot=genes_reserves[i][k];limit=duration + queue_range[reserve_spot] / (spots[genes[i][reserve_spot]].capacity * spots[genes[i][reserve_spot]].t);}
                if(genes_reserves[i][k]!=-1){reserve_spot=genes_reserves[i][k];limit=duration*1.5;} //時間計算　一時的
                //if(limit == 0){limit == duration + calc_travel_time(genes[i][k], genes[i][reserve_spot]);} //待ち時間0の場合をまた考える
                int minutes = 0;
                int wait = queue_range[k] / (spots[genes[i][k]].capacity * spots[genes[i][k]].t);
                if(k==reserve_spot){
                    if(duration <= limit){duration = limit;wait=0;} //早く着いたら待つ
                    limit = INFINITY;
                }
                minutes += wait + spots[genes[i][k]].t;
                //printf("hello2 %d\n", k);
                minutes += calc_travel_time(genes[i][k], genes[i][k+1]); 
                //printf("limit:%f k:%d reserve:%d \n", limit, k, reserve_spot);
                if(duration + minutes > limit){
                    duration += (calc_travel_time(genes[i][k], genes[i][reserve_spot])+queue_range[k] / (spots[genes[i][k]].capacity * spots[genes[i][k]].t));
                    k = reserve_spot-1;
                    continue;
                }
                if (duration + minutes > TIMELIMIT)
                {
                    duration += (calc_travel_time(genes[i][k], genes[i][MAX_SPOTS-1])+queue_range[k] / (spots[genes[i][k]].capacity * spots[genes[i][k]].t));
                    break;
                }
                calc_queue_range(minutes);
                //printf("hello4\n");
                duration += minutes;
                satisfy += spots[genes[i][k]].value;
            }
            if (duration > TIMELIMIT)
            {
                satisfy -= 50;
            }
            
            sum+= satisfy;
            sum_duration+=duration;
            sum_satisdy+=satisfy;
        }
        fitness[i] = sum / LOOPS;
        printf("duration is %d, satisfy is %d\n", sum_duration/LOOPS, sum_satisdy/LOOPS);
    }
}

