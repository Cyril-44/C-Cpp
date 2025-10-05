#include <bits/stdc++.h>
using namespace std;

static const int MAXN = 4000005;  // 根据 N + M 需求调整

struct Node {
    int ch[2];   // 左右孩子索引
    int val;     // 关键字
    int cnt;     // 重复次数
    int sz;      // 子树总节点数（含重复）
    int mx;      // 子树最大值
} tr[MAXN];

int root = 0, top = 0;

// 更新 u 的 sz、mx
void pushup(int u) {
    tr[u].sz = tr[u].cnt;
    tr[u].mx = tr[u].val;
    if (tr[u].ch[0]) {
        tr[u].sz += tr[ tr[u].ch[0] ].sz;
    }
    if (tr[u].ch[1]) {
        tr[u].sz += tr[ tr[u].ch[1] ].sz;
        tr[u].mx  = tr[ tr[u].ch[1] ].mx;
    }
}

// 自顶向下分裂 splay，把 key 或 第 k 小 元素旋到根
// flag = 0 表示按 key，flag = 1 表示按第 k 小
int splay(int key, bool byRank = false) {
    static int stkL[MAXN], stkR[MAXN];
    int L = 0, R = 0;
    int u = root;
    // 分裂：把将要旋的路径节点分挂到 stkL/stkR
    while (true) {
        if (!byRank) {
            if (key < tr[u].val) {
                // zig-zig
                if (tr[u].ch[0] && key < tr[ tr[u].ch[0] ].val) {
                    int l = tr[u].ch[0];
                    tr[u].ch[0] = tr[l].ch[1];
                    tr[l].ch[1] = u;
                    pushup(u); u = l;
                }
                // 挂到右侧
                stkR[R++] = u;
                if (!tr[u].ch[0]) break;
                u = tr[u].ch[0];
            }
            else if (key > tr[u].val) {
                // zig-zig
                if (tr[u].ch[1] && key > tr[ tr[u].ch[1] ].val) {
                    int r = tr[u].ch[1];
                    tr[u].ch[1] = tr[r].ch[0];
                    tr[r].ch[0] = u;
                    pushup(u); u = r;
                }
                // 挂到左侧
                stkL[L++] = u;
                if (!tr[u].ch[1]) break;
                u = tr[u].ch[1];
            }
            else break;
        }
        else {
            // 按第 k 小
            if (tr[u].ch[0] && key <= tr[ tr[u].ch[0] ].sz) {
                // zig-zig
                if (tr[ tr[u].ch[0] ].ch[0] &&
                    key <= tr[ tr[ tr[u].ch[0] ].ch[0] ].sz) {
                    int l = tr[u].ch[0];
                    tr[u].ch[0] = tr[l].ch[1];
                    tr[l].ch[1] = u;
                    pushup(u); u = l;
                }
                // 挂到右侧
                stkR[R++] = u;
                u = tr[u].ch[0];
            }
            else {
                int leftSz = tr[u].ch[0] ? tr[ tr[u].ch[0] ].sz : 0;
                if (key > leftSz + tr[u].cnt) {
                    key -= leftSz + tr[u].cnt;
                    // zig-zig
                    if (tr[u].ch[1] && key > tr[ tr[u].ch[1] ].sz) {
                        int r = tr[u].ch[1];
                        tr[u].ch[1] = tr[r].ch[0];
                        tr[r].ch[0] = u;
                        pushup(u); u = r;
                    }
                    // 挂到左侧
                    stkL[L++] = u;
                    u = tr[u].ch[1];
                }
                else break;
            }
        }
    }
    // 重新拼接左右
    while (L) {
        int p = stkL[--L];
        tr[p].ch[1] = tr[u].ch[0];
        pushup(p);
        tr[u].ch[0] = p;
        pushup(u);
    }
    while (R) {
        int p = stkR[--R];
        tr[p].ch[0] = tr[u].ch[1];
        pushup(p);
        tr[u].ch[1] = p;
        pushup(u);
    }
    root = u;
    return u;
}

