#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>

using namespace std;
typedef long long LL;

const int N = 100003;
const int M = 11;

string s;
string NuLl;
int v[M];
int cnt[M];
int x[M][N]; // The position of the ith occurrence of number i in the sequence
int cz[M];
long long sum, ans;

void print(int d) {
    for (int i = 1; i <= d; i++) {
        printf("##%d", cz[i]);
    }
    printf("\n");
}

bool ccheck(int d) {
    int ccnt[M] = {0};
    for (int i = 1; i <= d; i++) {
        if (++ccnt[cz[i]] > cnt[cz[i]]) return false;
    }
    return true;
}

void init();
void dfs(int);
void check(int);
int find(int, int); // Returns the position where number 'a' first appears starting from 'l'

int main() {
#ifndef ONLINE_JUDGE
    freopen("bargain.in", "r", stdin);
    freopen("bargain.out", "w", stdout);
#endif
    int C;
    scanf("%d", &C);
    int T;
    scanf("%d", &T);
    while (T--) {
        init();
        dfs(1);
        printf("%lld\n", ans);
    }
    return 0;
}

void init() {
    s = NuLl;
    memset(v, 0, sizeof(v));
    memset(cnt, 0, sizeof(cnt));
    memset(x, 0x3f, sizeof(x));
    memset(cz, 0, sizeof(cz));
    sum = ans = 0;

    cin >> s;
    int l = s.size();
    s = '0' + s;
    for (int i = 1; i <= 9; i++) scanf("%d", &v[i]);
    for (int i = 1; i <= l; i++) {
        int a = s[i] - '0';
        sum += v[a];
        x[a][++cnt[a]] = i;
    }
    ans = sum;
}

void dfs(int d) {
    if (d > 6) return;
    for (int i = 1; i <= 9; i++) {
        cz[d] = i;
        check(d);
        dfs(d + 1);
    }
}

void check(int d) {
    if (!ccheck(d)) return;

    long long cost = 0;
    long long les = 0;
    int l = 1;

    for (int i = 1; i <= d; i++) {
        int a = cz[i];
        int cc = cnt[a];
        les += v[a];
        cost = cost * 10 + a;
        if (l > x[a][cc]) return; // If the current lookup number is greater than the latest position of 'a', the target cannot be found
        l = find(l, a) + 1;
    }

    if (sum + cost - les < ans) {
        ans = sum + cost - les;
    }
}

int find(int l, int a) {
    int ll = 1;
    int rr = cnt[a] + 1;
    int mid;

    while (ll < rr) {
        mid = (ll + rr) / 2;
        long long p = x[a][mid];
        if (p < l) ll = mid + 1;
        else rr = mid;
    }
    return x[a][ll];
}
