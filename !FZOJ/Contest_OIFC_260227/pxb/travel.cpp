#include<bits/stdc++.h>
using namespace std;
int t,n,m,k,fa[100005],flag[100005],nw[100005],cntt,ru[100005],du[100005];
struct node{
	int x,op,id;
}a[200005],st[200005],anss[1000005];
struct chg{
	int x,y,opx,opy;//x为出发，y为到达
}b[100005],c[100005];
vector<int> g[100005];
queue<int> q;
bool cmp(node x,node y){
	return x.x<y.x;
}
int find(int x){
	if(x==fa[x]) return x;
	return fa[x]=find(fa[x]);
}
bool check(int x,int y){
	int cc[5]={0,c[x].x,c[x].y,c[y].x,c[y].y};
	sort(cc+1,cc+4+1);
	if(cc[1]==cc[2]||cc[2]==cc[3]||cc[3]==cc[4]) return 1;
	if(c[x].x==cc[1]&&c[x].y==cc[2]) return 0;
	if(c[x].x==cc[3]&&c[x].y==cc[4]) return 0;
	return 1;
}
int main(){
	cin>>t;
	while(t--){
		memset(ru,0,sizeof(ru));
		memset(du,0,sizeof(du));
		scanf("%d%d",&n,&m);
		k=0;
		for(int i=1;i<=n;i++){
			fa[i]=i;
			int x,y;
			scanf("%d%d",&x,&y);
			c[i].x=x,c[i].y=y;
			nw[i]=x;
			a[i]=(node){x,1,i};
			a[n+i]=(node){y,0,i};
			if(x<=y) flag[i]=1;
			else flag[i]=0;
		}
		sort(a+1,a+n+n+1,cmp);
		int top=0,cnt=0,ans=0;
		for(int i=1;i<=n+n;i++){
			if(a[i].op==1){
				if(cnt<0){
					cnt++;
					ans+=a[i].x-st[top].x;
					if(!check(a[i].id,st[top].id)) continue;
					int fx=find(a[i].id),fy=find(st[top].id);
					if(fx==fy){
						top--;
						continue;
					}
					fa[fy]=fx;
					if(flag[a[i].id]!=flag[st[top].id]){
						b[++k]=(chg){a[i].id,st[top].id,flag[a[i].id],flag[st[top].id]};
					}
					top--;
				}
				else{
					cnt++;
					st[++top]=a[i];
				}
			}
			else{
				if(cnt>0){
					cnt--;
					ans+=a[i].x-st[top].x;
					if(!check(a[i].id,st[top].id)) continue;
					int fx=find(a[i].id),fy=find(st[top].id);
					if(fx==fy){
						top--;
						continue;
					}
					fa[fy]=fx;
					if(flag[a[i].id]!=flag[st[top].id]){
						b[++k]=(chg){st[top].id,a[i].id,flag[st[top].id],flag[a[i].id]};
					}
					top--;
				}
				else{
					cnt--;
					st[++top]=a[i];
				}
			}
		}
		for(int i=1;i<=k;i++){
			ru[b[i].y]=i;
		}
		for(int i=1;i<=k;i++){
			if(ru[b[i].x]){
				g[i].push_back(ru[b[i].x]);
				du[ru[b[i].x]]++;
			}
		}
		for(int i=1;i<=k;i++){
			if(!du[i]){
				q.push(i);
			}
		}
		cntt=0;
		while(!q.empty()){
			int i=q.front();
			q.pop();
			int w;
			if(b[i].opx) w=max(c[b[i].x].x,c[b[i].y].y);
			else w=min(c[b[i].x].x,c[b[i].y].y);
			if(nw[b[i].x]!=w){
				anss[++cntt]=(node){0,b[i].x,w};
				nw[b[i].x]=w;
			}
			if(nw[b[i].y]!=w){
				anss[++cntt]=(node){0,b[i].y,w};
				nw[b[i].y]=w;
			}
			anss[++cntt]=(node){1,b[i].x,b[i].y};
			for(int y:g[i]){
				du[y]--;
				if(!du[y]){
					q.push(y);
				}
			}
		}
		for(int i=1;i<=n;i++){
			if(nw[i]!=c[i].y){
				anss[++cntt]=(node){0,i,c[i].y};
			}
		}
		cout<<ans<<" "<<cntt<<endl;
		for(int i=1;i<=cntt;i++){
			printf("%d %d %d\n",anss[i].x,anss[i].op,anss[i].id);
		}
	}
	return 0;
}