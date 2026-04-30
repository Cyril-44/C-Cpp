#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
inline char gc() {
	const int BUF = 1e6;
	static char ch[BUF], *l, *r;
	return (l == r && (r = (l = ch) + fread(ch, 1, BUF, stdin), l == r)) ? EOF : *l++;
}
template <typename T> inline void in(T &x) {
	x = 0; int f = 1; char c = gc();
	for (; !isdigit(c); c = gc()) if (c == '-') f = -f;
	for (; isdigit(c); c = gc()) x = (x << 1) + (x << 3) + (c ^ 48);
	x *= f;
}
const int N=5000010;
int n,m,l,r; ll s,lb,rb;
int a[N],b[N];
int main(){
    in(n); in(m); in(s);
    for(int i=1;i<=m;i++){
        in(l); in(r);
        lb+=l; rb+=r;
        b[1]--; b[l+1]++;
        a[1]++; a[r+1]--;
    }
    if(s<lb||s>rb){
        printf("-1");
        return 0;
    }
    ll useful=0,useless=lb,ans=0,res;
    for(int i=1;i<=n;i++){
        a[i]+=a[i-1];
        b[i]+=b[i-1];
        useful+=a[i];
        useless+=b[i];
        if(useful+useless>s) res=(s-useless)/i;
        else res=useful/i;
        printf("%lld ",res);
        ans^=res*(n-i+1);
    }
    printf("%lld",ans);
}