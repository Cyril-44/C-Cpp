#include <bits/stdc++.h>
int main(int argc, char **argv) {
    FILE* in = fopen(argv[1], "r"), *out = fopen(argv[2], "r");
    int ans, ansx, ansy;
    fscanf(out, "%d %d %d", &ans, &ansx, &ansy);
    int n, m, w, h;
    fscanf(in, "%d%d%d%d", &n, &m, &w, &h);
    int cnt = 0;
    for (int i = 1, x, y; i <= n; i++) {
        fscanf(in, "%d%d", &x, &y);
        if (ansx <= x && x <= ansx + w && ansy <= y && y <= ansy + h)
            ++cnt;
    }
    for (int i = 1, x, y; i <= m; i++) {
        fscanf(in, "%d%d", &x, &y);
        if (ansx <= x && x <= ansx + w && ansy <= y && y <= ansy + h)
            --cnt;
    }
    fprintf(stderr, "%d %d\n", ans, std::abs(cnt));
    assert(ans == std::abs(cnt));
}