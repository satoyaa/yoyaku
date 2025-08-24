#ifndef EXTERN_H
#define EXTERN_H

#define MAX_ITERATION 1000
#define POPULATION 100
#define MAX_NODES 30
#define SPEED 3 //速度（km/h）
#define TIMELIMIT 60 //制限時間(分)


typedef struct {
    int vert;
    int value;
    int kind;
    int t;
    int crow;
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

extern Spot spots[MAX_NODES];  // 構造体配列の外部宣言
extern Node* create_node(int key,int value, double t, double elapsed);
extern void add_child(Node* parent, Node* child);
extern Node* genes[POPULATION];
extern double fitness[POPULATION];
extern void readdata(const char * filename);
extern void add_child_search(Node* root, int targetKey, int newKey, int newValue, double newT, double newElapsed);
extern Node* find_node(Node* root, int targetKey);
extern void initialize(int start, int goal);
extern void calc_fitness();



#endif
