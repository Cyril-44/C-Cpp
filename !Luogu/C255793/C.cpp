#include <cstdio>
#include <algorithm>
#include <cstring>
const int N = 1000005;
#include<type_traits>
#if!defined(FAST_IO)&&defined(_STDIO_H)
#define FAST_IO 1
#ifndef _FASTIO
#define _FASTIO 2
namespace FastIO{char*p1,*p2,buf[1<<16];__always_inline char nc(){return(p1==p2&&(p2=(p1=buf)+fread(buf,1,1<<16,stdin),p1==p2)?EOF:*p1++);}}
#endif
template<typename T,typename=typename std::enable_if<std::is_integral<T>::value,bool>::type>inline void in(T&x){int f=1;char ch=FastIO::nc();while(ch<'0'||ch>'9'){if(ch=='-'){f=-1,ch=FastIO::nc();break;}ch=FastIO::nc();}for(x=0;ch>='0'&&ch<='9';ch=FastIO::nc())x=(x<<3)+(x<<1)+(ch^'0');x*=f;}template<typename T,typename=typename std::enable_if<std::is_integral<T>::value,bool>::type>inline void out(T x){static char buf[64],stack[64];int topb(0),tops(0);if(x<0)buf[topb++]='-',x=-x;stack[tops++]=x%10,x/=10;while(x)stack[tops++]=x%10,x/=10;while(tops--)buf[topb++]=stack[tops]^'0';fwrite(buf,topb,64,stdout);}
#endif
int a[N], n, k;
inline bool check(const long long &s) {
    long long sum(0);
    for (int i = 1; i <= n; i++) {
        if (a[i] <= s) sum += s - a[i] + 1;
    }
    return sum <= k * s;
}
int main() {
#ifndef ONLINE_JUDGE
    freopen("ice6.in", "r", stdin);
    freopen("ice.out", "w", stdout);
#endif
    in(n), in(k);
    for (int i = 1; i <= n; i++) in(a[i]);
    long long l(0), r((long long)(1e12)), mid;
    while (l <= r) {
        mid = l + r >> 1;
        if (check(mid)) l = mid+1;
        else r = mid-1;
    }
    printf("%lld\n", r);
    return 0;
}