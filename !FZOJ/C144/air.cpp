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
const int N=2000010,Mod=998244353;
int n,m;
ll fac[N],inv[N];
ll C(int n,int m){
    if(n<0||m<0||n<m) return 0;
    return fac[n]*inv[m]%Mod*inv[n-m]%Mod;
}
char str[N];
int main(){
    freopen("ex_air02.in","r",stdin);
    // freopen("air.out","w",stdout);
    fac[0]=inv[0]=fac[1]=inv[1]=1;
    for(int i=2;i<=2000000;i++){
        fac[i]=fac[i-1]*i%Mod;
        inv[i]=(Mod-Mod/i)*inv[Mod%i]%Mod;
    }
    for(int i=2;i<=2000000;i++) inv[i]=inv[i-1]*inv[i]%Mod;
    in(n); in(m);
    ll ans=0;
    for(int i=1;i<=n;i++){
        // if(i<=n-m) (ans+=C(n+m,m))%=Mod;
        // else (ans+=C(n+m,m+i))%=Mod;
        (ans+=C(n+m,m+i))%=Mod;
    }
    printf("%lld",ans);
}