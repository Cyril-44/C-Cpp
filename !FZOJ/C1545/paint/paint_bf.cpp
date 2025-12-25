#include <bits/stdc++.h>
constexpr int N = 102, MX = 20;
int buc[N];
std::bitset<N> pos;
int h, n, cnt;
inline void chk() {
    int cur = 0;
    pos.reset();
    for (int i = 1; i <= cnt; i++) {
        while (pos.test(++cur));
        int tmp = cur;
        for (int j = 0; j < n; j++) {
            tmp += buc[j];
            if (tmp > h || pos.test(tmp)) return;
            pos.set(tmp);
        }
    }
    for (int j = 1; j < n; j++)
        printf("%d ", buc[j]);
    putchar('\n');
}
void dfs(int i) {
    if (i == n) return chk();
    for (int j = 1; j <= MX; j++)
        buc[i] = j, dfs(i + 1);
}
int main() {
    scanf("%d%d", &h, &n); cnt = h / n;
    dfs(1);
    return 0;
}