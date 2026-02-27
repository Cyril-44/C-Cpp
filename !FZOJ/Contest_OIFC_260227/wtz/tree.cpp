#include<bits/stdc++.h>
using namespace std;
const int N=100010;
int n,c[N],u,v;
struct Node{
    int a[9],cnt,val;
    Node(){a[0]=a[1]=a[2]=a[3]=a[4]=a[5]=a[6]=a[7]=a[8]=cnt=val=0;}
    void insert(int x){
        if(cnt==5) return;
        for(int i=1;i<=cnt;i++){
            if(a[i]==x) return;
            if(a[i]>x){
                for(int j=cnt;j>=i;j--) a[j+1]=a[j];
                a[i]=x; cnt++;
                return;
            }
        }
        a[++cnt]=x;
    }
    bool operator<(const Node &x){
        assert(min(cnt, x.cnt) <= 8);
        for(int i=1;i<=min(cnt,x.cnt);i++)
            if(a[i]<x.a[i]) return 1;
        return 0;
    }
    bool operator==(const Node &x){
        assert(min(cnt, x.cnt) <= 8);
        if(x.cnt^cnt) return 0;
        for(int i=1;i<=cnt;i++) if(x.a[i]^a[i]) return 0;
        return 1;
    }
};
void print(const Node &x){
    putchar('{');
    for(int j=1;j<=x.cnt;j++) printf("%d ",x.a[j]);
    printf("}(%d,%d) ",x.cnt,x.val);
}
Node merge(const Node &x,const Node &y){
    Node res; int cur=0;
    for(int i=1;i<=x.cnt;i++){
        while(cur<y.cnt){
            if(y.a[cur+1]<=x.a[i])
                res.a[++res.cnt]=y.a[++cur];
            else break;
        }
        if(x.a[i]!=res.a[res.cnt]) res.a[++res.cnt]=x.a[i];
    }
    while(cur<y.cnt) res.a[++res.cnt]=y.a[++cur];
    res.val=x.val+y.val;
    // if(res.cnt>5) cerr<<x.cnt<<' '<<y.cnt<<' '<<res.cnt<<endl;
    return res;
}
vector<Node> l[N][5];
vector<int> g[N];
int lim[5]={0,20,20,20,20};
void refresh(int u,int i){
    if(u==55973&&i==4){
        printf("IAMHERE\n");
        for(const Node &x:l[u][4]) print(x),putchar('\n');
    }
    sort(l[u][i].begin(),l[u][i].end());
    if(u==55973&&i==4){
        printf("IAMHERE\n");
        for(const Node &x:l[u][4]) print(x),putchar('\n');
    }
    l[u][i].erase(unique(l[u][i].begin(),l[u][i].end()),l[u][i].end());
    if(u==55973&&i==4){
        printf("IAMHERE\n");
        for(const Node &x:l[u][4]) print(x),putchar('\n');
    }
    sort(l[u][i].begin(),l[u][i].end(),[&](const Node &x,const Node &y){return x.val<y.val;});
    if(u==55973&&i==4){
        printf("IAMHERE\n");
        for(const Node &x:l[u][4]) print(x),putchar('\n');
    }
    while(l[u][i].size()>lim[i]) l[u][i].pop_back();
    if(u==55973&&i==4){
        printf("IAMHERE\n");
        for(const Node &x:l[u][4]) print(x),putchar('\n');
    }
}
int ans=INT_MAX;
void dfs(int u,int fa){
    Node my; my.insert(c[u]); my.val++;
    // if(my.cnt>5) cerr<<"?"<<endl;
    l[u][1].push_back(my);
    for(int v:g[u]){
        if(v==fa) continue;
        dfs(v,u);
        for(int i=1;i<=4;i++){
            for(Node x:l[v][i]){
                x.insert(c[u]); x.val++;
                if(x.cnt==5) ans=min(ans,x.val);
                // if(x.cnt>5) cerr<<"??"<<endl;
                l[u][x.cnt].push_back(x);
            }
        }
    }
    for(int i=1;i<=4;i++) refresh(u,i);
    for(int i=2;i<=4;i++){
        refresh(u,i);
        for(int j=4;j>=i;j--)
            for(const Node &x:l[u][i])
                for(const Node &y:l[u][j]){
                    if(x.cnt>5) cerr<<u<<" "<<i<<" ???"<<endl;
                    if(y.cnt>5) cerr<<u<<" "<<j<<" ???"<<endl;
                    my=merge(x,y); my.val--;
                    // if(my.cnt==5){
                    //     print(x); print(y); printf("-> "); print(my); putchar('\n');
                    // }
                    if(my.cnt>j&&my.cnt<5) l[u][my.cnt].push_back(my);
                    else if(my.cnt==5) ans=min(ans,my.val);
                }
    }
}
int main(){
    freopen("tree6.in","r",stdin);
    freopen("tree.out","w",stdout);
    scanf("%d",&n);
    for(int i=1;i<=n;i++) scanf("%d",&c[i]);
    for(int i=1;i<n;i++){
        scanf("%d%d",&u,&v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1,0);
    printf("%d\n",ans);
}