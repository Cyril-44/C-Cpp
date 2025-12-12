#include <bits/stdc++.h>
using namespace std;
int a[10005][505], tot[10005];
int now[505];
int main() {
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> rgN(1, 500);
    for (int NOW = 1; NOW <= 100; ++NOW) {
        FILE *input = fopen("sort_chk.in", "w");
        int sum_n = 0;
        int T = 10000;
        for (int j = 1; j <= T; j++) {
            int n = rgN(rng), m = n;
            fprintf(input, "%d %d\n", n, m);
            tot[j] = n;
            for (int i = 1; i <= n; i++)
                fprintf(input, "%d ", i), a[j][i] = i;
            fputc('\n', input);
            shuffle(a[j]+1, a[j]+1 + n, rng);
            for (int i = 1; i <= n; i++)
                fprintf(input, "%d ", a[j][i]);
            fputc('\n', input);
            sum_n += n;
        }
        fclose(input);
        if (system(("time -f %es ./sort " + to_string(T) + " < sort_chk.in > sort_chk.out").c_str()))
            return puts("\033[1;35mRuntime Error!\033[0m"), 1;
        FILE *output = fopen("sort_chk.out", "r");
        auto st = clock();
        for (int j = 1; j <= T; j++) {
            int m;
            fscanf(input, "%d", &m);
            int n = tot[j];
            for (int i = 1; i <= n; i++)
                now[i] = i;
            if (m > n) return printf("\033[1;31mWrong Answer on test case %d! m >= n.\033[0m\n", j), 2;
            int l, r; char op;
            for (int i = 1; i <= m; i++) {
                fscanf(input, "%d %d %c", &l, &r, &op);
                op == 'I' ? sort(now+l, now+r+1) : sort(now+l, now+r+1, greater<int>());
            } 
            if (memcmp(now, a[j], sizeof(int) * (n + 1))) {
                printf("\033[1;31mWrong Answer on test case %d!\033[0m\n", j);
                freopen("sort_chk.info", "w", stdout);
                puts("Expected:");
                for (int i = 1; i <= n; i++)
                    printf("%d ", a[j][i]);
                puts("\nFound:");
                for (int i = 1; i <= n; i++)
                    printf("%d ", now[i]);
                return 2;
            }
        }
        auto ed = clock();
        printf("\033[32mAccepted! #%d\033[0m (Elapsed %.3lfs) (sum_n=%d)\n", NOW, (ed - st) * 1. / CLOCKS_PER_SEC, sum_n);
    }
    puts("\033[1;32mAccepted!\033[0m");
    return 0;
}