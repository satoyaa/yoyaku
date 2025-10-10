#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "extern.h"

int waiting_queue[MAX_SPOTS]; // 待ち行列（純粋な待ち人数）
int busy_servers[MAX_SPOTS];  // サービス中の人数

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
    for (int k = 0; k < MAX_SPOTS; k++) {
        // パラメータの計算
        double lambda_per_minute = spots[k].crow / 60.0; // 1分あたりの平均到着率 (λ)
        double mu_per_minute = 1.0 / spots[k].t;         // 1分あたりの平均サービス率 (μ)
        int servers = spots[k].capacity;                 // 窓口数 (s)
        
        // 1分間にサービスが完了する確率（離散時間近似）
        double p_complete = 1.0 - exp(-mu_per_minute);

        // 初期化（シミュレーション開始前に外部で行われるべきだが、ここではループ開始時に初期化）
        // waiting_queue[k] と busy_servers[k] は外部で初期化されていると仮定
        // 例: waiting_queue[k] = 0; busy_servers[k] = 0;

        for (int t = 0; t < minutes; t++) {
            
            // 1. サービス完了（システムからの退場）
            // サービス中の人数から、完了した人数を二項分布で決定
            int completions = binomial_rand(busy_servers[k], p_complete);
            // 完了した人数分、サービス中から減らす
            busy_servers[k] -= completions;

            // 2. 待ちのサービス開始（待ち行列からサービス中へ移動）
            int free_servers = servers - busy_servers[k];
            
            // 待ち人数と空き窓口の数の少ない方を、サービス開始人数とする
            int to_start = (waiting_queue[k] < free_servers) ? waiting_queue[k] : free_servers;
            
            // サービス中の人数を増やし、待ち行列の人数を減らす
            busy_servers[k] += to_start;
            waiting_queue[k] -= to_start;

            // 3. 到着（待ち行列への加算）
            // 1分間に到着する人数をポアソン乱数で決定
            int arrivals = poisson_rand(lambda_per_minute);
            // 到着した人数は全て、待ち行列に追加される
            waiting_queue[k] += arrivals;

            // ★ シミュレーション結果の記録（例えば、毎分の waiting_queue[k] を保存など）
        }
    }
}

