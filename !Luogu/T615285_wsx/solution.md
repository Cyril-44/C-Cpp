欧拉函数应用题。

考虑 $\frac{\operatorname{lcm}(a,b)}{\gcd(a,b)} = a$ 等价于什么。显然这等价于 $b = \gcd(a,b)^2$。考虑到这里有个平方，显然必须要从这个平方入手。我们考虑枚举 $\gcd(a,b)$。这个是 $10^7$ 级别的。 

考虑对于一个 $\gcd(a,b)$ 的满足条件的个数。显然 $b$ 是确定的，那么有多少个 $a \le b$ 满足条件呢。我们假设令 $a=k\gcd(a,b)$，那么显然满足条件的就是 $\gcd(k,\gcd(a,b)) = 1$。你考虑这等价于 $\gcd(a,b)$ 的欧拉函数。我们线性筛的时候求一下就好了。

你考虑这同时启示我们什么。我们发现 $b$ 的有效个数只有 $O(\sqrt n)$ 个。那么我们可以直接预处理前缀和，在每次查询的时候直接差分求解。我们考虑 $s_i$ 表示 $\gcd(a,b) \le i$ 的对数。然后你考虑直接把一段区间内的也乘上因为你需要注意有一些位置也要计算。这样在回答询问的时候你就可以做到直接差分以 $O(1)$ 了。

```cpp
#include <bits/stdc++.h>
using namespace std;

const int N = 1e7, P = 1e9 + 7;
int p[N + 5], tot;
int phi[N + 5], s1[N + 5], s2[N + 5];
bool flg[N + 5];

void init() {
    phi[1] = 1;
    for(int i = 2; i <= N; i++) {
        if(!flg[i]) {
            p[++tot] = i;
            phi[i] = i - 1;
        } 
        for(int j = 1; j <= tot && i * p[j] <= N; j++) {
            flg[i * p[j]] = 1;
            if(i % p[j] == 0) {
                phi[i * p[j]] = phi[i] * p[j];
                break;
            }
            phi[i * p[j]] = phi[i] * phi[p[j]];
        }
    }
    for(int i = 1; i <= N; i++)
        s1[i] = (s1[i - 1] + phi[i]) % P,
        s2[i] = (s2[i - 1] + 1ll * i * i % P * phi[i] % P) % P;
}

int query(long long n) {
    int k = sqrt(n);
    return ((1ll * (n + 1) * s1[k] % P - s2[k]) % P + P) % P;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    init();
    int T; cin >> T;
    while(T--) {
        long long l, r; cin >> l >> r;
        cout << ((query(r) - query(l - 1)) % P + P) % P << "\n";
    }
    return 0;
}
```