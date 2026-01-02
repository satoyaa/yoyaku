#ifndef EXTERN_H
#define EXTERN_H

#define MAX_ITERATION 200
#define POPULATION 100 //絶対に偶数
#define MAX_SPOTS 30 //観光地数
#define MAX_NODES 900 //遺伝子長の最大値
#define MAX_INITIALIZE_NODES 900
#define MAX_RESERVES 10
#define SPEED 30 //速度（km/h）
//#define TIMELIMIT 120 //制限時間(分)
#define LOOPS 30
//#define reserve_rate 1
#define reserve_time 2 //予約にかかる時間(分)，暫定的に2分
#define EARTH_RAD 6378.137 // 地球の半径(km)
#define BRANCHES 5 //分岐の数


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
} Save;

typedef struct Gene
{
    int vert; //ノードの値
    int time; //出発予定時刻
    int dest; //行先(destination)
} Gene; 


extern Spot spots[MAX_SPOTS];  // 構造体配列の外部宣言
extern Gene genes[POPULATION][MAX_NODES];
extern Reserve genes_reserves[POPULATION][MAX_SPOTS];
extern int queue_range[MAX_SPOTS];
extern double fitness[POPULATION];
extern double crossover_rate;
extern double mutation_rate;
extern Save save_maxroot[MAX_NODES];
extern Save save_minroot[MAX_NODES];
extern Save save_temproot[LOOPS][MAX_NODES];
extern int count_temproot[LOOPS];
extern double min;
extern double max;
extern int TIMELIMIT;
extern int reserve_rate;
extern int savemode;
extern int local_search_mode;
extern void crossover_pmx();
extern void crossover_twopoint();
extern void selection_tournament();
extern void mutation_swap();
extern void mutation_random();
extern void local_search(int start, int goal);
extern void local_search_ultimate(int start, int goal);
extern void local_search_binary(int start, int goal);
void mutation_newpop(int start, int goal);
extern void readdata(const char * filename);
extern void initialize(int start, int goal);
extern void calc_fitness(int start, int goal);
extern int debug;
extern double fitnessLog;
extern double increase_rate;
//extern void serchAll();



#endif