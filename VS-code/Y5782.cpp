#include<bits/stdc++.h>
#define ll long long
using namespace std;
ll ans=1e18;
int t,n,W,cntp,cntq,g[505][505],a[505];
struct node{
    int x,y,z,a,b,c,d,e,f;
}p[1005];
void dfs(int x){
    if(x>n){
        ll cnt=0;
        for(int i=1;i<=n;i++){
            cnt+=a[i];
        }
        for(int i=1;i<=cntp;i++){
            cnt+=1ll*p[i].a*abs(a[p[i].x]-a[p[i].y])+p[i].b*abs(a[p[i].y]-a[p[i].z])+p[i].c*abs(a[p[i].z]-a[p[i].x])+p[i].d*(a[p[i].x]-a[p[i].y])+p[i].e*(a[p[i].y]-a[p[i].z])+p[i].f*(a[p[i].z]-a[p[i].x]);
        }
        ans=min(ans,cnt);
        return;
    }
    int ok0=1,ok1=1,can0=1,can1=1;
    for(int i=1;i<=n;i++){
        if(a[i]<0){
            if(g[i][x]==2) ok1=0;
            else if(g[i][x]==3) ok0=0;
            else if(g[i][x]==4) return;
            else if(g[i][x]==5) ok1=0;
        }
        else{
            if(g[i][x]==1) ok0=0;
            else if(g[i][x]==3) return;
            else if(g[i][x]==4) ok1=0;
            else if(g[i][x]==5) ok0=0;
        }
    }
    for(int i=1;i<=n;i++){
        if(g[x][i]==3) can1=0;
        else if(g[x][i]==4) can0=0;
    }
    if(ok0&&can0){
        a[x]=-1;
        dfs(x+1);
    }
    if(ok1&&can1){
        a[x]=1;
        dfs(x+1);
    }
}
int main(){
    cin>>t;
    while(t--){
        ans=1e18;
        memset(g,0,sizeof(g));
        cin>>n>>W>>cntp>>cntq;
        for(int i=1;i<=cntp;i++){
            scanf("%d%d%d%d%d%d%d%d%d",&p[i].x,&p[i].y,&p[i].z,&p[i].a,&p[i].b,&p[i].c,&p[i].d,&p[i].e,&p[i].f);
        }
        for(int i=1;i<=cntq;i++){
            int x,y,r;
            scanf("%d%d%d",&x,&y,&r);
            if(r==1){
                g[x][y]=g[y][x]=5;
            }
            else if(r==0){
                if(g[x][y]==5) continue;
                if(g[x][y]==3) continue;
                if(g[x][y]==2){
                    g[x][y]=g[y][x]=5;
                    continue;
                }
                g[x][y]=1;
                g[y][x]=2;
            }
            else{
                g[x][y]=3;
                g[y][x]=4;
            }
        }
        dfs(1);
        printf("%lld\n",ans);
    }
    return 0;
}