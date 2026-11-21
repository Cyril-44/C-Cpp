#include "testlib.h"
#include <bits/stdc++.h>
using namespace std; using ll=long long; using G=vector<vector<pair<int,int>>>;

void dfs(G& g,int u,int f,int mx,ll d,vector<int>& a,vector<ll>& b){
    a[u]=mx;b[u]=d;
    for(auto [v,w]:g[u]) if(v!=f) dfs(g,v,u,max(mx,w),d+w,a,b);
}

int main(int argc,char** argv){
    registerTestlibCmd(argc,argv);
    int T=inf.readInt(); bool bad=0;
    for(int tc=1;tc<=T;tc++){
        int n=inf.readInt(); G A(n+1),B(n+1);
        for(int i=1,u,v,w;i<n;i++) u=inf.readInt(),v=inf.readInt(),w=inf.readInt(),A[u].push_back({v,w}),A[v].push_back({u,w});

        ll opt=ans.readLong();
        for(int i=1;i<n;i++) ans.readInt(),ans.readInt(),ans.readInt();
        ll out=ouf.readLong();
        if(out!=opt) quitf(_wa,"wrong diameter on tc%d, found%lld, exp%lld", tc, out, opt);

        vector<int> p(n+1); iota(p.begin(),p.end(),0);
        function<int(int)> F=[&](int x){return p[x]==x?x:p[x]=F(p[x]);};
        bool ok=1;
        for(int i=1,u,v,w;i<n;i++){
            u=ouf.readInt(1,n);v=ouf.readInt(1,n);w=ouf.readInt(1,1000000000);
            int x=F(u),y=F(v); if(x==y) ok=0; else p[x]=y;
            B[u].push_back({v,w});B[v].push_back({u,w});
        }

        vector<int> x(n+1),y(n+1); vector<ll> dx(n+1),dy(n+1); ll diam=0;
        if(ok) for(int s=1;s<=n;s++){
            dfs(A,s,0,0,0,x,dx); dfs(B,s,0,0,0,y,dy);
            if(x!=y){ok=0;break;}
            for(int i=1;i<=n;i++) diam=max(diam,dy[i]);
        }
        if(!ok||diam!=out) bad=1;
    }
    if(bad) quitp(0.5,"correct diameter, wrong construction");
    quitf(_ok,"correct");
}
