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

double deg2rad(double deg) {
  return deg * M_PI / 180.0;
}

int calc_travel_time(int i, int j){
    double x1 = spots[i].coordinate_x;
    double x2 = spots[j].coordinate_x;
    double y1 = spots[i].coordinate_y;
    double y2 = spots[j].coordinate_y;
    x1 = deg2rad(x1);
    x2 = deg2rad(x2);
    y1 = deg2rad(y1);
    y2 = deg2rad(y2);
    //double dx = spots[i].coordinate_x - spots[j].coordinate_x;
    //double dy = spots[i].coordinate_y - spots[j].coordinate_y;
    double dist = EARTH_RAD * acos(sin(y1) * sin(y2) + cos(y1) * cos(y2) * cos(x2 - x1));
    int t = (int)(dist *60 / SPEED);  //<===============ワンちゃんダメかも
    return(t);
}


void calc_queue_range(){
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
    }
}

void calc_fitness(int start, int goal){

    //初期化.
    int sum = 0;
    int sum_duration = 0;  //デバッグ用
    int sum_satisfy = 0;   //デバッグ用
    if (savemode)
    {
        //確率計算用の配列を初期化
        for (int j = 0; j < LOOPS; j++)
        {
            for (int k = 0; k < MAX_SPOTS; k++)
            {
                save_temproot[j][k].vert = -1;
                save_temproot[j][k].time = 0;
                save_temproot[j][k].reserve = -1;
                save_temproot[j][k].reservetimes = 0;
            } 
            count_temproot[j] = 0;
        }
    }
    /*
    for (int j = 0; j < MAX_NODES; j++)
    {
        printf("%d ", genes[j].vert);
        if(genes[j].vert==-1){break;}
    }
    printf("\n");
    for (int j = 0; j < MAX_NODES; j++)
    {
        printf("%d ", genes[j].dest);
    }
    printf("\n");*/
    for (int j = 0; j < LOOPS; j++)
    {
        int duration = 0; //時間計算用
        int satisfy = 0; //満足度計算
        int next_index = 0;
        int pivot_index = start;
        int dest_node = MAX_NODES-1;
        int temp_index = 0;
        int expect;
        Save temp_root[MAX_NODES];
        int use_reserve[MAX_SPOTS];
        for (int k = 0; k < MAX_SPOTS; k++)
        {
            use_reserve[k] = genes_reserves[k].spot;
            waiting_queue[k] = spots[k].capacity;  //待ち人数の初期値として席数を入れる(満席状態からスタートも少し違和感があるが)
        }
        //srand((unsigned int)time(NULL));
        //srand(0);
        //printf("rand:%d\n",rand());
        for (int k = 0; k < MAX_NODES; k++)
        {
            temp_root[k].vert = -1;
            temp_root[k].time = 0;
            temp_root[k].reserve = -1;
            temp_root[k].reservetimes = 0;
        }
        temp_index++;
        //待ち時間をシミュレーション
        calc_queue_range();
        //評価値計算のメイン
        for (int k = 1; k < MAX_NODES-1; k++)
        {   
            
            //間違って範囲外にアクセスした場合の対応
            if(genes[pivot_index].vert == -1){break;}
            
            
            

            //printf("%d ", duration);
            temp_root[temp_index].vert = genes[pivot_index].vert;
            temp_root[temp_index].time = genes[pivot_index].vert;
            temp_root[temp_index].reserve = genes_reserves[pivot_index].spot;
            temp_root[temp_index].reservetimes = genes_reserves[pivot_index].time;
            temp_index++;
            int minutes = 0;
            
            //待ち時間を計算
            //printf("k:%d pivot:%d next:%d sat:%d dur:%d\n", k, genes[pivot_index, genes[next_index].vert, satisfy, duration);
            int wait = waiting_queue[genes[pivot_index].vert] * spots[genes[pivot_index].vert].t / (spots[genes[pivot_index].vert].capacity );
            //printf("hello2\n");
            //予約観光地では待ち時間0，早く着いた場合は予約時間まで待機．また，時間に間に合った場合は予約を使用，間に合わなかった場合はちょっとだけペナルティ
            if(use_reserve[genes[pivot_index].vert]==1){
                if(duration <= genes_reserves[genes[pivot_index].vert].time){
                    duration = genes_reserves[genes[pivot_index].vert].time;//早く着いたら待つ
                    wait=0; //待ち時間は0
                    use_reserve[genes[pivot_index].vert] = 0; //予約を満たす
                } 
                else{
                    use_reserve[genes[pivot_index].vert] = 0.5; //ちょっとだけペナルティ
                }
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
            minutes += spots[genes[pivot_index].vert].t;
            //もし待ち時間+所要時間が制限時間をオーバーするならゴールに向かう
            if(duration+minutes > TIMELIMIT){break;}
            duration+=minutes;
            //待ち時間シミュレーション（出発時）
            calc_queue_range();
            //printf("\ncalc_queue_range1[%d][%d][%d] done",i,j,k);
            minutes = 0; //シミュレーションが終わったので時間リセット

            while (1)
            {
                //printf("hello\n");
                //次の巡回地点を決定
                if (genes[k].dest != -1)
                {
                    //現在時刻が出発時刻より早い場合は行先を目的地に更新
                    if (duration < genes[k].time)
                    {
                        dest_node = genes[k].dest;
                    }//現在時刻より出発時刻が遅い場合は巡回地点に行先を代入
                    else{ 
                        k = genes[k].dest;
                    }
                }else{break;}
                //次の観光地が重複またはstart or goalの場合読み飛ばし，配列を修正
                int flag = 0;
                for (int l = 0; l < MAX_NODES; l++)
                {
                    if(temp_root[l].vert==-1){break;}
                    if(genes[k].vert==temp_root[l].vert || genes[k].vert==start || genes[k].vert==goal){
                        flag=1;
                        for (int m = k; m < MAX_NODES-1; m++)
                        {
                            genes[m].vert = genes[m+1].vert;
                            genes[m].time = genes[m+1].time;
                            genes[m].dest = genes[m+1].dest-1;
                        }
                        genes[MAX_NODES-1].vert = -1;
                        genes[MAX_NODES-1].time = 100000;
                        genes[MAX_NODES-1].dest = -1;
                        break;
                    }
                }
                if(flag){continue;}
                break;
            }
            
            next_index = k;
            //次の観光地が目的地より先なら終了
            if(next_index >= dest_node){break;}
            //printf("k:%d pivot:%d next:%d sat:%d dur:%d\n", k, j, genes[next_index].vert, satisfy, duration);
            
            //ここで次の観光地を回ってもゴールに間に合うかを予想．
            // 出発時間+移動時間+次観光地の所要時間+ゴールまでの移動時間の予想 
            expect = duration + calc_travel_time(genes[pivot_index].vert, genes[next_index].vert) + spots[genes[next_index].vert].t + calc_travel_time(genes[next_index].vert, goal);
            //printf("\ncalc_travel_time3[%d][%d][%d] done",i,j,k);
            //出発前にゴールに間に合うか判定 間に合わない場合は終了
            if(expect > TIMELIMIT){
                break;
            }
            //移動時間
            minutes += calc_travel_time(genes[pivot_index].vert, genes[next_index].vert); 
            //printf("\ncalc_travel_time4[%d][%d][%d] done",i,j,k);

            duration += minutes;
            satisfy += spots[genes[pivot_index].vert].value;
            pivot_index = next_index;
        }
        
        //終了後ゴールまでの経路を入れる
        duration+=calc_travel_time(genes[pivot_index].vert, goal);
        
        //終了時刻に間に合わない場合はペナルティ
        if (duration > TIMELIMIT)
        {
            satisfy -= 100;
        }

        //予約を使わなかった場合はペナルティ
        for (int k = 0; k < MAX_SPOTS; k++)
        {
            satisfy -= (use_reserve[k] * 100);
        }
        
        if (savemode)
        {
            //評価値が最低であればsaveを更新
            //printf("hello2\n");
            if (satisfy < min)
            {
                for (int k = 0; k < MAX_SPOTS; k++)
                {
                    save_minroot[k].vert = -1;
                    save_minroot[k].time = 0;
                    save_minroot[k].reserve = 0;
                    save_minroot[k].reservetimes = -1;
                }
                for (int k = 0; k < MAX_SPOTS; k++)
                {
                    save_minroot[k].vert = temp_root[k].vert;
                    save_minroot[k].time = temp_root[k].time;
                    save_minroot[k].reserve = temp_root[k].reserve;
                    save_minroot[k].reservetimes = temp_root[k].reservetimes;
                }
                min = satisfy;
            }
            //評価値が最大であれば経路を保存
            //printf("hello3\n");
            if (max < satisfy)
            {
                /**/
                for (int k = 0; k < MAX_SPOTS; k++)
                {
                    save_maxroot[k].vert = -1;
                    save_maxroot[k].time = 0;
                    save_maxroot[k].reserve = 0;
                    save_maxroot[k].reservetimes = -1;
                }
                for (int k = 0; k < MAX_SPOTS; k++)
                {
                    save_maxroot[k].vert = temp_root[k].vert;
                    save_maxroot[k].time = temp_root[k].time;
                    save_maxroot[k].reserve = temp_root[k].reserve;
                    save_maxroot[k].reservetimes = temp_root[k].reservetimes;
                }
                max = satisfy;
            }
            
            
            
            //tempルートを保存し発生確率を計算
            int index = -1;
            int flag = 0;
            //デバック用
            if(0){
                printf("duration:%d\n",duration);
                printf("temproot:");
                for (int k = 0; k < MAX_SPOTS; k++)
                {
                    printf("%d ", temp_root[k].vert);
                }
                printf("\n");
            }
            
            
            for (int k = 0; k < LOOPS; k++)
            {
                //一致しない場合,-1をindexに
                if(save_temproot[k][0].vert==-1){index=k;break;}
                //一致判定
                for (int l = 0; l < MAX_SPOTS; l++)
                {
                    if(save_temproot[k][l].vert != temp_root[l].vert){flag=0;break;}
                    flag = 1;
                }
                //一致するなら終わり
                if(flag){index = k;break;}
            }
            count_temproot[index] += 1;
            //printf("root:");
            for (int k = 0; k < MAX_SPOTS; k++)
            {
                //printf("%d ",temp_root[k].vert);
                save_temproot[index][k].vert = temp_root[k].vert;
                save_temproot[index][k].time = temp_root[k].time;
                save_temproot[index][k].reserve = temp_root[k].reserve;
                save_temproot[index][k].reservetimes = temp_root[k].reservetimes;
                //printf("%d ",save_temproot[index][k].vert);
                
            }
            //printf("\n");
        }
        if (min>satisfy)
        {
            min = satisfy;
        }
        if (max<satisfy)
        {
            max = satisfy;
        }
        
        sum+= satisfy;
        sum_duration+=duration;
        sum_satisfy+=satisfy;
    }
    fitness = sum / LOOPS;
    printf("fitness is %f\n", fitness);
    //printf("duration is %d, satisfy is %d\n", sum_duration/LOOPS, sum_satisfy/LOOPS);
}