void calc_fitness(){
    //printf("hello5\n");
    for (int i = 0; i < POPULATION; i++)
    {
        //初期化.
        int sum = 0;
        int sum_duration = 0;  //デバッグ用
        int sum_satisdy = 0;   //デバッグ用
        if (savemode)
        {
            //確率計算用の配列を初期化
            for (int j = 0; j < LOOPS; j++)
            {
                for (int k = 0; k < MAX_SPOTS; k++)
                {
                    save_temproot[j][k] = -1;
                } 
                count_temproot[j] = 0;
            }
        }
        
        for (int j = 0; j < LOOPS; j++)
        {
            int duration = 0; //時間計算用
            int satisfy = 0; //満足度計算
            int next_node;
            int pivot_node = genes[i][0];
            int expect;
            float limit = INFINITY; //予約時間計算
            int reserve_spot = -1; //予約地点
            int temp_root[MAX_SPOTS];
            int temp_index = 0; //temp_root用index
            //srand((unsigned int)time(NULL));
            //srand(0);
            //printf("rand:%d\n",rand());
            for (int k = 0; k < MAX_SPOTS; k++)
            {
                waiting_queue[k] = spots[k].capacity;  //待ち人数の初期値として席数を入れる(満席状態からスタートも少し違和感があるが)
                temp_root[k] = -1;
            }
            //スタートを入れる
            temp_root[temp_index] = genes[i][0];
            temp_index++;
            //待ち時間を30分だけシミュレーション
            calc_queue_range(30);
            //評価値計算のメイン
            for (int k = 1; k < MAX_SPOTS-1; k++)
            {   
                int next_node = k;
                //printf("k:%d pivot:%d next:%d sat:%d dur:%d reserve:%d\n", k, genes[i][pivot_node], genes[i][next_node], satisfy, duration, reserve_spot);
                
                if (times[i][next_node] < duration)
                {
                    continue;
                }
                //printf("%d ", duration);
                temp_root[temp_index] = genes[i][k];
                temp_index++;
                int minutes = 0;
                
                //待ち時間を計算
                int wait = waiting_queue[genes[i][pivot_node]] * spots[genes[i][pivot_node]].t / (spots[genes[i][pivot_node]].capacity );
                //予約観光地では待ち時間0，予約時間まで待機．
                if(k==reserve_spot){
                    if(duration <= limit){duration = limit;wait=0;} //早く着いたら待つ
                    limit = INFINITY;
                }
                //待ち時間シミュレータ確認用
                if(0){
                    printf("%dmin : ",duration);
                    for (int l = 0; l < MAX_SPOTS; l++)
                    {
                        int waits = waiting_queue[l];
                        //int waits = waiting_queue[l] * spots[l].t / spots[l].capacity;
                        printf("%d ", waits);
                    }
                    printf("\n");
                }
                
                //店での所要時間＋待ち時間
                minutes += wait;
                minutes += spots[genes[i][pivot_node]].t;
                //もし待ち時間+所要時間が制限時間をオーバーするならゴールに向かう
                if(duration+minutes > TIMELIMIT){break;}
                duration+=minutes;
                //待ち時間シミュレーション（出発時）予約確認用
                calc_queue_range(minutes);
                minutes = 0; //シミュレーションが終わったので時間リセット
                //予約処理
                for (int l = k+1; l < MAX_SPOTS; l++)
                {
                    if (genes_reserves[i][l].spot == 1)
                    {
                        reserve_spot = l;
                        limit = genes_reserves[i][l].time;
                        break;
                    }
                }
                

                //ここで次の観光地を回っても予約観光地に間に合うかを予想
                expect = duration + calc_travel_time(genes[i][pivot_node], genes[i][next_node]) + spots[genes[i][next_node]].t + calc_travel_time(genes[i][next_node], genes[i][MAX_SPOTS-1]);

                //次の観光地を回っても予約に間に合うか判定
                //間に合わない場合の処理
                //観光地の巡回を諦めて予約地点に向かう
                if(expect > limit && duration + calc_travel_time(genes[i][pivot_node], genes[i][reserve_spot]) < limit){ 
                    minutes += calc_travel_time(genes[i][pivot_node], genes[i][reserve_spot]); 
                    //現在時刻の待ち時間を算出（到着時）
                    calc_queue_range(minutes);
                    duration += minutes;
                    satisfy += spots[genes[i][reserve_spot]].value;
                    pivot_node = reserve_spot;
                    k = reserve_spot;
                    continue;
                }else if (expect > limit) //予約破棄 悪いことなのでペナルティ
                {
                    reserve_spot = -1;
                    limit = INFINITY;
                    satisfy -= spots[genes[i][reserve_spot]].penalty;
                }
                

                //ここで次の観光地を回ってもゴールに間に合うかを予想．
                // 出発時間+移動時間+次観光地の所要時間+ゴールまでの移動時間の予想 
                expect = duration + calc_travel_time(genes[i][pivot_node], genes[i][next_node]) + spots[genes[i][next_node]].t + calc_travel_time(genes[i][next_node], genes[i][MAX_SPOTS-1]);

                //出発前にゴールに間に合うか判定
                if(expect > TIMELIMIT){
                    break;
                }

                //移動時間
                minutes += calc_travel_time(genes[i][pivot_node], genes[i][next_node]); 
                
               
                //現在時刻の待ち時間を算出（到着時）
                calc_queue_range(minutes);
                duration += minutes;
                satisfy += spots[genes[i][pivot_node]].value;
                pivot_node = next_node;
            }
            //printf("\n");

            //printf("hello1\n");
            //終了後ゴールまでの経路を入れる
            duration+=calc_travel_time(genes[i][pivot_node], genes[i][MAX_SPOTS-1]); 
            temp_root[temp_index] = genes[i][MAX_SPOTS-1];
            temp_index++;
            
            if (duration > TIMELIMIT)
            {
                satisfy -= 100;
            }
            //評価値が最低であればsaveを更新
            //printf("hello2\n");
            if (satisfy < min || i == 0 || savemode)
            {
                for (int k = 0; k < MAX_SPOTS; k++)
                {
                    save_minroot[k] = -1;
                }
                for (int k = 0; k < MAX_SPOTS; k++)
                {
                    save_minroot[k] = temp_root[k];
                }
                min = satisfy;
            }
            //評価値が最大であれば経路を保存
            //printf("hello3\n");
            if (max < satisfy || i == 0 || savemode)
            {
                for (int k = 0; k < MAX_SPOTS; k++)
                {
                    save_maxroot[k] = -1;
                }
                for (int k = 0; k < MAX_SPOTS; k++)
                {
                    save_maxroot[k] = temp_root[k];
                }
                max = satisfy;
            }
            //tempルートを保存し発生確率を計算
            
            if (savemode)
            {
                int index = -1;
                int flag = 0;
                //デバック用
                if(0){
                    printf("duration:%d\n",duration);
                    printf("temproot:");
                    for (int k = 0; k < MAX_SPOTS; k++)
                    {
                        printf("%d ", temp_root[k]);
                    }
                    printf("\n");
                }
                
                
                for (int k = 0; k < LOOPS; k++)
                {
                    //一致しない場合,-1をindexに
                    if(save_temproot[k][0]==-1){index=k;break;}
                    //一致判定
                    for (int l = 0; l < MAX_SPOTS; l++)
                    {
                        if(save_temproot[k][l] != temp_root[l]){flag=0;break;}
                        flag = 1;
                    }
                    //一致するなら終わり
                    if(flag){index = k;break;}
                }
                count_temproot[index] += 1;
                //printf("root:");
                for (int k = 0; k < MAX_SPOTS; k++)
                {
                    //printf("%d ",temp_root[k]);
                    save_temproot[index][k] = temp_root[k];
                }
                //printf("\n");
            }
            
            sum+= satisfy;
            sum_duration+=duration;
            sum_satisdy+=satisfy;
        }
        fitness[i] = sum / LOOPS;
        //printf("duration is %d, satisfy is %d\n", sum_duration/LOOPS, sum_satisdy/LOOPS);
    }
}

