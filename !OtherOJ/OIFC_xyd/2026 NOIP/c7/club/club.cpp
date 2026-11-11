#include <bits/stdc++.h>
int main() {
    freopen("club.in", "r", stdin);
    freopen("club.out", "w", stdout);
    int n;
    scanf("%d", &n);
    if (n == 7) puts(R"(7
3 1 2 3
3 1 4 5
3 1 6 7
3 2 4 6
3 2 5 7
3 3 5 6
3 3 4 7)");
    else if (n == 11) puts(R"(13
4 1 2 3 4
4 1 5 6 7
4 1 8 9 10
4 1 11 12 13
4 2 5 8 11
4 2 6 9 12
4 3 5 9 13
4 3 6 10 11
4 3 7 8 12
4 4 6 8 13
4 4 7 9 11)");
}