// 插入一个值 x
void insert(int x) {
    if (!root) {
        root = ++top;
        tr[top] = {{0,0}, x, 1, 1, x};
        return;
    }
    splay(x, false);
    if (tr[root].val == x) {
        tr[root].cnt++;
    }
    else {
        int nd = ++top;
        tr[nd] = {{0,0}, x, 1, 1, x};
        if (x < tr[root].val) {
            tr[nd].ch[1] = root;
            tr[nd].ch[0] = tr[root].ch[0];
            tr[root].ch[0]  = 0;
            pushup(root);
        } else {
            tr[nd].ch[0] = root;
            tr[nd].ch[1] = tr[root].ch[1];
            tr[root].ch[1] = 0;
            pushup(root);
        }
        pushup(nd);
        root = nd;
    }
}

// 删除一个值 x（若 multi 次插入，只删除一次）
void erase(int x) {
    if (!root) return;
    splay(x, false);
    if (tr[root].val != x) return;  // 不存在
    if (tr[root].cnt > 1) {
        tr[root].cnt--;
        pushup(root);
        return;
    }
    // 真删节点
    if (!tr[root].ch[0] && !tr[root].ch[1]) {
        root = 0;
    }
    else if (!tr[root].ch[0]) {
        root = tr[root].ch[1];
    }
    else if (!tr[root].ch[1]) {
        root = tr[root].ch[0];
    }
    else {
        int L = tr[root].ch[0];
        int oldRoot = root;
        root = L;
        // 把左子树中最大值旋到根
        splay(INT_MAX, false);
        tr[root].ch[1] = tr[oldRoot].ch[1];
        pushup(root);
    }
}

// 查询 x 的排名（第几小），rank 从 1 开始
int getRank(int x) {
    if (!root) return 1;
    splay(x, false);
    int res = tr[ tr[root].ch[0] ].sz + 1;
    return res;
}

// 查询第 k 小元素的值
int getKth(int k) {
    if (!root || k <= 0 || k > tr[root].sz) return -1;
    splay(k, true);
    return tr[root].val;
}

// 查询前驱（小于 x 的最大值）
int getPrev(int x) {
    insert(x);
    splay(x, false);
    int u = tr[root].ch[0];
    while (tr[u].ch[1]) u = tr[u].ch[1];
    int res = tr[u].val;
    erase(x);
    return res;
}

// 查询后继（大于 x 的最小值）
int getNext(int x) {
    insert(x);
    splay(x, false);
    int u = tr[root].ch[1];
    while (tr[u].ch[0]) u = tr[u].ch[0];
    int res = tr[u].val;
    erase(x);
    return res;
}

// 快速读入
inline int readInt() {
    int x = 0, c = getchar();
    while (c < '0' || c > '9') c = getchar();
    for (; c >= '0' && c <= '9'; c = getchar())
        x = x * 10 + (c - '0');
    return x;
}

int main() {
    freopen("P6136_11.in", "r", stdin);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n = readInt();
    int m = readInt();

    // 插入初始 n 个元素
    for (int i = 0; i < n; i++) {
        int a = readInt();
        insert(a);
    }

    int lastAns = 0, ansXor = 0;
    while (m--) {
        int op = readInt(), x = readInt() ^ lastAns;
        switch (op) {
            case 1:
                insert(x);
                break;
            case 2:
                erase(x);
                break;
            case 3:
                lastAns = getRank(x) - 1;
                ansXor ^= lastAns;
                break;
            case 4:
                lastAns = getKth(x);
                ansXor ^= lastAns;
                break;
            case 5:
                lastAns = getPrev(x);
                ansXor ^= lastAns;
                break;
            case 6:
                lastAns = getNext(x);
                ansXor ^= lastAns;
                break;
        }
    }

    printf("%d\n", ansXor);
    return 0;
}

