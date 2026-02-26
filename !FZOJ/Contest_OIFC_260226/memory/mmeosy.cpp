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
#include <map>

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

const int N = 500005;
const int mod = 1000000007;

int T, n, k;
int cnt[N];
char ch[N], s[N], t[N];

inline void get(int l, int r) {
    // printf("!!: %d\n", l, r);
    for (int i = l; i <= r; i++) {
        if (s[i - 1] == '0') s[i] = '1';
        else s[i] = '0';
    }
}

bool solve1(char *s) {
    int j = n + 1;
    bool flag = 0;
    for (int i = 1; i <= n; i++) s[i] = ch[i];
    s[n + 1] = '\0';
    for (int i = n; i >= 1; i--) {
        if (ch[i] != '?') {
            flag = 1;
            if (i == j - 1) {
                j = i;
                continue;
            }
            if (j == n + 1) {
                get(i + 1, n);
            } else if (ch[i] == ch[j]) {
                if (ch[i] == '0') {
                    if ((j - i - 1) & 1) get(i + 1, j - 1);
                    else s[i + 1] = '0', get(i + 2, j - 1);
                } else {
                    if ((j - i - 1) & 1) get(i + 1, j - 1);
                    else s[i + 1] = s[i + 2] = '0', get(i + 3, j - 1);
                }
            } else {
                if (ch[i] == '0') {
                    if ((j - i) & 1) get(i + 1, j - 1);
                    else s[i + 1] = '0', get(i + 2, j - 1);
                } else {
                    if ((j - i) & 1) get(i + 1, j - 1);
                    else s[i + 1] = s[i + 2] = '0', get(i + 3, j - 1);
                }
            }
            j = i;
        }
    }
    if (j == n + 1) {
        s[1] = '0';
        get(2, n);
    } else {
        for (int i = j - 1; i >= 1; i--) {
            if (s[i + 1] == '1') s[i] = '0';
            else s[i] = '1';
        }
    }
    // printf("string s: %s\n", s + 1);
    cnt[n] = 0;
    cnt[n + 1] = 0;
    for (int i = n - 1; i >= 1; i--) {
        cnt[i] = cnt[i + 1] + (s[i] != s[i + 1]);
    }
    if (cnt[1] < k) return false;
    // printf("cnt : !!; %d\n", cnt[1]);
    int tag = 0;
    if ((cnt[1] & 1) != (k & 1)) {
        // printf("FDSDSF\n");
        if (ch[1] == '?' && s[1] == '1') {
            tag = 1;
            s[1] = '0';
            
        } else if (ch[n] == '?') {
            int p = n;
            while (ch[p - 1] == '?') p--;
            // printf("here!!: %d\n",p);
            if (s[p] == '1') {
                s[p] = '0';
                get(p + 1, n);
            } else {
                if (p + 1 <= n) s[p + 1] = '0', get(p + 2, n);
                else s[p] = '1';
            }
            tag = n;
        } else if (ch[1] == '?' && s[1] == '0') {
            tag = 1;
            s[1] = '1';
            if (2 < j) s[2] = '0';
            if (3 < j) s[3] = '0';
            get(4, j - 1);
        } else return false;
    }
    for (int i = n - 1; i >= 1; i--) {
        cnt[i] = cnt[i + 1] + (s[i] != s[i + 1]);
    }
    if (cnt[1] > k && ch[1] == '?' && s[1] == '1' && ch[n] == '?') {
        if (ch[1] == '?' && s[1] == '1') {
            tag = 1;
            s[1] = '0';
            
        } else if (ch[1] == '?' && s[1] == '0') {
            tag = 1;
            s[1] = '1';
            if (2 < j) s[2] = '0';
            if (3 < j) s[3] = '0';
            get(4, j - 1);
        }
        if (ch[n] == '?') {
            int p = n;
            while (ch[p - 1] == '?') p--;
            // printf("here!!: %d\n",p);
            if (s[p] == '1') {
                s[p] = '0';
                get(p + 1, n);
            } else {
                if (p + 1 <= n) s[p + 1] = '0', get(p + 2, n);
                else s[p] = '1';
            }
            tag = n;
        } 
    }
    for (int i = n - 1; i >= 1; i--) {
        cnt[i] = cnt[i + 1] + (s[i] != s[i + 1]);
    }
    // printf("strings2: %s %d %d\n", s + 1, cnt[1], k);

    int sum = 0;
    bool ff = (cnt[1] == k);
    for (int i = 1; i <= n; i++) {
        char tmp = s[i];
        if (ch[i] == '?' && i != tag && i != 1 && i != n) s[i] = '0';
        if (i > 1) sum += (s[i] != s[i - 1]);
        if (sum + (s[i] != s[i + 1]) + cnt[i + 1] < k) {
            s[i] = tmp;
            // printf("DFDFSDDFZXFV%s %d\n", s + 1, ff);
            return ff;
        }
        // printf("%s %d %d\n", s + 1, sum + (s[i] != s[i + 1]) + cnt[i + 1], k);
        if (sum + (i < n && s[i] != s[i + 1]) + cnt[i + 1] == k) {
            ff = 1;
        }
    }
    return ff;
}

