#include<bits/stdc++.h>
#define ll long long
using namespace std;
const int B=3005,mod=999999999;
int n,m,a[4005][4005];
ll cnt[4005][4005],sum[3005][4005];
struct node{
	int dir,x,op,k;
}q[3005];
int operate(int x,int op,int k){
	if(op==1) return x&k;
	if(op==2) return x|k;
	if(op==3) return x^k;
	return (x+k)%mod;
}
void work(int len){
	for(int i=1;i<=n;i++){
		for(int j=1;j<=n;j++){
			cnt[i][j]=cnt[i][j-1]+cnt[i-1][j]-cnt[i-1][j-1]+a[i][j];
		}
	}
	int nn=0;
	for(int k=1;k<=len;k++){
		int op;
		scanf("%d",&op);
		if(op==1){
			nn++;
			scanf("%d%d%d%d",&q[nn].dir,&q[nn].x,&q[nn].op,&q[nn].k);
			if(!q[nn].dir){
				for(int i=1;i<=n;i++){
					int xx=operate(a[q[nn].x][i],q[nn].op,q[nn].k);
					sum[nn][i]=sum[nn][i-1]+xx-a[q[nn].x][i];
					a[q[nn].x][i]=xx;
				}
			}
			else{
				for(int i=1;i<=n;i++){
					int xx=operate(a[i][q[nn].x],q[nn].op,q[nn].k);
					sum[nn][i]=sum[nn][i-1]+xx-a[i][q[nn].x];
					a[i][q[nn].x]=xx;
				}
			}
		}
		else{
			int x,y;
			scanf("%d%d",&x,&y);
			ll ans=cnt[x][y];
			for(int i=1;i<=nn;i++){
				if(!q[i].dir&&x>=q[i].x) ans+=sum[i][y];
				if(q[i].dir&&y>=q[i].x) ans+=sum[i][x];
			}
			printf("%lld\n",ans);
		}
	}
}
int main(){
	cin>>n>>m;
	for(int l=1,r=B;l<=m;l+=B,r+=B){
		r=min(r,m);
		work(r-l+1);
	}
	return 0;
}