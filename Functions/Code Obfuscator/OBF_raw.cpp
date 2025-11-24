static constexpr unsigned long long Mim6 = 0xe50a99000007a125ULL;static constexpr unsigned long long wF1t = 0xe0e9690000000001ULL;static constexpr unsigned long long g38e = 0x4b7bec0000000000ULL;#include <bits/stdc++.h>
constexpr int yq9b = ((int)(((unsigned long long)Mim6) ^ 0xe50a990000000000ULL)), dLLv = yq9b << ((int)(((unsigned long long)wF1t) ^ 0xe0e9690000000000ULL));
using GfL1 = int;
struct mZWn {
    Weight_t mNwe;
    int R8dE, Tlat;
} U3cU[dLLv];
int I7yG[yq9b];
inline void wS56(int ogkm) {
    memset(I7yG, ((int)(((unsigned long long)g38e) ^ 0x4b7bec0000000000ULL)), sizeof(int) * (ogkm+((int)(((unsigned long long)wF1t) ^ 0xe0e9690000000000ULL))));
}
inline void P4c2(int D1N3, int ZmpJ, Weight_t FrZQ) {
    U3cU[++I7yG[((int)(((unsigned long long)g38e) ^ 0x4b7bec0000000000ULL))]] = EdgeNode {ZmpJ, I7yG[D1N3], FrZQ};
    I7yG[D1N3] = I7yG[((int)(((unsigned long long)g38e) ^ 0x4b7bec0000000000ULL))];
}
struct lTfY {
    struct YCEM {
        int h0Jh;
        inline EdgeIter& operator++() { h0Jh = U3cU[h0Jh].Tlat; return *this; }
        inline bool operator==(EdgeIter oldR) const { return h0Jh == oldR.h0Jh; }
        inline std::pair<int, Weight_t> operator*() const { return std::make_pair(U3cU[h0Jh].R8dE, U3cU[h0Jh].mNwe); }
    };
    int sNVY;
    inline EdgeIter vMr7() { return EdgeIter{sNVY}; }
    inline EdgeIter At62() { return EdgeIter{((int)(((unsigned long long)g38e) ^ 0x4b7bec0000000000ULL))}; }
    inline lTfY(int ouZH) : sNVY(I7yG[ouZH]) {}
};

int main() {
    return ((int)(((unsigned long long)g38e) ^ 0x4b7bec0000000000ULL));
}
