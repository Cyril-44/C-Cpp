#include <bits/stdc++.h>
using namespace std;

long long a[1000005];
int x[1000005];
int f[1000005];
int size[1000005];
struct node{
    long long x;int w;
    friend bool operator<(node x,node y){
        return x.x>y.x;
    }
};
int find(int x){
    if (x==f[x]||f[x]==-1) return x;
    return f[x]=find(f[x]);
}
vector<int>p[1000005];
priority_queue<node>q;
int main(){
//    freopen("mud13.in","r",stdin);
//    freopen("mud13.out","w",stdout);
    int t;
    scanf("%d",&t);
    while(t--){
        long long n;int m;
        scanf("%lld%d",&n,&m);
        for(int i=0;i<=m;i++) p[i].clear();
        for(int i=1;i<=m;i++) scanf("%lld",&a[i]);
        for(int i=1;i<=m;i++) scanf("%d",&x[i]);
        for(int i=1;i<=m;i++) f[i]=i,size[i]=1;
        for(int i=1;i<=m;i++){
            int u=find(i),v=find(x[i]);
            if (x[i]!=0){
                if (u==v) f[find(i)]=-1;
                else{
                    if (size[u]<size[v]){
                        f[u]=v;
                        size[v]+=size[u];
                    }
                    else f[v]=u,size[u]+=size[v];
                }
            }    
        }
        for(int i=1;i<=m;i++){
            if (find(i)!=-1){
                p[x[i]].push_back(i);
            }
        }
        while(!q.empty()) q.pop();
        int ans=0;
        node p2;p2.w=0,p2.x=0;
        q.push(p2);
        while(!q.empty()){
            node p1=q.top();
            q.pop();
            if (n<p1.x) break;
            n-=p1.x;
            ans++;
            for(int i=0;i<p[p1.w].size();i++){
                node rt;
                rt.w=p[p1.w][i];
                rt.x=a[rt.w];
                q.push(rt);
            }
        }
        printf("%d\n",ans-1);
    } 
    return 0;
}