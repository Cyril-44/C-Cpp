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
const int N=510,M=1000010;
int n,m;ll k;
struct Node{
    ll a,b;
    bool operator>(const Node &x)const{
        return a>x.a;
    }
}a[N];
int b[M];
ll f[N][N];
ll calc(int cnt){
    memset(f,0xc0,sizeof(f));
    f[0][0]=0;
    for(int i=1;i<=n;i++){
        f[i][0]=f[i-1][0]+a[i].a*max(0ll,a[i].b-cnt*k);
        for(int j=1;j<=cnt;j++){
            f[i][j]=max(f[i-1][j]+a[i].a*max(0ll,a[i].b-cnt*k),f[i-1][j-1]+a[i].a*max(0ll,b[j]-j*k+k));
        }
    }
    return f[n][cnt];
}
int main(){
    freopen("fief1.in","r",stdin);
    // printf("here\n");
    in(n); in(m); in(k);
    // printf("here\n");
    for(int i=1;i<=n;i++)
        in(a[i].a),in(a[i].b);
    sort(a+1,a+n+1,greater<Node>());
    // printf("here\n");
    for(int i=1;i<=m;i++)
        in(b[i]);
    sort(b+1,b+m+1,greater<int>());
    // printf("here\n");
    m=min(n,m);
    ll ans=0;
    for(int i=0;i<=m;i++) ans=max(ans,calc(i));
    printf("%lld",ans);
    return 0;
}