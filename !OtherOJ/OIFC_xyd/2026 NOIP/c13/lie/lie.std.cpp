#include<bits/stdc++.h>
using namespace std;
const long long N=5005,P=1e9+7;long long n,dp[N][N],f[N][N],g[N][N],siz[N],ans=0;vector<int> a[N];
long long ksm(long long x,long long y){long long cnt=1;while(y) (y&1?(cnt=x*cnt%P,1):1),x=x*x%P,y>>=1;return cnt;}
struct node{
    long long f[N],finv[N];
    void init(int n){
        f[0]=1;for(int i=1;i<=n;i++) f[i]=f[i-1]*i%P;finv[n]=ksm(f[n],P-2);
        for(int i=n-1;i>=0;i--) finv[i]=finv[i+1]*(i+1)%P;
    }
}tr;
long long C(long long n,long long m){return tr.f[n]*tr.finv[m]%P*tr.finv[n-m]%P;}
void dfs1(int x,int fa){
    siz[x]=1,f[x][1]=1,g[x][1]=0,dp[x][1]=0;
    for(auto i:a[x]) if(i!=fa){
        dfs1(i,x);
        for(int j=siz[x];j>=1;j--) for(int k=siz[i];k>=1;k--){
            (f[x][j+k]+=f[x][j]*f[i][k]%P*C(j+k-1,j-1)%P)%=P;
            (g[x][j+k]+=g[x][j]*f[i][k]%P*C(j+k-2,j-2)%P+(g[i][k]+f[i][k])*f[x][j]%P*C(j+k-2,j-1)%P)%=P;
            if(j+k==2) (dp[x][j+k]+=f[x][1]*f[i][1]%P)%=P;
            if(j>2) (dp[x][j+k]+=dp[x][j]*f[i][k]%P*C(j+k-3,j-3)%P)%=P;
            if(k>1) (dp[x][j+k]+=dp[i][k]*f[x][j]%P*C(j+k-3,k-2)%P)%=P;
            if(j>1) (dp[x][j+k]+=g[x][j]*f[i][k]%P*C(j+k-3,j-2)%P*2ll%P+
                (g[i][k]+f[i][k])%P*f[x][j]%P*C(j+k-3,j-2)%P*2ll%P)%=P;
        }
        siz[x]+=siz[i];
    }
}
int main(){
	freopen("traveller.in","r",stdin);
	freopen("traveller.out","w",stdout);
	ios::sync_with_stdio(0);cin.tie(0),cout.tie(0);
	cin>>n;for(int i=1,j,k;i<n;i++) cin>>j>>k,a[j].push_back(k),a[k].push_back(j);
	tr.init(n),dfs1(1,0);
    for(int i=2;i<=n;i++) (ans+=dp[1][i]*tr.f[n-i+1]%P*tr.finv[n]%P)%=P;
    cout<<ans;
	return 0;
}
