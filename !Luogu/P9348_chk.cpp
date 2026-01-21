#include <bits/stdc++.h>
using namespace std;
char bufstd[1000005]{}, bufout[1000005]{};
int main() {
    mt19937 rng(random_device{}());
    auto rand = [&] (int l, int r) {
        return uniform_int_distribution<int>(l, r)(rng);
    };
    for (int testCase = 1; testCase <= 100; ++testCase) {
        int t = rand(1, 1000);
        FILE *input = fopen("P9348_chk.in", "w");
        if (!input) return puts("\e[32mFile Write Error!\e[0m\n"), 1;
        fprintf(input, "%d\n", t);
        for (int tt = 1; tt <= t; tt++) {
            int n = rand(1, 10000);
            for (int i = 1; i <= n; i++)
                fputc(rand('a', 'z'), input);
            fputc('\n', input);
        }
        fclose(input);
        system("./P9348_bot < P9348_chk.in > P9348_chk.ans");
        if (system("./P9348 < P9348_chk.in > P9348_chk.out"))
            return fprintf(stderr, "\e[35mRuntime Error!\e[0m]\n"), 2;
        FILE *out = fopen("P9348_chk.out", "r"), *ans = fopen("P9348_chk.ans", "r");
        for (int i = 1; i <= t; i++) {
            fscanf(ans, "%s", bufstd), fscanf(out, "%s", bufout);
            if (strcmp(bufstd, bufout)) return fprintf(stderr, "\e[31mWrong Answer on test case %d!\e[0m\n", i), 3;
        }
        printf("\e[32mAccepted! #%d\e[0m\n", testCase);
    }
    return 0;
}