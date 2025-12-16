#include <bits/stdc++.h>
using namespace std;
typedef long long zLt4deMAFHl5;
template <class Izl76iItGZ> void QH9dl0mdIjTvA_yw(Izl76iItGZ &gTwC6OHdybL) {
    gTwC6OHdybL = 0;
    int R98bH8V6 = 1;
    char VBEE7PC = getchar();
    while (VBEE7PC < '0' || VBEE7PC > '9') {
        if (VBEE7PC == '-') R98bH8V6 = -1;
        VBEE7PC = getchar();
    }
    while (VBEE7PC >= '0' && VBEE7PC <= '9') {
        gTwC6OHdybL = gTwC6OHdybL * 10 + VBEE7PC - '0';
        VBEE7PC = getchar();
    }
}
const int b5GXwXiW = 1000010, aUhJx3a9OK = 20, fUtwk7Y4eAp = 30;
vector<int> xkkwOyR[b5GXwXiW];
int mYYbAfGkOj, hbDL2G, glsRvEDLDlZt[b5GXwXiW], ugirUB2BUlJ, g1LnJmh, YSJUxckw;
struct RQ7x6ieqUc {
    int pyoLsQFQ, sqcBlIclA;
};
vector<RQ7x6ieqUc> uzmHlPQJe[b5GXwXiW];
int wy3HMK[b5GXwXiW], oxYFgzp[b5GXwXiW], gOQ_bNyGa[b5GXwXiW], SD7ZEGex[b5GXwXiW], FH2THbuICt;
void lFbYdl(int DtyN0Q8rB) {
    SD7ZEGex[DtyN0Q8rB] = gOQ_bNyGa[DtyN0Q8rB];
    for (int zWW0TjUfZHT : xkkwOyR[DtyN0Q8rB]) {
        gOQ_bNyGa[zWW0TjUfZHT] = gOQ_bNyGa[DtyN0Q8rB] + 1;
        lFbYdl(zWW0TjUfZHT);
        SD7ZEGex[DtyN0Q8rB] = max(SD7ZEGex[DtyN0Q8rB], SD7ZEGex[zWW0TjUfZHT]);
        if (SD7ZEGex[oxYFgzp[DtyN0Q8rB]] <= SD7ZEGex[zWW0TjUfZHT]) oxYFgzp[DtyN0Q8rB] = zWW0TjUfZHT;
    }
}
zLt4deMAFHl5 hqd_QJbg[b5GXwXiW][aUhJx3a9OK], su8v2rEAV67_[b5GXwXiW][fUtwk7Y4eAp];
int E2NJhK18[b5GXwXiW];
int ML_XQK(int bM3KMuv7, int WbjmPb8vl7M) {
    if (SD7ZEGex[bM3KMuv7] - gOQ_bNyGa[bM3KMuv7] >= WbjmPb8vl7M) return wy3HMK[bM3KMuv7] + WbjmPb8vl7M;
    return 0;
}
zLt4deMAFHl5 rGT0eQ[b5GXwXiW];
void u6_UZSnQ(int UwKrjyIi) {
    wy3HMK[UwKrjyIi] = ++FH2THbuICt;
    if (oxYFgzp[UwKrjyIi]) u6_UZSnQ(oxYFgzp[UwKrjyIi]);
    for (int nvF_Y78O3D : xkkwOyR[UwKrjyIi]) {
        if (nvF_Y78O3D == oxYFgzp[UwKrjyIi]) continue;
        u6_UZSnQ(nvF_Y78O3D);
        int HnmPcAC4 = gOQ_bNyGa[oxYFgzp[UwKrjyIi]] - gOQ_bNyGa[nvF_Y78O3D];
        for (int FgZQiJsptmNP = 0; FgZQiJsptmNP <= SD7ZEGex[nvF_Y78O3D] - gOQ_bNyGa[nvF_Y78O3D]; FgZQiJsptmNP++) {
            for (int JSutlR6dEB = 0; JSutlR6dEB < aUhJx3a9OK; JSutlR6dEB++) hqd_QJbg[ML_XQK(oxYFgzp[UwKrjyIi], FgZQiJsptmNP)][JSutlR6dEB] += hqd_QJbg[ML_XQK(nvF_Y78O3D, FgZQiJsptmNP)][JSutlR6dEB];
            for (int WwQatw = 0; WwQatw < fUtwk7Y4eAp; WwQatw++) su8v2rEAV67_[ML_XQK(oxYFgzp[UwKrjyIi], FgZQiJsptmNP)][WwQatw] += su8v2rEAV67_[ML_XQK(nvF_Y78O3D, FgZQiJsptmNP)][WwQatw];
            E2NJhK18[ML_XQK(oxYFgzp[UwKrjyIi], FgZQiJsptmNP)] += E2NJhK18[ML_XQK(nvF_Y78O3D, FgZQiJsptmNP)];
        }
    }
    hqd_QJbg[ML_XQK(UwKrjyIi, 0)][0] = 0;
    for (int ptdvzm6W_CJ = 0; ptdvzm6W_CJ < fUtwk7Y4eAp; ptdvzm6W_CJ++) su8v2rEAV67_[ML_XQK(UwKrjyIi, 0)][ptdvzm6W_CJ] = su8v2rEAV67_[ML_XQK(UwKrjyIi, 1)][ptdvzm6W_CJ] + (glsRvEDLDlZt[UwKrjyIi] & (1 << ptdvzm6W_CJ));
    E2NJhK18[ML_XQK(UwKrjyIi, 0)] = E2NJhK18[ML_XQK(UwKrjyIi, 1)] + 1;
    for (int msTFtCjSOi = 1; msTFtCjSOi < aUhJx3a9OK; msTFtCjSOi++) {
        hqd_QJbg[ML_XQK(UwKrjyIi, 0)][msTFtCjSOi] = hqd_QJbg[ML_XQK(UwKrjyIi, 0)][msTFtCjSOi - 1] + su8v2rEAV67_[ML_XQK(UwKrjyIi, 0)][msTFtCjSOi - 1] - su8v2rEAV67_[ML_XQK(UwKrjyIi, 1 << msTFtCjSOi - 1)][msTFtCjSOi - 1];
        hqd_QJbg[ML_XQK(UwKrjyIi, 0)][msTFtCjSOi] += hqd_QJbg[ML_XQK(UwKrjyIi, 1 << msTFtCjSOi - 1)][msTFtCjSOi - 1] + (1ll << msTFtCjSOi - 1) * (E2NJhK18[ML_XQK(UwKrjyIi, 1 << msTFtCjSOi - 1)] - E2NJhK18[ML_XQK(UwKrjyIi, 1 << msTFtCjSOi)]) -
                         su8v2rEAV67_[ML_XQK(UwKrjyIi, 1 << msTFtCjSOi - 1)][msTFtCjSOi - 1] + su8v2rEAV67_[ML_XQK(UwKrjyIi, 1 << msTFtCjSOi)][msTFtCjSOi - 1];
    }
    for (const RQ7x6ieqUc &IwJrbplS : uzmHlPQJe[UwKrjyIi]) {
        int gFGEMSOTEwi = 0, LtYWpM66BXgE = fUtwk7Y4eAp - 1;
        for (int dBIJdh = aUhJx3a9OK - 1; dBIJdh >= 0; dBIJdh--) {
            if (IwJrbplS.pyoLsQFQ & (1 << dBIJdh)) {
                rGT0eQ[IwJrbplS.sqcBlIclA] += hqd_QJbg[ML_XQK(UwKrjyIi, gFGEMSOTEwi)][dBIJdh];
                for (; LtYWpM66BXgE > dBIJdh; LtYWpM66BXgE--) rGT0eQ[IwJrbplS.sqcBlIclA] += su8v2rEAV67_[ML_XQK(UwKrjyIi, gFGEMSOTEwi)][LtYWpM66BXgE] - su8v2rEAV67_[ML_XQK(UwKrjyIi, IwJrbplS.pyoLsQFQ)][LtYWpM66BXgE];
                rGT0eQ[IwJrbplS.sqcBlIclA] += su8v2rEAV67_[ML_XQK(UwKrjyIi, gFGEMSOTEwi)][LtYWpM66BXgE] - su8v2rEAV67_[ML_XQK(UwKrjyIi, gFGEMSOTEwi + (1 << LtYWpM66BXgE))][LtYWpM66BXgE];
                rGT0eQ[IwJrbplS.sqcBlIclA] += (E2NJhK18[ML_XQK(UwKrjyIi, gFGEMSOTEwi + (1 << LtYWpM66BXgE))] - E2NJhK18[ML_XQK(UwKrjyIi, IwJrbplS.pyoLsQFQ)]) * (1ll << LtYWpM66BXgE);
                rGT0eQ[IwJrbplS.sqcBlIclA] -= su8v2rEAV67_[ML_XQK(UwKrjyIi, gFGEMSOTEwi + (1 << LtYWpM66BXgE))][LtYWpM66BXgE] - su8v2rEAV67_[ML_XQK(UwKrjyIi, IwJrbplS.pyoLsQFQ)][LtYWpM66BXgE];
                LtYWpM66BXgE--;
                gFGEMSOTEwi += (1 << dBIJdh);
            }
        }
    }
}
int main() {
    QH9dl0mdIjTvA_yw(mYYbAfGkOj);
    for (int LcdggwY = 1; LcdggwY <= mYYbAfGkOj; LcdggwY++) QH9dl0mdIjTvA_yw(glsRvEDLDlZt[LcdggwY]);
    for (int QpiQugrQrb = 2; QpiQugrQrb <= mYYbAfGkOj; QpiQugrQrb++) {
        QH9dl0mdIjTvA_yw(ugirUB2BUlJ);
        xkkwOyR[ugirUB2BUlJ].push_back(QpiQugrQrb);
    }
    QH9dl0mdIjTvA_yw(hbDL2G);
    for (int L4t1rxnhRrF = 1; L4t1rxnhRrF <= hbDL2G; L4t1rxnhRrF++) {
        QH9dl0mdIjTvA_yw(g1LnJmh);
        QH9dl0mdIjTvA_yw(YSJUxckw);
        YSJUxckw++;
        uzmHlPQJe[g1LnJmh].push_back((RQ7x6ieqUc){YSJUxckw, L4t1rxnhRrF});
    }
    lFbYdl(1);
    u6_UZSnQ(1);
    for (int Y_xiOTPIsIt = 1; Y_xiOTPIsIt <= hbDL2G; Y_xiOTPIsIt++) printf("%lld\n", rGT0eQ[Y_xiOTPIsIt]);
    return 0;
}