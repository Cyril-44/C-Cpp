#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int N = 100010;
const int MOD = 1000000007; // match code 1

int n;
struct Node
{
    int a, b;
} x[N];
int upl[N], dnl[N];

static inline ll tri(ll t)
{
    // t >= 0, returns t*(t+1)/2 % MOD
    return (t % MOD) * ((t + 1ll) % MOD) % MOD * ((MOD + 1ll) / 2) % MOD; // or cast and divide before mod
}

int main()
{
    scanf("%d", &n);

    // -INF / +INF sentinels
    memset(upl, 0xc0, sizeof(upl)); // very small
    memset(dnl, 0x3f, sizeof(dnl)); // very large

    for (int i = 1; i <= n; i++)
    {
        int a, b;
        scanf("%d%d", &a, &b);

        // adopt the same shift as code 1
        ++a;
        ++b;

        // aggregate constraints (do NOT overwrite)
        // upl at a-1 gets max b-1, dnl at a gets min b
        upl[a - 1] = max(upl[a - 1], b - 1);
        dnl[a] = min(dnl[a], b);
    }

    // build suffix max (upl) on [0..n] and prefix min (dnl) on [0..n]
    // ensure bases are defined
    upl[n] = max(upl[n], INT_MIN / 4);
    dnl[0] = min(dnl[0], INT_MAX / 4);

    for (int i = n - 1; i >= 0; i--)
        upl[i] = max(upl[i], upl[i + 1]);
    for (int i = 1; i <= n; i++)
        dnl[i] = min(dnl[i], dnl[i - 1]);

    // scan the active band [l..r] where feasible region exists
    int l = INT_MAX, r = -1;
    for (int i = 0; i <= n; i++)
    {
        if (upl[i] >= dnl[i])
        {
            l = min(l, i);
            r = max(r, i);
        }
    }
    if (r < l)
    {
        printf("0\n");
        return 0;
    } // empty

    // sliding window over i with a pointer 'to' and accumulators
    ll ans = 0;
    ll sum = 0; // sum of triangle counts per active column relative to current row i
    ll sq = 0;  // sum of lengths (upl[to]-dnl[i]+1) across active columns
    int to = l;

    for (int i = l; i <= r; i++)
    {
        // expand 'to' while column 'to' is still feasible at current row i
        while (to <= r && upl[to] >= dnl[i])
        {
            ll len = (ll)upl[to] - dnl[i] + 1;                                          // inclusive; len >= 1
            ll contrib = (len % MOD) * ((len + 1) % MOD) % MOD * ((MOD + 1) / 2) % MOD; // tri(len)
            sum += contrib;
            if (sum >= MOD)
                sum -= MOD;

            sq += len; // sq can be large; only reduce when adding to ans
            to++;
        }

        // accumulate current window contribution
        ans += sum;
        if (ans >= MOD)
            ans -= MOD;

        // remove column i as we advance i -> i+1
        if (upl[i] >= dnl[i])
        {
            ll len_i = (ll)upl[i] - dnl[i] + 1;
            ll contrib_i = (len_i % MOD) * ((len_i + 1) % MOD) % MOD * ((MOD + 1) / 2) % MOD;
            sum -= contrib_i;
            if (sum < 0)
                sum += MOD;

            sq -= len_i;
        }

        // now rows move from dnl[i] to dnl[i+1] (if any): each step increases all active columns’ lengths by 1
        if (i < r)
        {
            int nextFloor = dnl[i + 1]; // safe because i < r <= n
            for (int y = dnl[i] - 1; y >= nextFloor; --y)
            {
                // for each unit row shift, every active column adds exactly (to - i - 1) cells
                // total cells added = number of active columns = to - (i + 1)
                ll addCols = (ll)(to - (i + 1));
                // ans += sq + addCols (each column length increases by 1, which adds one per column)
                // Here sq tracked total lengths relative to current dnl[i]; moving one row adds 'addCols'.
                ll inc = (sq % MOD + addCols % MOD) % MOD;
                ans += inc;
                if (ans >= MOD)
                    ans -= MOD;

                sq += addCols;
            }
        }
    }

    printf("%lld\n", ans % MOD);
    return 0;
}
