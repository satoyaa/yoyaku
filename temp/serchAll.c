#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "extern.h"

void perm_rec(int arr[], int pos[], bool used[], int depth, int n, int first, int last) {
    if (depth == n) {
        // 先頭固定
        printf("%d ", first);
        // 並び替えた部分
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[pos[i]]);
        }
        // 末尾固定
        printf("%d\n", last);
        return;
    }

    for (int i = 0; i < n; i++) {
        if (!used[i]) {
            used[i] = true;
            pos[depth] = i;
            perm_rec(arr, pos, used, depth + 1, n, first, last);
            used[i] = false;
        }
    }
}

void serchAll(int start, int goal){
    int buf[MAX_SPOTS];
    bool used[MAX_SPOTS+1] = {false}; // 1〜5を使うので添字は6まで確保
    perm_rec(arr, pos, used, 0, m, first, last);
}