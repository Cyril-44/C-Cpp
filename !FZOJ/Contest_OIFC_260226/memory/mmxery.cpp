#include <bits/stdc++.h>
using namespace std;

int a[15];
int ans[15];
int b[15];
int n, k;
bool check(int x) {
    if (x == 0) return true;
    for (int i = 1; i <= x; i++) {
        if (a[i] == -1)
            b[i] = 0;
        else
            b[i] = a[i];
    }
    int cnt = 0;
    int kmin = 0, kmax = 0;
    for (int i = 1; i < x; i++)
        if (b[i] != b[i + 1]) cnt++;
    for (int i = x + 1; i <= n; i++) {
        if (i <= n && b[i] != -1)
            cnt += (b[i] != b[i - 1]);
        else {
            int w = i;
            while (i <= n && b[i] == -1) i++;
            if (i == n + 1) {
                int len = i - w;
                kmax += len;
            } else {
                int len = i - w;
                if (b[i] != b[w - 1]) kmin++;
                if (b[i] == b[w - 1]) {
                    if (len % 2 == 0)
                        kmax += len;
                    else
                        kmax += (len + 1);
                } else {
                    if (len % 2 == 0)
                        kmax += (len + 1);
                    else
                        kmax += len;
                }
            }
        }
    }
    return k >= cnt + kmin && k <= cnt + kmax;
}
void solve(int l) {
    if (l == 0) {
        int kmin = 0, kmax = 0;
        int pp = 1;
        while (pp <= n && a[pp] == -1) pp++;
        kmax += (pp - 1);
        for (int i = pp + 1; i <= n; i++) {
            if (a[i] != -1)
                kmin += (a[i] != a[i - 1]), kmax += (a[i] != a[i - 1]);
            else {
                int w = i;
                while (i <= n && a[i] == -1) i++;
                if (i == n + 1) {
                    int len = i - w;
                    kmax += len;
                } else {
                    int len = i - w;
                    if (a[i] != a[w - 1]) kmin++;
                    if (a[i] == a[w - 1]) {
                        if (len % 2 == 0)
                            kmax += len;
                        else
                            kmax += (len + 1);
                    } else {
                        if (len % 2 == 0)
                            kmax += (len + 1);
                        else
                            kmax += len;
                    }
                }
            }
        }
        if (k == kmin) {
            int ppp = 1;
            while (ppp <= n && a[ppp] == -1) ppp++;
            if (ppp == n + 1)
                for (int i = 1; i <= n; i++) ans[i] = 0;
            else {
                for (int i = 1; i <= n; i++)
                    if (a[i] != -1) ans[i] = a[i];
                for (int i = 1; i < ppp; i++) ans[i] = a[ppp];
                for (int i = ppp + 1; i <= n; i++) {
                    if (a[i] != -1)
                        continue;
                    else {
                        int w = i;
                        while (i <= n && a[i] == -1) i++;
                        if (i == n + 1)
                            for (int j = w; j < i; j++) ans[j] = ans[w - 1];
                        else {
                            if (a[w - 1] == a[i])
                                for (int j = w; j < i; j++) ans[j] = a[i];
                            else
                                for (int j = w; j < i; j++) ans[j] = 0;
                        }
                    }
                }
            }
            for (int i = 1; i <= n; i++) cout << ans[i];
            cout << endl;
        } else {
            int ppp = 1;
            while (ppp <= n && a[ppp] == -1) ppp++;
            if (ppp == n + 1)
                for (int i = 1; i <= n; i++) ans[i] = 0;
            else {
                for (int i = 1; i <= n; i++)
                    if (a[i] != -1) ans[i] = a[i];
                for (int i = ppp - 1; i >= 1; i--) ans[i] = !ans[i + 1];
                for (int i = ppp + 1; i <= n; i++) {
                    int w = i;
                    while (i <= n && a[i] == -1) i++;
                    if (a[i] != -1)
                        continue;
                    else {
                        int len = i - w;
                        if (a[i] == a[w - 1]) {
                            if (len % 2 == 0) {
                                ans[w] = 0;
                                for (int j = w + 1; j < i; j++) ans[j] = !ans[j - 1];
                            } else {
                                for (int j = w; j < i; j++) ans[j] = !ans[j - 1];
                            }
                        } else {
                            if (len % 2 == 1) {
                                ans[w] = 0;
                                for (int j = w + 1; j < i; j++) ans[j] = !ans[j - 1];
                            } else {
                                for (int j = w; j < i; j++) ans[j] = !ans[j - 1];
                            }
                        }
                    }
                }
            }
            for (int i = 1; i <= n; i++) cout << ans[i];
            cout << endl;
        }
        return;
    }
    int cnt = 0, kmin = 0, kmax = 0;
    for (int i = 1; i <= l; i++)
        if (a[i] == -1)
            ans[i] = 0, a[i] = 0;
        else
            ans[i] = a[i];
    for (int i = l + 1; i <= n; i++)
        if (a[i] != -1) ans[i] = a[i];
    for (int i = 1; i < l; i++)
        if (a[i] != a[i + 1]) cnt++;
    for (int i = l + 1; i <= n; i++) {
        if (i <= n && a[i] != -1)
            cnt += (a[i] != a[i - 1]);
        else {
            int w = i;
            while (i <= n && a[i] == -1) i++;
            if (i == n + 1) {
                int len = i - w;
                kmax += len;
            } else {
                int len = i - w;
                if (a[i] != a[w - 1]) kmin++;
                if (a[i] == a[w - 1]) {
                    if (len % 2 == 0)
                        kmax += len;
                    else
                        kmax += (len + 1);
                } else {
                    if (len % 2 == 0)
                        kmax += (len + 1);
                    else
                        kmax += len;
                }
            }
        }
    }
    if (k == cnt + kmax) {
        for (int i = l + 1; i <= n; i++) {
            if (a[i] != -1)
                continue;
            else {
                int w = i;
                while (i <= n && a[i] == -1) i++;
                if (i == n + 1) {
                    for (int j = w; j < i; j++) ans[j] = !ans[j - 1];
                } else {
                    if (a[i] == a[w - 1]) {
                        int len = i - w;
                        if (len % 2 == 0) {
                            ans[w] = 0;
                            for (int j = w + 1; j < i; j++) ans[j] = !ans[j - 1];
                        } else {
                            for (int j = w; j < i; j++) ans[j] = !ans[j - 1];
                        }
                    } else {
                        int len = i - w;
                        if (len % 2 == 1) {
                            ans[w] = 0;
                            for (int j = w + 1; j < i; j++) ans[j] = !ans[j - 1];
                        } else {
                            for (int j = w; j < i; j++) ans[j] = !ans[j - 1];
                        }
                    }
                }
            }
        }
        for (int i = 1; i <= n; i++) cout << ans[i];
        cout << endl;
    } else {
        for (int i = l + 1; i <= n; i++) {
            if (a[i] != -1)
                continue;
            else {
                int w = i;
                while (i <= n && a[i] == -1) i++;
                if (i == n + 1) {
                    for (int j = w; j < i; j++) ans[j] = !a[w - 1];
                } else {
                    if (a[i] == a[w - 1])
                        for (int j = w; j < i; j++) ans[j] = ans[j - 1];
                    else
                        for (int j = w; j < i; j++) ans[j] = 0;
                }
            }
        }
        for (int i = 1; i <= n; i++) cout << ans[i];
        cout << endl;
    }
}
int main() {
    freopen("memory.in","r",stdin);
    freopen("memory.out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int t;
    cin >> t;
    while (t--) {
        cin >> n >> k;
        string s;
        cin >> s;
        for (int i = 1; i <= n; i++) {
            if (s[i - 1] == '1' || s[i - 1] == '0')
                a[i] = s[i - 1] - '0', ans[i] = a[i];
            else
                a[i] = ans[i] = -1;
        }
        int kmin = 0, kmax = 0;
        int pp = 1;
        while (pp <= n && a[pp] == -1) pp++;
        kmax += (pp - 1);
        for (int i = pp + 1; i <= n; i++) {
            if (a[i] != -1)
                kmin += (a[i] != a[i - 1]), kmax += (a[i] != a[i - 1]);
            else {
                int w = i;
                while (i <= n && a[i] == -1) i++;
                if (i == n + 1) {
                    int len = i - w;
                    kmax += len;
                } else {
                    int len = i - w;
                    if (a[i] != a[w - 1]) kmin++;
                    if (a[i] == a[w - 1]) {
                        if (len % 2 == 0)
                            kmax += len;
                        else
                            kmax += (len + 1);
                    } else {
                        if (len % 2 == 0)
                            kmax += (len + 1);
                        else
                            kmax += len;
                    }
                }
            }
        }
        cout << kmin << " " << kmax << endl;
        if (k < kmin || k > kmax) {
            cout << -1 << endl;
            continue;
        }
        int l = 0, r = n;
        while (l < r) {
            int mid = (l + r + 1) / 2;
            if (check(mid))
                l = mid;
            else
                r = mid - 1;
        }
        cout << l << endl;
        solve(l);
    }
    return 0;
}
