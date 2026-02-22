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
#include <random>

using namespace std;

const int maxn = 1 << 20;
char in[maxn],out[maxn],*p1=in,*p2=in,*p3=out;
#define getchar() (p1==p2&&(p2=(p1=in)+fread(in,1,maxn,stdin),p1==p2)?EOF:*p1++)
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
    x < 0 ? x = -x, putchar('-') : 0;
    static short Stack[50], top(0);
    do Stack[++top]=x%10,x/=10; while(x);
    while(top) putchar(Stack[top--]|48);
    mode?putchar('\n'):putchar(' ');
}

#define ll long long

const int N = 505, V = 262144;

int n, m, k;
struct Node {
    int x, y;
};
struct Str {
    int x, y, v;
    bool operator <(const Str &a) const {
        return a.v < v;
    }
};
vector<Node> v, e;
int a[N][N];
int dx[4] = {0, 0, 1, -1};
int dy[4] = {1, -1, 0, 0};
bool vis[N][N];

ll get(vector<Node> &v) {
    memset(vis, 0, sizeof(vis));
    priority_queue<Str> q;
    for (auto x : v) {
        q.push({x.x, x.y, a[x.x][x.y]});
        vis[x.x][x.y] = 1;
    }
    ll ans = 0;
    while (!q.empty()) {
        auto p = q.top();
        q.pop();
        ans += p.v - a[p.x][p.y];
        // printf("%d %d %d\n", p.x, p.y, p.v);
        for (int i = 0; i < 4; i++) {
            int x = p.x + dx[i], y = p.y + dy[i];
            if (x >= 1 && x <= n && y >= 1 && y <= m && !vis[x][y]) {
                q.push({x, y, max(p.v, a[x][y])});
                vis[x][y] = 1;
            }
        }
    }
    return ans;
}  

int main() {
    read(n, m, k);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            read(a[i][j]);
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            bool flag = 1;
            for (int k = 0; k < 4; k++) {
                int x = i + dx[k];
                int y = j + dy[k];
                if (x >= 1 && x <= n && y >= 1 && y <= m && a[x][y] < a[i][j]) flag = 0;
            }
            if (flag) e.push_back({i, j});
        }
    }
    int T = min((int)e.size(), k);
    ll res = 0;
    while (T--) {
        ll tans = 0x3f3f3f3f3f3f3f3fll;
        Node tx;
        for (auto x : e) {
            v.push_back(x);
            ll ans = get(v);
            v.pop_back();
            if (ans < tans) tans = ans, tx = x;
        }
        v.push_back(tx);
        res ^= tans;
        // printf("%d %d\n", tx.x, tx.y);
    }
    write(res);
    flush();
    return 0;
}