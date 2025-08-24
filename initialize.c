#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "extern.h"

int calc_duration(int i, int j){
    double x1 = spots[i].coordinate_x;
    double x2 = spots[j].coordinate_x;
    double y1 = spots[i].coordinate_y;
    double y2 = spots[j].coordinate_y;

    double dx = (x1 - x2);
    double dy = (y1 - y2);

    double distance = sqrt(dx*dx + dy*dy);

    double duration = distance/(SPEED*1000/60);
    return(duration);
}

void initialize(int start, int goal){
    
    
    for (int i = 0; i < POPULATION; i++)
    {
        // 主要ルート作成.
        int index = 0;
        int next_node;
        int selected_node[30];
        double t;
        for (int j = 0; j < 30; j++)
        {
            selected_node[j]=-1;
        }
        genes[i] = create_node(index, start, TIMELIMIT, 0);
        selected_node[index] = start;
        while (1)
        {
            t = 0;
            next_node = rand()%MAX_NODES;
            int flag = 0;
            for (int j = 0; j < 30; j++)
            {
                if(selected_node[j]==next_node){flag=1;}
            }
            if(flag){continue;}
            Node* found = find_node(genes[i], index);
            //printf("%d %d %d\n", found->value,next_node, goal);
            t = found->elapsed + calc_duration(found->value, next_node) + spots[next_node].t + calc_duration(next_node, goal);
            //printf("time is %f\n", t);
            if (t > TIMELIMIT)
            {
                break;
            }
            if(index > MAX_NODES){break;}
            add_child_search(genes[i], index, index+1, next_node, 0.5, found->elapsed + calc_duration(found->value, next_node) + spots[next_node].t);
            index++;
        }
        Node* found = find_node(genes[i], index);
        add_child_search(genes[i], index, index+1, goal, 0.5, found->elapsed + calc_duration(found->value, goal));
        index++;

        //分岐を作成．
        int r = rand()%index;
        found = find_node(genes[i], r);
        add_child_search(genes[i], r, index+1, next_node, 0.5, found->elapsed + calc_duration(found->value, next_node) + spots[next_node].t);
        index++;
        while (1)
        {
            t = 0;
            next_node = rand()%MAX_NODES;
            int flag = 0;
            for (int j = 0; j < 30; j++)
            {
                if(selected_node[j]==next_node){flag=1;}
            }
            if(flag){continue;}
            Node* found = find_node(genes[i], index);
            //printf("%d %d %d\n", found->value,next_node, goal);
            t = found->elapsed + calc_duration(found->value, next_node) + spots[next_node].t + calc_duration(next_node, goal);
            //printf("time is %f\n", t);
            if (t > TIMELIMIT)
            {
                break;
            }
            add_child_search(genes[i], index, index+1, next_node, 0.5, found->elapsed + calc_duration(found->value, next_node) + spots[next_node].t);
            index++;
        }
    }
}