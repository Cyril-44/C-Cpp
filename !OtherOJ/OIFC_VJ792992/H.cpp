#include<bits/stdc++.h>
#define ll long long
#define ID(x,y) (1ll*(x-1)*n+y)
using namespace std;
int t,n,m,fa[100005],du[100005],k1,k2;
struct node{
    int x,y,op;
}e1[200005],e2[200005];
vector<int> g[100005];
unordered_map<ll,int> mp;
void dfs(int x,int fth,int tar){
    fa[x]=tar;
    for(int y:g[x]){
        if(y!=fth){
            dfs(y,x,tar);
        }
    }
}
void out(){
    puts("Yes");
    for(int i=1;i<=k1;i++){
        if(e1[i].op){
            printf("%d %d\n",e1[i].x,e1[i].y);
        }
    }
    for(int i=1;i<=k2;i++){
        if(e2[i].op){
            printf("%d %d\n",e2[i].x,e2[i].y);
        }
    }
}
int find(int x){
    if(x==fa[x]) return x;
    return fa[x]=find(fa[x]);
}
int main(){
    cin>>t;
    while(t--){
        scanf("%d%d",&n,&m);
        k1=k2=0;
        mp.clear();
        for(int i=1;i<=n;i++){
            g[i].clear();
            du[i]=0;
            fa[i]=i;
        }
        for(int i=1;i<=m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
			if(x==y) continue;
            int fx=find(x),fy=find(y);
            if(fx!=fy){
                fa[fy]=fx;
                g[x].push_back(y);
                g[y].push_back(x);
                du[x]++;
                du[y]++;
                e1[++k1]=(node){x,y,1};
                mp[ID(x,y)]=k1;
                mp[ID(y,x)]=k1;
            }
            else{
                e2[++k2]=(node){x,y,0};
            }
        }
        int id=0;
        for(int i=1;i<=n;i++){
            if(du[i]*2>n){
                id=i;
                break;
            }
        }
        if(!id){
            out();
            continue;
        }
        for(int y:g[id]){
            dfs(y,id,y);
        }
        for(int i=1;i<=k2;i++){
			if(e2[i].x==id||e2[i].y==id) continue;
            int fx=find(e2[i].x),fy=find(e2[i].y);
            if(fx!=fy){
                if(du[fx]>du[fy]){
                    swap(e2[i].x,e2[i].y);
                    swap(fx,fy);
                }
                du[fy]--;
                du[id]--;
                du[e2[i].x]++;
                du[e2[i].y]++;
                if(du[e2[i].x]*2>n||du[e2[i].y]*2>n){
                    du[fy]++;
                    du[id]++;
                    du[e2[i].x]--;
                    du[e2[i].y]--;
                    continue;
                }
                fa[fy]=fx;
                e2[i].op=1;
                e1[mp[ID(fy,id)]].op=0;
            }
        }
        if(du[id]*2>n){
            puts("No");
        }
        else out();
    }
    return 0;
}