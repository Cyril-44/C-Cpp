#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

inline int read() {
    int x = 0; char ch = getchar();
    while (ch < '0' || ch > '9') ch = getchar();
    while (ch >= '0' && ch <= '9') { x = x * 10 + ch - '0'; ch = getchar(); }
    return x;
}

const int MAXN = 5000005;
int a[MAXN];
int n;

long long solve(int L) {
    long long sum_max = 0;
    for (int i = 0; i < n; i += L) {
        int cur_max = 0;
        for (int j = i; j < i + L; ++j) {
            if (a[j] > cur_max) cur_max = a[j];
        }
        sum_max += cur_max;
    }
    return sum_max * L;
}

int main() {
    n = read();
    for (int i = 0; i < n; ++i) a[i] = read();

    long long min_val = -1;

    for (int L = 1; L * L <= n; ++L) {
        if (n % L == 0) {
            long long res1 = solve(L);
            if (min_val == -1 || res1 < min_val) min_val = res1;
            
            if (L * L != n) {
                long long res2 = solve(n / L);
                if (min_val == -1 || res2 < min_val) min_val = res2;
            }
        }
    }
    printf("%lld\n", min_val);
    return 0;
}