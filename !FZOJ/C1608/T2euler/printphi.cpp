#include <bits/stdc++.h>
constexpr int N = 1e6;
int p[N], phi[N+1];
bool np[N+1];
int main() {
    for (int i = 2; i <= N; i++) {
        if (!np[i]) p[++p[0]] = i, phi[i] = i-1;
        for (int j = 1; j <= p[0] && i * p[j] <= N; j++) {
            np[i * p[j]] = true;
            if (i % p[j] == 0) {
                phi[i * p[j]] = phi[i] * p[j];
                break;
            }
            phi[i * p[j]] = phi[i] * (p[j] - 1);
        }
    }
    auto testpremx = [&] () {
        int premx = 0;
        for (int i = 2; i <= N; i++) {
            if (phi[i] > premx) {
                premx = phi[i];
                if (np[i]) printf("Holy Fuck! ");
                printf("%d: %d\n", i, premx);
            }
        }
        putchar('\n');
    };
    auto testchain = [&]() {
        for (int i = 2; i <= N; i++) {
            printf("#%d", i);
            int x = i;
            int len = 1;
            do {
                x = phi[x]; ++len;
                printf(" --> %d", x);
            } while (x > 1);
            if (len > 1 + std::ceil(std::log2(i))) printf(" Holy Fuck!!! gt %g; ", len - std::ceil(std::log2(i)));
            printf(" Chain Length: %d\n", len);
        }
    };
    testchain();
    return 0;
}