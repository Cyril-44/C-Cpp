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

const int N = 1000005;

int T, n;
char ch[N];
int a[N], f[N], nxt[N], tmp[2], ans[N];

int work(int len) {
    if (ans[len]) return ans[len];
    for (int i = 1; i <= n; i++) f[i] = 0x3f3f3f3f;
    int p[2] = {tmp[0], tmp[1]};
    for (int i = 1; i <= n; i++) {
        int ni = a[i] & 1 ^ 1;
        while (i - p[ni] > len) p[ni] = nxt[p[ni]];
        while (p[ni] < i && f[p[ni]] > n) p[ni] = nxt[p[ni]];
        if (p[ni] < i) f[i] = f[p[ni]] + 1;
        // printf("%d ", p[ni]);
    }
    return ans[len] = (f[n] <= n ? f[n] : -1);
}

int main() {
    scanf("%d", &T);
    while (T--) { 
        scanf("%s", ch + 1);
        n = strlen(ch + 1);
        for (int i = 1; i <= n; i++) {
            a[i] = (ch[i] == '1');
            a[i] += a[i - 1];
            ans[i] = 0;
        }
        tmp[0] = tmp[1] = n + 1;
        for (int i = n; i >= 0; i--) {
            int ni = a[i] & 1;
            nxt[i] = tmp[ni];
            tmp[ni] = i;
        }
        for (int len = 1; len <= n; len++) {
            int now = work(len);
            int l = len + 1, r = n;
            while (l <= r) {
                int mid = (l + r) >> 1;
                int tt = work(mid);
                if (tt == now) l = mid + 1;
                else r = mid - 1;
            }
            for (int i = len; i <= r; i++) ans[i] = now;
            len = r; 
        }
        for (int i = 1; i <= n; i++) write(ans[i], i == n);
    }
    flush();
    return 0;
}
