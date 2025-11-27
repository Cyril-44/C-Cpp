#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
template<typename T>
void in(T &x){
	x=0; char c=getchar(); int f=1;
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
typedef pair<double,int> p;
const int N=2010,M=10010;
struct Edge{
    int v,a,b;
};
vector<Edge> g[N];
double dis[N];
int req[N];
int n,m,k,id,u,v,a,b;
int dijkstra(double w,int st){
    priority_queue<p,vector<p>,greater<p> > q;
    for(int i=1;i<=n;i++)
        dis[i]=1e30;
    dis[st]=0; p top;
    q.push({0,st});
    while(!q.empty()){
        top=q.top(); q.pop();
        if(top.first>dis[top.second]) continue;
        for(const Edge &x:g[top.second]){
            if(dis[x.v]>top.first+x.b+x.a*w){
                dis[x.v]=top.first+x.b+x.a*w;
                q.push({dis[x.v],x.v});
            }
        }
    }
    int cnt=0;
    for(int i=1;i<=n;i++)
        if(dis[i]<=req[i]) cnt++;
    return cnt;
}
bool check(double w){
    for(int i=1;i<=n;i++)
        if(dijkstra(w,i)>=k) return 1;
    return 0;
}
int main(){
    freopen("gem3.in","r",stdin);
    freopen("gem.out","w",stdout);
    in(n); in(m); in(k); in(id);
    for(int i=1;i<=n;i++) in(req[i]);
    if(id==4||id==5){
        double ans=-1;
        for(int i=1;i<=m;i++){
            in(u); in(v); in(a); in(b);
            ans=max(ans,(req[v]-b)/(double)a);
        }
        if(ans<-1e-10) printf("-1");
        else printf("%.8lf\n",ans);
        return 0;
    }else if(id>=6&&id<=8){
        
    }
    for(int i=1;i<=m;i++){
        in(u); in(v); in(a); in(b);
        g[u].push_back((Edge){v,a,b});
    }
    if(!check(1e-10)){
        printf("-1");
        return 0;
    }
    double l=0,r=1e9,m;
    while(r-l>=1e-8){
        m=(l+r)/2;
        if(check(m)) l=m;
        else r=m;
    }
    printf("%.8lf",l);
    return 0;
}