template <typename T>
T inv(const T& x, const T& y) {
    assert(x != 0);
    T u = 0, v = 1, a = x, m = y, t;
    while (a != 0) {
        t = m / a;
        swap(a, m -= t * a);
        swap(u -= t * v, v);
    }
    assert(m == 1);
    return u;
}

template <typename T>
class Modular {

public:
    using Type = typename decay<decltype(T::value)>::type;

    constexpr Modular() : value() {}
    template <typename U> Modular(const U& x) { value = normalize(x); }

    template <typename U>
    static Type normalize(const U& x) {
        Type v = static_cast<Type>((-mod() <= x && x < mod()) ? x : x % mod());
        if (v < 0) v += mod();
        return v;
    }

    const Type& operator()() const { return value; }
    template <typename U> explicit operator U() const { return static_cast<U>(value); }
    constexpr static Type mod() { return T::value; }

    Modular& operator+=(const Modular& other) {
        if ((value += other.value) >= mod()) value -= mod();
        return *this;
    }
    Modular& operator-=(const Modular& other) {
        if ((value -= other.value) < 0) value += mod();
        return *this;
    }
    template <typename U> Modular& operator+=(const U& other) { return *this += Modular(other); }
    template <typename U> Modular& operator-=(const U& other) { return *this -= Modular(other); }
    Modular& operator++() { return *this += 1; }
    Modular& operator--() { return *this -= 1; }
    Modular operator++(int) {
        Modular result(*this);
        *this += 1;
        return result;
    }
    Modular operator--(int) {
        Modular result(*this);
        *this -= 1;
        return result;
    }
    Modular operator-() const { return Modular(-value); }

    template <typename U = T>
    typename enable_if<is_same<typename Modular<U>::Type, int>::value, Modular>::type& operator*=(const Modular& rhs) {
#ifdef _WIN32
        uint64_t x = static_cast<int64_t>(value) * static_cast<int64_t>(rhs.value);
        uint32_t xh = static_cast<uint32_t>(x >> 32), xl = static_cast<uint32_t>(x), d, m;
        asm(
            "divl %4; 
    "
            : "=a"(d), "=d"(m)
            : "d"(xh), "a"(xl), "r"(mod()));
        value = m;
#else
        value = normalize(static_cast<int64_t>(value) * static_cast<int64_t>(rhs.value));
#endif
        return *this;
    }
    template <typename U = T>
    typename enable_if<is_same<typename Modular<U>::Type, long long>::value, Modular>::type& operator*=(const Modular& rhs) {
        long long q = static_cast<long long>(static_cast<long double>(value) * rhs.value / mod());
        value = normalize(value * rhs.value - q * mod());
        return *this;
    }
    template <typename U = T>
    typename enable_if<!is_integral<typename Modular<U>::Type>::value, Modular>::type& operator*=(const Modular& rhs) {
        value = normalize(value * rhs.value);
        return *this;
    }

    Modular& operator/=(const Modular& other) { return *this *= Modular(inv(other.value, mod())); }

    friend const Type& abs(const Modular& x) { return x.value; }
    template <typename U> friend bool operator==(const Modular<U>& lhs, const Modular<U>& rhs);
    template <typename U> friend bool operator<(const Modular<U>& lhs, const Modular<U>& rhs);
    template <typename V, typename U> friend V& operator>>(V& stream, Modular<U>& number);

private:
    Type value;
};

template <typename T> bool operator==(const Modular<T>& lhs, const Modular<T>& rhs) { return lhs.value == rhs.value; }
template <typename T, typename U> bool operator==(const Modular<T>& lhs, U rhs) { return lhs == Modular<T>(rhs); }
template <typename T, typename U> bool operator==(U lhs, const Modular<T>& rhs) { return Modular<T>(lhs) == rhs; }

template <typename T> bool operator!=(const Modular<T>& lhs, const Modular<T>& rhs) { return !(lhs == rhs); }
template <typename T, typename U> bool operator!=(const Modular<T>& lhs, U rhs) { return !(lhs == rhs); }
template <typename T, typename U> bool operator!=(U lhs, const Modular<T>& rhs) { return !(lhs == rhs); }

