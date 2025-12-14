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
const int N=200010;
struct BIT{
    int a[N];
    int lowbit(int x){return x&-x;}
    void update(int x,int y){
        for(;x<=200000;x+=lowbit(x))
            a[x]+=y;
    }
    int query(int x){
        int ans=0;
        for(;x;x-=lowbit(x))
            ans+=a[x];
        return ans;
    }
}bit;
int n,m,a[N],b[N];
bool tag[N];
void work(){
    in(n); in(m);
    for(int i=1;i<=n;i++){
        in(a[i]);
        b[a[i]]=i; tag[i]=0;
        bit.update(i,1);
    }
    ll ans=0;
    for(int i=1;i<=n;i++){
        bit.update(b[i],-1);
        int cnt=(bit.query(b[i])+m-2)/(m-1);
        if(cnt&&b[i]-(cnt-1)*(m-1)<=m){
            ans+=cnt-1;
            tag[cnt]=1;
        }else ans+=cnt;
    }
    for(int i=1;i<=n;i++) ans+=tag[i];
    printf("%lld\n",ans);
}
int t;
int main(){
    freopen("maze.in","r",stdin);
    freopen("maze.out","w",stdout);
    in(t);
    while(t--) work();
    return 0;
}