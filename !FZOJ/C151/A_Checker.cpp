#include <bits/stdc++.h>
using namespace std;
int a[100005];
int main() {
    mt19937_64 rng(time(nullptr));
    uniform_int_distribution<int> Edge(1, 100000000), N(1, 100000);
    int n, m, Q;
    vector<tuple<int,int,int>> edges;
start:
    freopen("A.in", "w", stdout);
    n = N(rng), m = 0, Q = N(rng);
    edges.clear();
    for (int i = 1; i < n; i++) {
        iota(a+1, a+1+n-i, i+1);
        int j = uniform_int_distribution<int>(0, std::min(n-i, 300000-m))(rng);
        m += j;
        for (int k = 1; k <= j; k++)
            edges.emplace_back(i, a[k], Edge(rng));
    }
    printf("%d %d\n", n, m);
    for (auto [i, j, k] : edges) printf("%d %d %d\n", i, j, k);
    printf("%d\n", Q);
    while (Q--) {
        int a = uniform_int_distribution<int>(1, n)(rng), b = uniform_int_distribution<int>(1, n)(rng);
        printf("%d %d\n", a, b);
    }
    fclose(stdout);
    system("./std <A.in >A.ans");
    system("./A <A.in >A.out");
    if (system("diff -b A.ans A.out >/dev/null 2>&1")) fputs("Wrong Answer!\n", stderr);
    else {fputs("Accepted!\n", stderr); goto start;}
    return 0;
}