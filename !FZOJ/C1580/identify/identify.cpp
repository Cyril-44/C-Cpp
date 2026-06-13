#include <bits/stdc++.h>
int size();
int test(std :: vector<int > d);
void Reverse(int l,int r);
void answer(int x);
namespace {
void solve(int n){
    int blk = n == 5000 ? 100 : 900;
    int ans = 0;
    for (int i = 1; i <= blk; i += 2) {
        switch (test({i, i+1})) {
        case 1: ans += 2; break;
        case -1: Reverse(i, i+1); break;
        case 0: ++ans;
            if (test({i}) == 1) Reverse(i+1,i+1);
            else Reverse(i,i);
        }
    }
    for (int i = blk+1; i <= 2*blk; i += 2) {
        switch (test({i, i+1})) {
        case 1: ans += 2, Reverse(i,i+1); break;
        case 0: ++ans;
            if (test({i}) == 1) Reverse(i,i);
            else Reverse(i+1,i+1);
        }
    }
    for (int i = 2*blk + 1; i <= n; i += blk) {
        int len = std::min(blk, n-i+1);
        std::vector<int> cur(len);
        std::iota(cur.begin(), cur.end(), i);
        int l = -blk, r = blk, targ = -1;
        while (l <= r) {
            int mid = (l + r) / 2;
            cur.resize(len + std::abs(mid));
            if (mid > 0) std::iota(cur.begin() + len, cur.end(), 1);
            else std::iota(cur.begin() + len, cur.end(), blk + 1);
            int res = test(cur);
            if (res == 0) { targ = mid; break; }
            if (res > 0) r = mid - 1;
            else l = mid + 1;
        }
        assert(~targ);
        ans += (len - targ) / 2;
    }
    answer(ans);
}
}
void solve() { solve(size()); }

/*Trick: 先将前面一部分暴力询问出来，后面使用前面的信息二分
*/