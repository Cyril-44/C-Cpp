#include <bits/stdc++.h>
 
using namespace std;
 
const int N = 200005;
const int mod = 1000000009;
 
int tid, T, n, m;
bool tag[N];
int a[N];
 
void work() {
    vector<int> v1, v2;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        tag[i] = 0;
    }
    for (int i = 1; i <= m; i++) {
        int x;
        scanf("%d", &x);
        tag[x] = 1;
    }
    for (int i = 1; i <= n; i++) {
        v1.push_back(a[i]);
        v2.push_back(a[i]);
        if (!tag[i]) {
            vector<int> t1(v1), t2(v2);
            reverse(t1.begin(), t1.end());
            reverse(t2.begin(), t2.end());
            for (int j = 0; j < i; j++) {
                if (v1[j] > t2[j]) {
                    // printf("FDSFVDXV %d\n", i);
                    // for (int x : t2) printf("%d ", x);
                    // printf("\n");
                    v1 = t2;
                    break;
                } else if (v1[j] < t2[j]) break;
            }
            for (int j = i - 1; j >= 0; j--) {
                if (v2[j] > t1[j]) {
                    v2 = t1;
                    break;
                } else if (v2[j] < t1[j]) break;
            }
        }
    }
    // for (int x : v1) printf("%d ", x);
    int ans = 0;
    for (int i = 0, j = 1; i < n; i++, j = 154ll * j % mod) {
        ans = (ans + 1ll * j * v1[i]) % mod;
    }
    printf("%d\n", ans);
}
 
int main() {
    scanf("%d%d", &tid, &T);
    while (T--) {
        work();
    }
    return 0;   
}