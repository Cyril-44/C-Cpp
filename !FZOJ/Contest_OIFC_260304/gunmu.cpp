#include <cstdio>
#include <vector>
#include <unordered_set>
constexpr int N = 40005, P = 8005;
int p[P]; bool np[N], _init_np = [](const int n = 4e4) {
    for (int i = 2; i <= n; i++) {
        if (!np[i]) p[++p[0]] = i;
        for (int j = 1; j <= p[0] && i * p[j] <= n; j++) {
            np[i * p[j]] = true;
            if (i % p[j] == 0) break;
        }
    }
return false;}();
int fac[40][2], tot;
std::unordered_set<int> facs;
unsigned long long ans;
void calc(int idx, int now, int phi) {
    if (idx > tot) {
        if (facs.insert(now).second)
            ans += phi;
        return;
    }
    calc(idx+1, now, phi);
    calc(idx+1, now *= fac[idx][0], phi *= (fac[idx][0] - 1));
    for (int i = 2; i <= fac[idx][1]; i++)
        calc(idx+1, now *= fac[idx][0], phi *= fac[idx][0]);
}
int main() {
    freopen("gunmu.in", "r", stdin);
    freopen("gunmu.out", "w", stdout);
    int n, w;
    scanf("%d%d", &n, &w);
    for (int li; n--; ) {
        scanf("%d", &li);
        tot = 0;
        for (int i = 1; i <= p[0] && p[i] * p[i] <= li; i++)
            if (li % p[i] == 0) {
                fac[++tot][0] = p[i]; fac[tot][1] = 0;
                do ++fac[tot][1], li /= p[i];  while (li % p[i] == 0);
            }
        if (li > 1) fac[++tot][0] = li, fac[tot][1] = 1;
        calc(1, 1, 1);
    }
    printf("%llu\n", ans);
    return 0;
}