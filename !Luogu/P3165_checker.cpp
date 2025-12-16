#include <bits/stdc++.h>
using namespace std;
constexpr int N = 100005;
int a[N];
int main() {
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> rgN(1, 10), rgAI(1, 15);
    for (int NOW = 1; NOW <= 100; ++NOW) {
        int n = rgN(rng);
        for (int i = 1; i <= n; i++) a[i] = rgAI(rng);
        FILE *input = fopen("P3165_chk.in", "w");
        fprintf(input, "%d\n", n);
        for (int i = 1; i <= n; i++)
            fprintf(input, "%d ", a[i]);
        fprintf(input, "\n");
        fclose(input);
        system("./P3165_std <P3165_chk.in >P3165_chk.ans"); system("./P3165 <P3165_chk.in >P3165_chk.out");
        if (system("diff -b P3165_chk.ans P3165_chk.out")) return puts("\033[1;31mWrong Answer!\033[0m"), 1;
        puts("\033[32mAccepted!\033[0m");
    }
    return 0;
}