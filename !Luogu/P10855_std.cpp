#include<bits/stdc++.h>
using namespace std;
using ui=unsigned int;
using ll=long long;
using ull=unsigned long long;
using i128=__int128;
using u128=__uint128_t;
using pii=pair<int,int>;
#define fi first
#define se second
constexpr int N=2e5+5,mod=1e9+7;
int n,m,k,val;
inline ll qpow(ll a,ll b){
	ll res=1;
	for(;b;b>>=1,a=a*a%mod)
		if(b&1)res=res*a%mod;
	return res;
}
int p[N],plen,mu[N],lpf[N];bool v[N];
void sieve(int n){
	mu[1]=1;
	for(int i=2;i<=n;i++){
		if(!v[i])p[++plen]=i,mu[i]=mod-1,lpf[i]=i;
		for(int j=1;i*p[j]<=n;j++){
			v[i*p[j]]=1;
			lpf[i*p[j]]=p[j];
			if(i%p[j]==0)break;
			mu[i*p[j]]=(mod-mu[i])%mod;
		}
	}
}
int X,prm[N],cnt[N],tot,dvs[N],len;
ll g[N];
void dfs(int x,ll cur){
	if(x==tot+1){
		dvs[++len]=cur;
		return;
	}
	for(int i=0;i<=cnt[x];i++,cur*=prm[x])
		dfs(x+1,cur);
}
void init(int x){
	tot=len=0;X=x;
	for(;x>1;x/=lpf[x]){
		if(prm[tot]!=lpf[x])
			prm[++tot]=lpf[x],cnt[tot]=1;
		else
			++cnt[tot];
	}
	dfs(1,1);
}
ll ask(int n){ // sum_{1<=i<=n} gcd(x,i)^k
	ll res=0;
	for(int i=1;i<=len;i++)
		res=(res+(n/dvs[i])*g[dvs[i]])%mod;
	return res;
}
ll f(int l,int r){
	if(!l&&!r)return qpow(X,k);
	else if(!l)return (qpow(X,k)+ask(r))%mod;
	else if(l==1)return ask(r);
	else return (ask(r)-ask(l-1)+mod)%mod;
}
ll get(int v,int n){ // sum_i f(i)*[i xor v <= n]
	ll res=0;
	for(int i=m;i>=0;i--)
		if(n>>i&1)
			res=(res+f(v&~((1<<i)-1),v|((1<<i)-1)))%mod,v^=1<<i;
	return (res+f(v,v))%mod;
}
void conv(){
	for(int i=1;i<=n;i++)g[i]=0;
	for(int i=1;i<=n;i++){
		ll v=qpow(i,k);
		for(int j=1;i*j<=n;j++)
			g[i*j]=(g[i*j]+v*mu[j])%mod;
	}
    for (int i = 1; i <= n; i++)
        printf("%d ", g[i]);
    putchar('\n');
}
int main(){
	ios::sync_with_stdio(false);
	cin.tie(0),cout.tie(0);
	int _Test;cin>>_Test;
	sieve(2e5);
    for (int i = 1; i <= 20; i++)
        printf("%d ", mu[i] > 1 ? -1 : mu[i]);
    putchar('\n');
	while(_Test--){
		cin>>n>>k;m=__lg(n);
		conv();
		ll ans=0;
		for(int i=1;i<=n;i++){
			init(i);
			ans=(ans+get(i,n)-get(i,i-1)+mod)%mod;
		}
		cout<<ans<<'\n';
	}
	return 0;
}
