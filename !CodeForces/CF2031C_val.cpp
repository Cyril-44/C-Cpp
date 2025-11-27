#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <math.h>
#include <map>
#include <random>
const int N = 100005;
int a[N];
int main() {
    system("g++ CF2031C.cpp -O2 -std=c++17 -o CF2031C");
    freopen("CF2031C.in", "w", stdout);
    std::mt19937 rnd(std::random_device{}());
    int n;
    printf("%d\n", n = rnd() % 100000);
    for (int i = 1; i <= n; i++) {
        printf("%d\n", a[i] = rnd() % 200000);
    }
    fclose(stdout);
    system("time ./CF2031C <CF2031C.in >CF2031C.out");
    freopen("CF2031C.out", "r", stdin);
    freopen("CF2031C.res", "w", stderr);
    for (int i = 1; i <= n; i++) {
        int x;
        scanf("%d", &x);
        if (x == -1) continue;
        std::vector<int> mp[N];
        mp[x].emplace_back(1);
        for (int j = 2; j <= a[i]; j++) {
            scanf("%d", &x);
            mp[x].emplace_back(j);
        }
        for (int i = 1; i <= 50000; i++) {
            bool flg = true;
            auto& vec = mp[i];
            for (int i = 0; flg && i < vec.size(); i++)
                for (int j = i+1; flg && j < vec.size(); j++) {
                    int tmp = vec[j] - vec[i], s = sqrt(tmp);
                    if (s * s != tmp) flg = false;
                }
            if (!flg) fprintf(stderr, "WA on Test Case %d\n", i);
        }
    }
    return 0;
}