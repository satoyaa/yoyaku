#ifndef EXTERN_H
#define EXTERN_H

#define MAX_ITERATION 50
#define POPULATION 30
#define MAX_SPOTS 28
#define MAX_RESERVES 10
#define SPEED 30 //速度（km/h）
//#define TIMELIMIT 120 //制限時間(分)
#define LOOPS 50
//#define reserve_rate 1
#define reserve_time 2 //予約にかかる時間(分)，暫定的に2分
#define EARTH_RAD 6378.137 // 地球の半径(km)


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
    int penalty;
} Spot;

/* --- ノード定義 --- */
typedef struct Node {
    int key;  
    int value;              // ノードの値
    double time;               // 出発予定時刻
    double elapsed;         // 経過時間
    int childCount;         // 子ノードの数
    struct Node **children; // 子ノードの配列
} Node;

typedef struct Reserve
{
    int spot;
    int time;
} Reserve;

typedef struct Save
{
    int vert; //ノードの値
    int time; //出発予定時刻
    int reserve; //予約状況
    int reservetimes; //予約時刻
} Save;

extern Spot spots[MAX_SPOTS];  // 構造体配列の外部宣言
extern int genes[POPULATION][MAX_SPOTS];
extern Reserve genes_reserves[POPULATION][MAX_SPOTS];
extern int genes_timelimit[POPULATION][MAX_SPOTS];
extern int times[POPULATION][MAX_SPOTS];
extern int queue_range[MAX_SPOTS];
extern double fitness[POPULATION];
extern double crossover_rate;
extern double mutation_rate;
extern Save save_maxroot[MAX_SPOTS];
extern Save save_minroot[MAX_SPOTS];
extern Save save_temproot[LOOPS][MAX_SPOTS];
extern int count_temproot[LOOPS];
extern double min;
extern double max;
extern int TIMELIMIT;
extern int reserve_rate;
extern int savemode;
extern void crossover_pmx();
extern void selection_tournament();
extern void mutation_swap();
extern void readdata(const char * filename);
extern void initialize(int start, int goal);
extern void calc_fitness();
//extern void serchAll();



#endif
