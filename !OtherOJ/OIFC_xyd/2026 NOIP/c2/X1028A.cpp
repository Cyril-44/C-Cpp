#include <bits/stdc++.h>
using namespace std;
int n, m, nn, pc[1 << 16], pl[1 << 16][20], rk[1 << 16][20], of[1 << 16], sz[1 << 16], of1[1 << 16];
double p[20], nw[4500000], buf[2][530000], tmp[20], tmp2[20];
int main() {
    freopen("gunfire.in", "r", stdin);
    freopen("gunfire.out", "w", stdout);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) { scanf("%lf", &p[i]); }
    if (n == 1) {
        puts("1.000000");
        return 0;
    }
    nn = 1 << n;
    for (int k = 0; k < nn; k++) {
        int c = 0;
        for (int i = 1; i <= n; i++) {
            if (k >> (i - 1) & 1) {
                pl[k][c] = i;
                rk[k][i] = c++;
            }
        }
        pc[k] = c, sz[k] = c * c;
    }
    for (int k = 1; k < nn; k++) {
        of[k] = of[k - 1] + sz[k - 1];
        of1[k] = of1[k - 1] + pc[k - 1];
    }
    int pf = 0;
    for (int s = 1; s <= m; s++) {
        for (int i = n; i >= 1; i--) {
            for (int k = 0; k < nn; k++) {
                if (!(k >> (i - 1) & 1)) continue;
                int c = pc[k], id = rk[k][i], b = of[k] + id * c;
                if (c == 1) {
                    nw[b] = 1;
                    continue;
                }
                if (id + 1 < c) {
                    int b2 = of[k] + (id + 1) * c;
                    for (int t = 0; t < c; t++) tmp[t] = nw[b2 + t];
                } else if (s == 1) {
                    for (int t = 0; t < c; t++) tmp[t] = 1.0 / c;
                } else {
                    for (int t = 0; t < c; t++) tmp[t] = buf[pf][of1[k] + t];
                }
                double pi = p[i];
                if (pi == 0) {
                    for (int t = 0; t < c; t++) nw[b + t] = tmp[t];
                    continue;
                }
                double v0 = tmp[id], vm = v0;
                for (int j = 0; j < c; j++) {
                    if (j == id) continue;
                    int mj = k ^ (1 << (pl[k][j] - 1)), c2 = c - 1;
                    double hi;
                    if (c2 == 1)
                        hi = 1.0;
                    else {
                        int i2 = rk[mj][i];
                        if (i2 + 1 < c2)
                            hi = nw[of[mj] + (i2 + 1) * c2 + i2];
                        else
                            hi = (s == 1) ? 1.0 / c2 : buf[pf][of1[mj] + i2];
                    }
                    double vj = (1 - pi) * v0 + pi * hi;
                    if (vj > vm) vm = vj;
                }
                memset(tmp2, 0, sizeof(tmp2));
                int cn = 0;
                if (vm - v0 <= 1e-10) {
                    for (int t = 0; t < c; t++) tmp2[t] += tmp[t];
                    cn++;
                }
                for (int j = 0; j < c; j++) {
                    if (j == id) continue;
                    int mj = k ^ (1 << (pl[k][j] - 1)), c2 = c - 1, i2 = rk[mj][i];
                    double hi;
                    if (c2 == 1)
                        hi = 1;
                    else if (i2 + 1 < c2)
                        hi = nw[of[mj] + (i2 + 1) * c2 + i2];
                    else
                        hi = (s == 1) ? 1.0 / c2 : buf[pf][of1[mj] + i2];
                    double vj = (1 - pi) * v0 + pi * hi;
                    if (vm - vj <= 1e-10) {
                        if (c2 == 1) {
                            for (int t = 0; t < c; t++) tmp2[t] += (1 - pi) * tmp[t] + pi * (t == id);
                        } else {
                            for (int t = 0; t < c; t++) {
                                double he;
                                if (t == j)
                                    he = 0;
                                else {
                                    int t2 = t - (t > j);
                                    if (i2 + 1 < c2)
                                        he = nw[of[mj] + (i2 + 1) * c2 + t2];
                                    else
                                        he = (s == 1) ? 1.0 / c2 : buf[pf][of1[mj] + t2];
                                }
                                tmp2[t] += (1 - pi) * tmp[t] + pi * he;
                            }
                        }
                        cn++;
                    }
                }
                for (int t = 0; t < c; t++) nw[b + t] = tmp2[t] / cn;
            }
        }
        for (int k = 0; k < nn; k++) {
            int c = pc[k];
            if (!c) continue;
            if (c == 1)
                buf[pf ^ 1][of1[k]] = 1.0;
            else
                for (int t = 0; t < c; t++) buf[pf ^ 1][of1[k] + t] = nw[of[k] + t];
        }
        pf ^= 1;
    }
    printf("%.6f\n", buf[pf][of1[nn - 1]]);
    return 0;
}