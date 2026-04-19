#include<bits/stdc++.h>
#define ll long long
using namespace std;
const int oo=0x3f3f3f3f;
int n,m,n2,cnt[1005],ans,dis[405],cur[405],s,t,cnt2;
struct node{
    int op,l,r,w;
}q1[205],q2[205];
bool cmp(node x,node y){
    return x.op>y.op;
}
bool vis[405];
struct flow{
	int y,c,w,id;
};
vector<flow> g[405];
queue<int> q;
void add(int x,int y,int c,int w){
	int id1=g[x].size(),id2=g[y].size();
	g[x].push_back((flow){y,c,w,id2});
	g[y].push_back((flow){x,0,-w,id1});
}
int spfa(){
	memset(dis,0x3f,sizeof(dis));
	memset(cur,0,sizeof(cur));
	q.push(s);
	dis[s]=0;
	while(!q.empty()){
		int x=q.front();
		q.pop();
		int l=g[x].size();
		for(int i=0;i<l;i++){
			if(g[x][i].c&&dis[x]+g[x][i].w<dis[g[x][i].y]){
				q.push(g[x][i].y);
				dis[g[x][i].y]=dis[x]+g[x][i].w;
			}
		}
	}
	return dis[t]<oo;
}
int dfs(int x,int lim){
	if(x==t) return lim;
	int l=g[x].size(),sum=0;
	vis[x]=1;
	for(int i=cur[x];i<l;i++){
		cur[x]=i;
		int y=g[x][i].y;
		if(dis[y]==dis[x]+g[x][i].w&&g[x][i].c&&!vis[y]){
			int tmp=dfs(y,min(g[x][i].c,lim));
			g[x][i].c-=tmp;
			g[y][g[x][i].id].c+=tmp;
			cnt2+=tmp*g[x][i].w;
			lim-=tmp;
			sum+=tmp;
			if(!lim) break;
		}
	}
	vis[x]=0;
	return sum;
}
int dinic(){
	cnt2=0;
	int sum=0,fl;
	while(spfa()){
		while(fl=dfs(s,oo)){
			sum+=fl;
		}
	}
	return sum;
}
int main(){
    cin>>n>>m;
    for(int i=1;i<=n;i++){
        int op;
        scanf("%d",&op);
        if(op==1){
            int l,r,w,pos;
            scanf("%d%d%d%d",&l,&r,&w,&pos);
            if(pos==0){
                q2[++n2]=(node){op,l,r,w};
            }
            else{
                q1[pos]=(node){op,l,r,w};
            }
        }
        else{
            int l,r,pos;
            scanf("%d%d%d",&l,&r,&pos);
            if(pos==0){
                q2[++n2]=(node){op,l,r,0};
            }
            else{
                q1[pos]=(node){op,l,r,0};
            }
        }
    }
    sort(q2+1,q2+n2+1,cmp);
    int st=1;
    for(int i=n;i>=1&&st<=n2&&q2[st].op==2;i--){
        if(!q1[i].op){
            q1[i]=q2[st];
            st++;
        }
    }
    t=n+n+1;
    for(int i=1;i<=n;i++){
        add(s,i,1,0);
        add(i+n,t,1,0);
    }
    for(int i=n;i>=1;i--){
        if(q1[i].op==2){
            for(int j=q1[i].l;j<=q1[i].r;j++){
                cnt[j]++;
            }
        }
        else if(q1[i].op==1){
            int cntt=0;
            for(int j=q1[i].l;j<=q1[i].r;j++){
                cntt+=cnt[j];
            }
            ans+=cntt*q1[i].w;
        }
        else if(!q1[i].op){
            for(int j=st;j<=n2;j++){
                int cntt=0;
                for(int k=q2[j].l;k<=q2[j].r;k++){
                    cntt+=cnt[k];
                }
                cntt*=q2[j].w;
                add(i,j+n,1,-cntt);
            }
        }
    }
    dinic();
    cout<<ans-cnt2;
    return 0;
}