#include <bits/stdc++.h>
constexpr int N = 65;
double a[N], k[N], b[N];
int main() {
    int n, m; double d;
    scanf("%d%d%lf%lf", &n, &m, &d, &a[1]), scanf("%lf", &a[n]);
    for (int i = 2; i <= n-1; i++) {
        k[i-1] = 0, b[i-1] = a[i-1];
        k[i] = 1, b[i] = 0;
        for (int j = i+1; j <= n; j++) {
            k[j] = k[j-2] - 2*k[j-1];
            b[j] = b[j-2] - 2*b[j-1] + 2*d;
        }
        // k[n] * a[i] + b[n] = a[n] ==> a[i] = (a[n] - b[n]) / k[n];
        // fprintf(stderr, "%g %g\n", k[n], b[n]);
        a[i] = (a[n] - b[n]) / k[n];
    }
    printf("%.3lf\n", a[m]);
}