#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
template<class T>
void in(T &x){
	char c=getchar();T f=1; x=0;
	while(c<'0'||c>'9'){
		if(c=='-') f=-1;
		c=getchar();
	}
	while(c>='0'&&c<='9'){
		x=x*10+c-'0';
		c=getchar();
	}
	x*=f;
}
const int N=300010,Mod=998244353;
ll ksm(ll a,ll b){
    ll res=1;
    while(b){
        if(b&1) res=res*a%Mod;
        a=a*a%Mod; b>>=1;
    }
    return res;
}
int du[N];
int n,m,u,v;ll ans;
int main(){
    freopen("graph.in","r",stdin);
    freopen("graph.out","w",stdout);
    in(n); in(m); ans=n;
    for(int i=1;i<=m;i++){
        in(u); in(v);
        du[u]++; du[v]++;
    }
    for(int i=1;i<=n;i++)
        ans+=Mod-ksm(du[i]+1,Mod-2);
    ans%=Mod;
    printf("%lld",ans);
    return 0;
}