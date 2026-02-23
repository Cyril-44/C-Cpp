#include<bits/stdc++.h>
#include <cstdlib>
#include <random>
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
using namespace std;
constexpr int N = 300005;
int a[N];
inline int standard(int n) {
    int m = 0;
    For (i, 1, n) {
        int j = i;
        if (j < n && a[j+1] == a[i] + 1) {
            int mn = a[i] + 1;
            for (; j < n && (a[j+1] == a[j] + 1 || a[j+1] >= mn && a[j+1] <= a[j]); ++j);
            i = j;
        }
        ++m;
    }
    return m;
}
inline int my(int n) {
    long long ans = 0;
    for (int i = 1; i <= n; i++) {
        int j = i;
        map<int, int> firsthit;
        firsthit[a[j]] = j;
        ans += 1ll * i * (n - i + 1);
        if (j < n && a[j+1] == a[i] + 1) {
            int mn = a[i] + 1;
            for (; j < n && (a[j+1] == a[j] + 1 || a[j+1] >= mn && a[j+1] <= a[j]); ++j) {
                if (!firsthit[a[j+1]]) firsthit[a[j+1]] = j+1;
                ans += 1ll * (j+1 - firsthit[a[j+1]] + 1) * (n - (j+1) + 1);
            }
            i = j;
        }
    }
    return ans;
}
int main() {
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> gen(1, 10);
    int n = 10, arr[11];
    for (int i = 1; i <= n; i++) arr[i] = gen(rng);
    int stdans=0;
    for (int i = 1; i <= n; i++)
        for (int j = i; j <= n; j++) {
            for (int k = 1; k <= j-i+1; k++)
                a[k] = arr[i+k-1];
            stdans += standard(j-i+1);
        }
    for (int i = 1; i <= n; i++) a[i] = arr[i];
    if (my(n) != stdans) {
        cout << n << endl;
        for (int i = 1; i <= n; i++) cout << arr[i] << ' ';
        cout << endl;
        cout << "Exp: " << stdans << ", Found: " << my(n) << endl;
        throw;
    }
    return 0;
}