template <typename T> bool operator<(const Modular<T>& lhs, const Modular<T>& rhs) { return lhs.value < rhs.value; }

template <typename T> Modular<T> operator+(const Modular<T>& lhs, const Modular<T>& rhs) { return Modular<T>(lhs) += rhs; }
template <typename T, typename U> Modular<T> operator+(const Modular<T>& lhs, U rhs) { return Modular<T>(lhs) += rhs; }
template <typename T, typename U> Modular<T> operator+(U lhs, const Modular<T>& rhs) { return Modular<T>(lhs) += rhs; }

template <typename T> Modular<T> operator-(const Modular<T>& lhs, const Modular<T>& rhs) { return Modular<T>(lhs) -= rhs; }
template <typename T, typename U> Modular<T> operator-(const Modular<T>& lhs, U rhs) { return Modular<T>(lhs) -= rhs; }
template <typename T, typename U> Modular<T> operator-(U lhs, const Modular<T>& rhs) { return Modular<T>(lhs) -= rhs; }

template <typename T> Modular<T> operator*(const Modular<T>& lhs, const Modular<T>& rhs) { return Modular<T>(lhs) *= rhs; }
template <typename T, typename U> Modular<T> operator*(const Modular<T>& lhs, U rhs) { return Modular<T>(lhs) *= rhs; }
template <typename T, typename U> Modular<T> operator*(U lhs, const Modular<T>& rhs) { return Modular<T>(lhs) *= rhs; }

template <typename T> Modular<T> operator/(const Modular<T>& lhs, const Modular<T>& rhs) { return Modular<T>(lhs) /= rhs; }
template <typename T, typename U> Modular<T> operator/(const Modular<T>& lhs, U rhs) { return Modular<T>(lhs) /= rhs; }
template <typename T, typename U> Modular<T> operator/(U lhs, const Modular<T>& rhs) { return Modular<T>(lhs) /= rhs; }

template <typename T, typename U>
Modular<T> qpow(const Modular<T>& a, const U& b) {
    assert(b >= 0);
    Modular<T> x = a, res = 1;
    for (T p = b; p; x *= x, p >>= 1)
        if (p & 1) res *= x;
    return res;
}

template <typename T> bool IsZero(const Modular<T>& number) { return number() == 0; }
template <typename T> std::string to_string(const Modular<T>& number) { return to_string(number()); }

// U == std::ostream? but done this way because of fastoutput
template <typename U, typename T> U& operator<<(U& stream, const Modular<T>& number) { return stream << number(); }

// U == std::istream? but done this way because of fastinput
template <typename U, typename T>
U& operator>>(U& stream, Modular<T>& number) {
    typename common_type<typename Modular<T>::Type, long long>::type x;
    stream >> x;
    number.value = Modular<T>::normalize(x);
    return stream;
}

// using ModType = int;
// struct VarMod { static ModType value; };
// ModType VarMod::value;
// ModType& md = VarMod::value;// for mod can change
// using Mint = Modular<VarMod>;

constexpr int md = (int)1e9 + 7; 
using Mint = Modular<std::integral_constant<decay<decltype(md)>::type, md>>;

struct Fact {
    std::vector<Mint> fact, factinv;
    const int n;
    Fact(const int& _n) : n(_n), fact(_n + 1, Mint(1)), factinv(_n + 1) {
        for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;
        factinv[n] = Modular<3>::inv(fact[n](), md);
        for (int i = n; i; --i) factinv[i - 1] = factinv[i] * i;
    }
    Mint C(const int& n, const int& k) {
        if (n < 0 || k < 0 || n < k) return 0;
        return fact[n] * factinv[k] * factinv[n - k];
    }
    Mint A(const int& n, const int& k) {
        if (n < 0 || k < 0 || n < k) return 0;
        return fact[n] * factinv[n - k];
    }
};