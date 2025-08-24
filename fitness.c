#include <stdio.h>
#include <stdlib.h>
#include "extern.h"

int calc_EV(Node* parent){
    double sum = 0;
    if((parent->childCount)<=0){return(spots[parent->value].value);} // 子どもを持たない場合はスキップ
    for (int i = 0; i < parent->childCount; i++)
    {
        Node* child = parent->children[i]; //親の方から見て確率計算
        double probability = child->t;
        sum += calc_EV(child) * probability;
    }
    return(sum + spots[parent->value].value);
}

void calc_fitness(){
    for (int i = 0; i < POPULATION; i++)
    {
        Node* found = find_node(genes[i], 0);
        fitness[i] = calc_EV(found);
    }
}