inline void mem(int l, int r, char c) {
    for (int i = l; i <= r; i++) t[i] = c;
}

bool solve2(char *s) {
    int j = n + 1;
    bool flag = 0;
    for (int i = 1; i <= n; i++) s[i] = ch[i];
    s[n + 1] = '\0';
    for (int i = n; i >= 1; i--) {
        if (ch[i] != '?') {
            flag = 1;
            if (i == j - 1) {
                j = i;
                continue;
            }
            if (j == n + 1) {
                mem(i + 1, n, ch[i]);
            } else if (ch[i] == ch[j]) {
                mem(i + 1, j - 1, ch[i]);
            } else {
                mem(i + 1, j - 1, '0');
            }
            j = i;
        }
    }
    if (j == n + 1) {
        s[1] = '0';
        mem(2, n, '0');
    } else {
        mem(1, j - 1, ch[j]);
    }
    cnt[n] = 0;
    cnt[n + 1] = 0;
    for (int i = n - 1; i >= 1; i--) {
        cnt[i] = cnt[i + 1] + (s[i] != s[i + 1]);
    }
    int tag = 0;
    if ((cnt[1] & 1) != (k & 1)) {
        if (ch[n] == '?') {
            int p = n;
            while (ch[p - 1] == '?') p--;
            if (s[n - 1] == '0') s[n] = '1';
            else mem(p, n, '0');
            tag = n;
        } else if (ch[1] == '?') {
            tag = 1;
            if (s[n - 1] == '0') s[1] = '1';
            else mem(1, j - 1, '0');
        } else return false;
    }
    for (int i = n - 1; i >= 1; i--) {
        cnt[i] = cnt[i + 1] + (s[i] != s[i + 1]);
    }
    if (cnt[1] < k && ch[1] == '?' && s[1] == '1' && ch[n] == '?') {
        if (ch[n] == '?') {
            int p = n;
            while (ch[p - 1] == '?') p--;
            mem(p, n, '0');
            tag = n;
        }
        if (ch[1] == '?') {
            tag = 1;
            if (s[n - 1] == '0') s[1] = '1';
            else mem(1, j - 1, '0');
        }
    }
    for (int i = n - 1; i >= 1; i--) {
        cnt[i] = cnt[i + 1] + (s[i] != s[i + 1]);
    }
    // printf("string ch: %s\n", ch + 1);
    // printf("string t : %s\n", s + 1);
    // printf("cnt: %d\n", cnt[4]);
    if (cnt[1] > k) return false;
    int sum = 0;
    bool ff = (cnt[1] == k);
    for (int i = 1; i <= n; i++) {
        char tmp = s[i];
        if (ch[i] == '?' && i != tag && i != 1 && i != n) s[i] = '0';
        if (i > 1) sum += (s[i] != s[i - 1]);
        if (sum + (s[i] != s[i + 1]) + cnt[i + 1] > k) {
            s[i] = tmp;
            // printf("have ans: %s %d\n", s + 1, ff);
            return ff;
        }
        // printf("%s %d %d\n", s + 1, sum + (s[i] != s[i + 1]) + cnt[i + 1], k);
        if (sum + (i < n && s[i] != s[i + 1]) + cnt[i + 1] == k) {
            ff = 1;
        }
    }
    // printf("!!: %d\n", ff);
    return ff;
}

inline void work() {
    scanf("%d%d", &n, &k);
    scanf("%s", ch + 1);
    bool f1 = solve1(s);
    bool f2 = solve2(t);
    // printf("FDSSFDDSFD %d %d\n", f1, f2);
    if (!f1 && !f2) printf("-1\n");
    else if (!f1) printf("%s\n", t + 1);
    else if (!f2) printf("%s\n", s + 1);
    else {
        bool x = 1;
        for (int i = 1; i <= n; i++) {
            if (s[i] < t[i]) {
                x = 1;
                break;
            }
            if (s[i] > t[i]) {
                x = 0;
                break;
            }
        }
        if (x) printf("%s\n", s + 1);
        else printf("%s\n", t + 1);
    }
}

int main() {
    freopen("memory.in", "r", stdin);
    freopen("memory.out", "w", stdout);
    scanf("%d", &T);
    while (T--) {
        work();
    }
    flush();
    return 0;
}