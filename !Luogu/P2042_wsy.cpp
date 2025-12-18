#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <queue>
#include <vector>
#include <set>
#include <cstring>
#include <unordered_map>
#include <bitset>
#include <cassert>

using namespace std;

const int maxn = 1 << 20;
char in[maxn],out[maxn],*p1=in,*p2=in,*p3=out;
// #define getchar() (p1==p2&&(p2=(p1=in)+fread(in,1,maxn,stdin),p1==p2)?EOF:*p1++)
#define flush() (fwrite(out,1,p3-out,stdout))
#define putchar(x) (p3==out+maxn&&(flush(),p3=out),*p3++=(x))
template<typename type>
void read(type &x)
{
    x = 0;
    int f = 1;
    char c = getchar();
    while (c < 48 || c > 57) 
    {
        if (c == '-') f = -1;
        c = getchar();
    }
    while (c >= 48 && c <= 57)
        x = x * 10 + c - 48, c = getchar();
    x *= f;
}

template<typename type, typename ...T>
void read(type &x, T &...y)
{
    read(x), read(y...);
}

template<typename type>
inline void write(type x,bool mode=1)//0为空格，1为换行
{
    x < 0 ? x =- x, putchar('-') : 0;
    static short Stack[50], top(0);
    do Stack[++top]=x%10,x/=10; while(x);
    while(top) putchar(Stack[top--]|48);
    mode?putchar('\n'):putchar(' ');
}

#define ll long long

const int N = 500005, M = 1500005;
const int INF = 100000000;
const int mod = 998244353;

queue<int> num;
int n, m, rt, idx;
int cnt[N], sz[N], l[N], r[N], sum[N], val[N], mx[N], ch[N][2], fa[N], rev[N], rep[N];
char s[20];

template<typename T>
inline T max(T a, T b, T c) {
    return max(a, max(b, c));
}

int newNode(int x, int c) {
    idx = num.front();
    num.pop();
    fa[idx] = ch[idx][0] = ch[idx][1] = rev[idx] = 0;
    rep[idx] = INF;
    cnt[idx] = sz[idx] = c;
    l[idx] = r[idx] = max(0, x);
    mx[idx] = val[idx] = sum[idx] = x;
    return idx;
}

bool dir(int x) {
    return x == ch[fa[x]][1];
}

void pushup(int x) {
    int ls = ch[x][0], rs = ch[x][1];
    sz[x] = sz[ls] + sz[rs] + cnt[x];
    // assert(l[ls] >= 0 && r[ls] >= 0 && r[ls] >= 0 && l[rs] >= 0);
    mx[x] = max(mx[ls], mx[rs], r[ls] + val[x] + l[rs]);
    l[x] = max(l[ls], sum[ls] + val[x] + l[rs]);
    r[x] = max(r[rs], sum[rs] + val[x] + r[ls]);
    sum[x] = sum[ls] + sum[rs] + val[x];
}

void oper(int x, int re, int rp) {
    if (rp != INF) {
        if (rp > 0) l[x] = r[x] = mx[x] = sum[x] = sz[x] * rp;
        else l[x] = r[x] = 0, mx[x] = rp, sum[x] = sz[x] * rp;
        val[x] = rp, rep[x] = rp, rev[x] = 0;
    }
    if (re && rep[x] == INF) swap(ch[x][0], ch[x][1]), rev[x] ^= 1, swap(l[x], r[x]);
}

void pushdown(int x) {
    if (ch[x][0]) oper(ch[x][0], rev[x], rep[x]);
    if (ch[x][1]) oper(ch[x][1], rev[x], rep[x]);
    rep[x] = INF, rev[x] = 0;
}

void rotate(int x) {
    int y = fa[x], z = fa[y];
    pushdown(y), pushdown(x);
    bool r = dir(x);
    ch[y][r] = ch[x][!r];
    ch[x][!r] = y;
    if (z) ch[z][dir(y)] = x;
    fa[ch[y][r]] = y;
    fa[y] = x;
    fa[x] = z;
    pushup(y), pushup(x);
}

void splay(int x, int z = 0, bool flag = 1) {
    for (int y; (y = fa[x]) != z; rotate(x)) {
        if (fa[y] != z) rotate(dir(x) == dir(y) ? y : x);
    }
    if (z == 0 && flag) rt = x;
}

