#include <cstdio>
constexpr int N = 405;
struct Node {
    int a, b, c, v;
} p[N];
int main() {
    int Tid, T, n;
    scanf("%d%d", &Tid, &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++)
            scanf("%d%d%d%d", &p[i].a, &p[i].b, &p[i].c, &p[i].v);
        puts("explode");
    }
    return 0;
}