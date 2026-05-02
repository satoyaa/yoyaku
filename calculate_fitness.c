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
    double dist = EARTH_RAD * acos(sin(y1) * sin(y2) + cos(y1) * cos(y2) * cos(x2 - x1));
    int t = (int)(dist *60 / SPEED);
    return(t);
}


void calc_queue_range(){
    for (int k = 0; k < MAX_SPOTS; k++) {
        double lambda_per_minute = spots[k].crow / 60.0;
        double mu_per_minute = 1.0 / spots[k].t;
        int servers = spots[k].capacity;
        double p_complete = 1.0 - exp(-mu_per_minute);
        int completions = binomial_rand(busy_servers[k], p_complete);
        busy_servers[k] -= completions;
        int free_servers = servers - busy_servers[k];
        int to_start = (waiting_queue[k] < free_servers) ? waiting_queue[k] : free_servers;
        busy_servers[k] += to_start;
        waiting_queue[k] -= to_start;
        int arrivals = poisson_rand(lambda_per_minute);
        waiting_queue[k] += arrivals;
    }
}


void calculate_fitness(int start, int goal){
    int finish = POPULATION;
    if (local_search_mode)
    {
        finish = 3;
    }
    
    for (int i = 0; i < finish; i++)
    {
        double sum = 0;
        int sum_duration = 0;
        double sum_satisfy = 0;
        if (savemode && i==0)
        {
            for (int j = 0; j < LOOPS; j++)
            {
                for (int k = 0; k < MAX_SPOTS; k++)
                {
                    save_temproot[j][k].vert = -1;
                    save_temproot[j][k].time = 0;
                } 
                count_temproot[j] = 0;
            }
        }
        
        int loops = LOOPS;
        for (int j = 0; j < loops; j++)
        {
            int duration = 0;
            double satisfy = 0;
            int next_index = 0;
            int pivot_index = start;
            int dest_node = MAX_NODES-1;
            int temp_index = 0;
            int expect;
            Save temp_root[MAX_NODES];
            int use_reserve[MAX_SPOTS];
            duration += (rand()%31 - 10);
            for (int k = 0; k < MAX_SPOTS; k++)
            {
                use_reserve[k] = genes_reserves[i][k].spot;
                waiting_queue[k] = spots[k].capacity;
            }
            for (int k = 0; k < MAX_NODES; k++)
            {
                temp_root[k].vert = -1;
                temp_root[k].time = 0;
            }
            temp_root[temp_index].vert = start;
            temp_root[temp_index].time = 0;
            temp_index++;
            calc_queue_range();
            for (int k = 1; k < MAX_NODES-1; k++)
            {   
                if(genes[i][pivot_index].vert == -1){break;}
                if (savemode)
                {
                }
                temp_root[temp_index].vert = genes[i][pivot_index].vert;
                temp_root[temp_index].time = genes[i][pivot_index].time;
                temp_index++;
                int minutes = 0;
                int wait = waiting_queue[genes[i][pivot_index].vert] * spots[genes[i][pivot_index].vert].t / (spots[genes[i][pivot_index].vert].capacity );
                if(use_reserve[genes[i][pivot_index].vert]==1){
                    if(duration <= genes_reserves[i][genes[i][pivot_index].vert].time){
                        duration = genes_reserves[i][genes[i][pivot_index].vert].time;
                        wait=0;
                        use_reserve[genes[i][pivot_index].vert] = 0;
                    }
                }
                minutes += wait;
                minutes += spots[genes[i][pivot_index].vert].t;
                if(duration+minutes > TIMELIMIT){break;}
                duration+=minutes;
                calc_queue_range();
                minutes = 0;
                while (1)
                {
                    if (genes[i][k].dest != -1)
                    {
                        if (duration < genes[i][k].time)
                        {
                            dest_node = genes[i][k].dest;
                        }
                        else{ 
                            k = genes[i][k].dest;
                        }
                    }
                    int flag = 0;
                    for (int l = 0; l < MAX_NODES; l++)
                    {
                        if(temp_root[l].vert==-1||k==MAX_NODES){break;}
                        if(genes[i][k].vert==temp_root[l].vert){
                            flag=1;
                            for (int m = k; m < MAX_NODES-1; m++)
                            {
                                genes[i][m].vert = genes[i][m+1].vert;
                                genes[i][m].time = genes[i][m+1].time;
                                if(genes[i][m+1].dest==-1){genes[i][m].dest = genes[i][m+1].dest;}
                                else{genes[i][m].dest = genes[i][m+1].dest-1;}
                            }
                            genes[i][MAX_NODES-1].vert = -1;
                            genes[i][MAX_NODES-1].time = 100000;
                            genes[i][MAX_NODES-1].dest = -1;
                            break;
                        }
                    }
                    if(flag){continue;}
                    break;
                }
                next_index = k;
                if(next_index >= dest_node){break;}
                expect = duration + calc_travel_time(genes[i][pivot_index].vert, genes[i][next_index].vert) + spots[genes[i][next_index].vert].t + calc_travel_time(genes[i][next_index].vert, goal);
                if(expect > TIMELIMIT){
                    break;
                }
                minutes += calc_travel_time(genes[i][pivot_index].vert, genes[i][next_index].vert); 
                duration += minutes;  
                satisfy += spots[genes[i][pivot_index].vert].value;              
                pivot_index = next_index;   
            }
            duration+=calc_travel_time(genes[i][pivot_index].vert, goal);
            temp_root[temp_index].vert = goal;
            temp_root[temp_index].time = 0;
            if (duration > TIMELIMIT)
            {
                satisfy -= 100;
            }
            for (int k = 0; k < MAX_SPOTS; k++)
            {
                satisfy -= (use_reserve[k] * 100);
            }
            if (savemode && i==0)
            {
                if (satisfy < min || j==0)
                {
                    for (int k = 0; k < MAX_NODES; k++)
                    {
                        save_minroot[k].vert = -1;
                        save_minroot[k].time = 0;
                    }
                    for (int k = 0; k < MAX_NODES; k++)
                    {
                        save_minroot[k].vert = temp_root[k].vert;
                        save_minroot[k].time = temp_root[k].time;
                    }
                    min = satisfy;
                }
                if (max < satisfy || j==0)
                {
                    for (int k = 0; k < MAX_NODES; k++)
                    {
                        save_maxroot[k].vert = -1;
                        save_maxroot[k].time = 0;
                    }
                    for (int k = 0; k < MAX_NODES; k++)
                    {
                        save_maxroot[k].vert = temp_root[k].vert;
                        save_maxroot[k].time = temp_root[k].time;
                    }
                    max = satisfy;
                }
                int index = -1;
                int flag = 0;
                for (int k = 0; k < LOOPS; k++)
                {
                    if(save_temproot[k][0].vert==-1){index=k;break;}
                    for (int l = 0; l < MAX_NODES; l++)
                    {
                        if(save_temproot[k][l].vert != temp_root[l].vert){flag=0;break;}
                        flag = 1;
                    }
                    if(flag){index = k;break;}
                }
                count_temproot[index] += 1;
                for (int k = 0; k < MAX_NODES; k++)
                {
                    save_temproot[index][k].vert = temp_root[k].vert;
                    save_temproot[index][k].time = temp_root[k].time;
                }
            }
            sum+= satisfy;
            sum_duration+=duration;
            sum_satisfy+=satisfy;
        }
        if(i==0&&(!savemode)){
            continue;
        }
        fitness[i] = sum / loops;
    }
}