void output() {
    printf("sz: ");
    for (int i = 1; i <= idx; i++) printf("%d ", sz[i]);
    printf("\n");
    for (int i = 1; i <= idx; i++) {
        if (ch[i][0]) printf("%d %d L\n", i, ch[i][0]);
        if (ch[i][1]) printf("%d %d R\n", i, ch[i][1]);
    }
}

void build(int n) {
    rt = newNode(-INF, 0);
    for (int i = 1; i <= n; i++) {
        int v;
        read(v);
        int x = newNode(v, 1);
        ch[x][0] = rt;
        fa[rt] = x;
        rt = x;
    }
    int x = newNode(-INF, 1);
    ch[x][0] = rt;
    fa[rt] = x;
    rt = x;
    splay(1);
}

int newTree(int n) {
    int v;
    read(v);
    int top = newNode(v, 1);
    int tmp = top;
    for (int i = 2; i <= n; i++) {
        read(v);
        int x = newNode(v, 1);
        ch[x][0] = top;
        fa[top] = x;
        top = x;
    }
    splay(tmp, 0, 0);
    // output();
    return tmp;
}

int findkth(int k) {
    if (k == 0) return 1;
    int x = rt;
    for (pushdown(x); ; pushdown(x)) {
        // if (x) printf("%d %d %d\n", rt, x, sz[x]);
        if (sz[ch[x][0]] >= k) {
            x = ch[x][0];
        } else {
            k -= sz[ch[x][0]] + cnt[x];
            if (k <= 0) return x;
            x = ch[x][1];
        }
    }
}

void dfs(int x) {
    num.push(x);
    if (ch[x][0]) dfs(ch[x][0]);
    if (ch[x][1]) dfs(ch[x][1]);
}

int main() {
    // freopen("P2042_2.in", "r", stdin);
    // freopen("1.out", "w", stdout);
    memset(rep, 0x3f, sizeof(rep));
    mx[0] = -INF;
    for (int i = 1; i < N; i++) num.push(i);
    read(n, m);
    build(n);
    // output();
    // for (int i = 1; i <= idx; i++) {
    //     printf("%d ", sum[i]);
    // }
    // printf("\n");
    // for (int i = 0; i < idx; i++) printf("%d ", val[findkth(i)]);
    while (m--) {
        // cout << "NS" << endl;
        scanf("%s", s);
        if (s[0] == 'I') {
            int pos, tot;
            read(pos, tot);
            int top = newTree(tot);
            int a = findkth(pos), b = findkth(pos + 1);
            splay(a), splay(b, a);
            ch[b][0] = top, fa[top] = b;
            pushup(b), pushup(a);
        } else if (s[0] == 'D') {
            int pos, tot;
            read(pos, tot);
            int a = findkth(pos - 1), b = findkth(pos + tot);
            splay(a), splay(b, a);
            dfs(ch[b][0]);
            fa[ch[b][0]] = 0, ch[b][0] = 0;
            pushup(b), pushup(a);
        } else if (s[0] == 'M' && s[2] == 'K') {
            int pos, tot, c;
            read(pos, tot, c);
            int a = findkth(pos - 1), b = findkth(pos + tot);
            splay(a), splay(b, a);
            oper(ch[b][0], 0, c);
            splay(ch[b][0]);
        } else if (s[0] == 'R') {
            int pos, tot;
            read(pos, tot);
            int a = findkth(pos - 1), b = findkth(pos + tot);
            splay(a), splay(b, a);
            oper(ch[b][0], 1, INF);
            splay(ch[b][0]);
        } else if (s[0] == 'G') {
            int pos, tot;
            read(pos, tot);
            int a = findkth(pos - 1), b = findkth(pos + tot);
            splay(a), splay(b, a), pushdown(a), pushdown(b);
            write(sum[ch[b][0]]);
        } else {
            // putchar('@');
            write(mx[rt]);
        }
        // for (int i = 0; i < idx; i++) printf("%d ", val[findkth(i)]);
        // cout << "size:" << sz[rt] <<  " " << rt << endl;
    }
    flush();
    return 0;
}