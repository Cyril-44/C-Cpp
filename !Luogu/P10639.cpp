#include <cstdio>
#include <cstring>
#include <vector>
#include <cstdint>
#include <cassert>
constexpr int N = 500005;
int a[N], n;
class SegBeats {
    struct Node {
        struct Data {
            int64_t val, add; int cnt;
            bool operator<(const Data &x) const { return val < x.val; }
            bool operator==(const Data &x) const { return val == x.val; }
            Data& operator+=(const Data &x) { cnt += x.cnt; return *this; }
            void operator+=(int64_t v) { val += v, add += v; }
            friend Data operator+(Data x, const Data &y) { return x += y; }
        };
        std::vector<Data> vec;
        int64_t sum, mxadd = 0, mnadd = 0, add = 0; int length;
        std::vector<Data>* operator->() { return &vec; }
        void addsome2(Data& dat, int64_t v) { sum += v * dat.cnt; dat += v; }
        void set(int64_t val) { vec = {{val, 0, 1}}, sum = val, length = 1; }
        void apply(int64_t mnadd, int64_t mxadd, int64_t add) {
            if (vec.size() == 1) {
                assert(mnadd == mxadd);
                addsome2(vec.front(), mxadd);
                return;
            }
            addsome2(vec.front(), mxadd), addsome2(vec.back(), mnadd);
            sum += add * (length - vec.front().cnt - vec.back().cnt);
            for (int i = 1; i+1 < (int)vec.size(); i++) vec[i] += add;
        }
    } tr[N << 2];
    void pushup(int u) {
        tr[u].sum = tr[u<<1].sum + tr[u<<1|1].sum;
        tr[u].vec = tr[u<<1|1].vec;
        auto &v1 = tr[u].vec; const auto &v2 = tr[u<<1].vec;
        for (int i = 0, j = 0; j < (int)v2.size(); j++) {
            while (i < (int)v1.size() && v1[i] < v2[j]) ++i;
            if (i == (int)v1.size() || v2[j] < v1[i]) v1.insert(v1.begin() + i, v2[j]), ++i;
            else v1[i] += v2[j];
        }
        if (v1.size() > 4) v1.erase(v1.begin() + 2, v1.end() - 2);
        v1.shrink_to_fit();
    }
    int L, R; int64_t X;
    void pushdown(int u) {
        int64_t lmnadd, lmxadd, ladd, rmnadd, rmxadd, radd;
        lmnadd = tr[u<<1]->front() == tr[u]->front() ? tr[u].mnadd : tr[u].add;
        lmxadd = tr[u<<1]->back() == tr[u]->back() ? tr[u].mxadd : tr[u].add;
        ladd = tr[u].add;
        tr[u<<1].apply(lmnadd, lmxadd, ladd);
        rmnadd = tr[u<<1|1]->front() == tr[u]->front() ? tr[u].mnadd : tr[u].add;
        rmxadd = tr[u<<1|1]->back() == tr[u]->back() ? tr[u].mxadd : tr[u].add;
        radd = tr[u].add;
        tr[u<<1|1].apply(rmnadd, rmxadd, radd);
    }
    void updadd(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].apply(X,X,X);
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) updadd(u<<1, l, mid);
        if (mid < R) updadd(u<<1|1, mid+1, r);
        pushup(u);
    }
    void updmx(int u, int l, int r) { // CheckMax
        if (tr[u]->size() == 1) return tr[u].apply(X,X,X);
        if (tr[u]->front().val < X && X < std::next(tr[u]->begin())->val) // min < X < se_min
            return tr[u].addsome2(tr[u]->front(), X - tr[u]->front().val); // += X-min
        int mid = l + r >> 1; pushdown(u); updmx(u<<1, l, mid); updmx(u<<1|1, mid+1, r); pushup(u);
    }
    void updmn(int u, int l, int r) { // CheckMin
        if (tr[u]->size() == 1) return tr[u].apply(X,X,X);
        if (tr[u]->back().val > X && X > std::next(tr[u]->rbegin())->val) // max > X > se_max
            return tr[u].addsome2(tr[u]->back(), X - tr[u]->back().val); // += X-max
        int mid = l + r >> 1; pushdown(u); updmn(u<<1, l, mid); updmn(u<<1|1, mid+1, r); pushup(u);
    }
    int64_t inqsum(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].sum;
        int mid = l + r >> 1;
        if (R <= mid) return inqsum(u<<1, l, mid);
        if (mid < L) return inqsum(u<<1|1, mid+1, r);
        return inqsum(u<<1, l, mid) + inqsum(u<<1|1, mid+1, r);
    }
    int64_t inqmn(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u]->front().val;
        int mid = l + r >> 1;
        if (R <= mid) return inqmn(u<<1, l, mid);
        if (mid < L) return inqmn(u<<1|1, mid+1, r);
        return std::min(inqmn(u<<1, l, mid), inqmn(u<<1|1, mid+1, r));
    }
    int64_t inqmx(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u]->back().val;
        int mid = l + r >> 1;
        if (R <= mid) return inqmx(u<<1, l, mid);
        if (mid < L) return inqmx(u<<1|1, mid+1, r);
        return std::max(inqmx(u<<1, l, mid), inqmx(u<<1|1, mid+1, r));
    }
public:
    void build(int u = 1, int l = 1, int r = n) {
        if (l == r) return tr[u].set(a[l]);
        int mid = l + r >> 1;
        build(u<<1, l, mid); build(u<<1|1, mid+1, r);
        pushup(u); tr[u].length = tr[u<<1].length + tr[u<<1|1].length;
    }
    void updateAdd(int l, int r, int64_t x) { L=l,R=r,X=x; updadd(1,1,n); }
    void checkMin(int l, int r, int64_t x) { L=l,R=r,X=x; updmn(1,1,n); }
    void checkMax(int l, int r, int64_t x) { L=l,R=r,X=x; updmx(1,1,n); }
    int64_t inquireSum(int l, int r) { L=l,R=r; return inqsum(1,1,n); }
    int64_t inquireMin(int l, int r) { L=l,R=r; return inqmn(1,1,n); }
    int64_t inquireMax(int l, int r) { L=l,R=r; return inqmx(1,1,n); }
} fs;
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    fs.build();
    int m; scanf("%d", &m);
    for (int op, l, r, x; m--; ) {
        scanf("%d%d%d", &op, &l, &r);
        if (op <= 3) scanf("%d", &x);
        switch (op) {
        case 1: fs.updateAdd(l, r, x); break;
        case 2: fs.checkMax(l, r, x); break;
        case 3: fs.checkMin(l, r, x); break;
        case 4: printf("%lld\n", fs.inquireSum(l, r)); break;
        case 5: printf("%lld\n", fs.inquireMax(l, r)); break;
        case 6: printf("%lld\n", fs.inquireMin(l, r)); break;
        }
    }
    return 0;
}
