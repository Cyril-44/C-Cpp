#include <bits/stdc++.h>
int main() {
    int h, n;
    std::cin >> h >> n;
    std::vector<int> seq(n-1);
    for (int &i : seq) std::cin >> i;
    std::vector<int> pos(h+1);
    int cur = 0;
    for (int i = 1; i * n <= h; ++i) {
        while (pos[++cur]);
        int tmp = cur;
        pos[cur] = i;
        for (int j : seq) {
            tmp += j;
            if (pos[tmp]) {
                fprintf(stderr, "Error occured on position %d, turn %d. (Duplicated items)\n", tmp, i);
                goto final;
            }
            pos[tmp] = i;
        }
    }
    final:
    for (int i = 1; i <= h; i++) printf("%d ", pos[i]);
    putchar('\n');
    return 0;
}