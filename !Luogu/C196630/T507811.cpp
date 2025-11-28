#include <stdio.h>
int main() {
    int a[5][5];
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            scanf("%d", &a[i][j]);
    bool flg = false, flg1, flg2;
    for (int i = 0; !flg && i < 5; i++) {
        flg1 = true;
        for (int j = 0; j < 5; j++)
            if (!a[i][j]) flg1 = false;
        if (flg1) flg = true;
    }
    for (int j = 0; !flg && j < 5; j++) {
        flg1 = true;
        for (int i = 0; i < 5; i++)
            if (!a[i][j]) flg1 = false;
        if (flg1) flg = true;
    }
    flg1 = flg2 = true;
    for (int i = 0; !flg && i < 5; i++) {
        if (!a[i][i]) flg1 = false;
        if (!a[i][4-i]) flg2 = false;
    }
    if (flg1 || flg2) flg = true;
    puts(flg ? "Yes" : "No");
    return 0;
}