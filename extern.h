#ifndef EXTERN_H
#define EXTERN_H

#define MAX_ITERATION 10
#define POPULATION 10
#define MAX_SPOTS 30
#define MAX_RESERVES 10
#define SPEED 3 //速度（km/h）
#define TIMELIMIT 240 //制限時間(分)
#define LOOPS 3
#define reserve_rate 0.5


typedef struct {
    int vert;
    int value;
    int kind;
    int t;
    int crow;
    int capacity;
    int cost;
    double coordinate_x;
    double coordinate_y;
    int reservable;
    int reserve;
    int penalty;
} Spot;

/* --- ノード定義 --- */
typedef struct Node {
    int key;  
    int value;              // ノードの値
    double t;               // 出発予定時刻
    double elapsed;         // 経過時間
    int childCount;         // 子ノードの数
    struct Node **children; // 子ノードの配列
} Node;

extern Spot spots[MAX_SPOTS];  // 構造体配列の外部宣言
extern int genes[POPULATION][MAX_SPOTS];
extern int genes_reserves[POPULATION][MAX_SPOTS];
extern int times[POPULATION];
extern int queue_range[MAX_SPOTS];
extern double fitness[POPULATION];
extern double crossover_rate;
extern void crossover_pmx();
extern void selection_tournament();
extern void readdata(const char * filename);
extern void initialize(int start, int goal);
extern void calc_fitness();



#endif
