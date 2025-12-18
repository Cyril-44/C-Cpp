#include<bits/stdc++.h>
using namespace std;
int n,m,a[600005],id,b[600005],st[600005],top,pre[600005],f[600005][35],ans;
struct segtree{
	int mx[200005],lzy[200005];
	void push_up(int x){
		mx[x]=max(mx[x<<1],mx[x<<1|1]);
	}
	void push_down(int x){
		mx[x<<1]+=lzy[x];
		mx[x<<1|1]+=lzy[x];
		lzy[x<<1]+=lzy[x];
		lzy[x<<1|1]+=lzy[x];
		lzy[x]=0;
	}
	void build(int x,int l,int r){
		mx[x]=lzy[x]=0;
		if(l==r) return;
		int mid=l+r>>1;
		build(x<<1,l,mid);
		build(x<<1|1,mid+1,r);
	}
	void update(int x,int l,int r,int L,int R,int k){
		if(l>=L&&r<=R){
			mx[x]+=k;
			lzy[x]+=k;
			return;
		}
		push_down(x);
		int mid=l+r>>1;
		if(mid>=L) update(x<<1,l,mid,L,R,k);
		if(mid<R) update(x<<1|1,mid+1,r,L,R,k);
		push_up(x);
	}
	int query(int x,int l,int r,int L,int R){
		if(l>=L&&r<=R) return mx[x];
		push_down(x);
		int mid=l+r>>1,cnt=0;
		if(mid>=L) cnt=max(cnt,query(x<<1,l,mid,L,R));
		if(mid<R) cnt=max(cnt,query(x<<1|1,mid+1,r,L,R));
		return cnt;
	}
}tr[30][2];
void work(){
	if(m>1){
		memset(f,-0x3f,sizeof(f));
		memset(tr,0,sizeof(tr));
	}
	top=0;
	for(int i=n;i>=1;i--){
		while(top&&b[i]>b[st[top]]){
			pre[st[top--]]=i+1;
		}
		st[++top]=i;
	}
	while(top){
		pre[st[top--]]=1;
	}
	top=f[0][0]=0;
	int cnt=0,mx=0;
	for(int i=1;i<=n;i++){
		if(b[i]>mx){
			cnt++;
			mx=b[i];
		}
		while(top&&b[i]>b[st[top]]){
			for(int j=1;j<m;j++){
				tr[j][1].update(1,1,n,1,st[top],-1);
			}
			top--;
		}
		st[++top]=i;
		for(int j=1;j<m;j++){
			tr[j][0].update(1,1,n,pre[i],i,1);
			tr[j][1].update(1,1,n,1,i,1);
		}
		f[i][1]=max(cnt,top);
		for(int j=2;j<=m&&j<=i;j++){
			f[i][j]=max(tr[j-1][0].query(1,1,n,j,i),tr[j-1][1].query(1,1,n,j,i));
		}
		if(i<n){
			for(int j=1;j<=i&&j<m;j++){
				tr[j][0].update(1,1,n,i+1,i+1,f[i][j]);
				tr[j][1].update(1,1,n,i+1,i+1,f[i][j]);
			}
		}
	}
}
int main(){
	cin>>n>>m;
	if(n==1){
		cout<<1;
		return 0;
	}
	for(int i=1;i<=n;i++){
		scanf("%d",&a[i]);
		if(a[i]==n) id=i;
	}
	int cnt=0;
	b[++cnt]=n;
	for(int i=id+1;i<=n;i++){
		b[++cnt]=a[i];
	}
	for(int i=1;i<id;i++){
		b[++cnt]=a[i];
	}
	b[1]=n;
	work();
	for(int i=m;i<=n;i++){
		ans=max(ans,f[i][m]);
	}
	b[cnt=1]=n;
	for(int i=id-1;i>=1;i--){
		b[++cnt]=a[i];
	}
	for(int i=n;i>id;i--){
		b[++cnt]=a[i];
	}
	work();
	for(int i=m;i<=n;i++){
		ans=max(ans,f[i][m]);
	}
	cout<<ans;
	return 0;
}