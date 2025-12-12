#include <bits/stdc++.h>
using namespace std;
using Rdi = std::uniform_int_distribution<int>;
int main() {
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    Rdi rgC[5]{Rdi(1, 3), Rdi(0, 3), Rdi(0, 3), Rdi(0, 1), Rdi(0, 0)}, rgM(1, 30);
    for (int NOW = 1; NOW <= 1000; ++NOW) {
        std::vector<pair<int,int>> edges;
        int root = 1;
        queue<pair<int,int>> q;
        q.emplace(root, 0);
        int tot = 1;
        while (!q.empty()) {
            auto [u, dep] = q.front(); q.pop();
            int ch_cnt = rgC[dep](rng);
            while (ch_cnt--) {
                edges.emplace_back(u, ++tot);
                q.emplace(tot, dep + 1);
            }
        }
        FILE *input = fopen("P3233_chk.in", "w");
        if (!input) return perror("fopen"), 1;
        fprintf(input, "%d\n", tot);
        for (const auto &[u, v] : edges)
            fprintf(input, "%d %d\n", u, v);
        int m = rgM(rng);
        fprintf(input, "%d\n", m);
        for (int i = 1; i <= m; i++) {
            vector<int> a(tot);
            iota(a.begin(), a.end(), 1);
            shuffle(a.begin(), a.end(), rng);
            int tbd = Rdi(1, tot)(rng);
            fprintf(input, "%d", tbd);
            for (int j = 0; j < tbd; j++)
                fprintf(input, " %d", a[j]);
            fprintf(input, "\n");
        }
        fclose(input);
        system("./P3233_std < P3233_chk.in > P3233_chk.ans");
        if (system("./P3233 < P3233_chk.in > P3233_chk.out 2> P3233_chk.txt"))
            return puts("\033[1;35mRuntime Error!\033[0m");
        if (system("diff -b P3233_chk.ans P3233_chk.out >/dev/null 2>&1"))
            return puts("\033[1;031mWrong Answer!\033[0m");
        printf("\033[32mAccepted! #%d\033[0m\n", NOW);
    }
    printf("\033[1;32mPretest passed!\033[0m\n");
    return 0;
}