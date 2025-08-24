#include <stdio.h>
#include <stdlib.h>

/* --- ノード定義 --- */
typedef struct Node {
    int key;                // ノードのキー
    int value;              // ノードの値
    double t;               // 出発予定時刻
    double elapsed;         // 経過時間
    int childCount;         // 子ノードの数
    struct Node **children; // 子ノードの配列
} Node;

/* --- ノード生成 --- */
Node* create_node(int key, int value, double t, double elapsed) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    node->key = key;
    node->value = value;
    node->t = t;
    node->elapsed = elapsed;
    node->childCount = 0;
    node->children = NULL;
    return node;
}

/* --- 子ノードを追加 --- */
void add_child(Node* parent, Node* child) {
    parent->childCount++;
    parent->children = (Node**)realloc(parent->children, parent->childCount * sizeof(Node*));
    if (!parent->children) {
        perror("realloc failed");
        exit(EXIT_FAILURE);
    }
    parent->children[parent->childCount - 1] = child;
}

void free_tree(Node* root) {
    if (!root) return;
    for (int i = 0; i < root->childCount; i++) {
        free_tree(root->children[i]);
    }
    free(root->children);
    free(root);
}

/* --- 任意のノードを検索 --- */
Node* find_node(Node* root, int targetKey) {
    if (!root) return NULL;
    if (root->key == targetKey) return root;
    for (int i = 0; i < root->childCount; i++) {
        Node* res = find_node(root->children[i], targetKey);
        if (res) return res;
    }
    return NULL;
}

/* --- 任意のノードに新しい子を追加 --- */
void add_child_search(Node* root, int targetKey, int newKey, int newValue, double newT, double newElapsed) {
    Node* target = find_node(root, targetKey);
    if (target) {
        Node* newNode = create_node(newKey, newValue, newT, newElapsed);
        add_child(target, newNode);
    } else {
        printf("Node with value %d not found.\n", targetKey);
    }
}
