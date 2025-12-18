#include<bits/stdc++.h>
using namespace std;
int n,m;
struct node{
	long double x,y;
}a[100005];
struct qry{
	long double sx,sy,sxy,sxx;
};
qry operator +(const qry &x,const qry &y){
	return (qry){x.sx+y.sx,x.sy+y.sy,x.sxy+y.sxy,x.sxx+y.sxx};
}
struct segtree{
	long double sumx[400005],sumy[400005],sumxy[400005],sumxx[400005];
	long double lzyS[400005],lzyT[400005],lzySm[400005],lzyTm[400005];
	void push_up(int x){
		sumx[x]=sumx[x<<1]+sumx[x<<1|1];
		sumy[x]=sumy[x<<1]+sumy[x<<1|1];
		sumxy[x]=sumxy[x<<1]+sumxy[x<<1|1];
		sumxx[x]=sumxx[x<<1]+sumxx[x<<1|1];
	}
	void push_down(int x,int l,int r){
		int mid=l+r>>1;
		long double S=lzyS[x],T=lzyT[x];
		sumxy[x<<1]+=sumx[x<<1]*T+sumy[x<<1]*S+S*T*(mid-l+1);
		sumxx[x<<1]+=sumx[x<<1]*S*2+S*S*(mid-l+1);
		sumx[x<<1]+=S*(mid-l+1),sumy[x<<1]+=T*(mid-l+1);
		sumxy[x<<1|1]+=sumx[x<<1|1]*T+sumy[x<<1|1]*S+S*T*(r-mid);
		sumxx[x<<1|1]+=sumx[x<<1|1]*S*2+S*S*(r-mid);
		sumx[x<<1|1]+=S*(r-mid),sumy[x<<1|1]+=T*(r-mid);
		lzyS[x<<1]+=S,lzyS[x<<1|1]+=S;
		lzyT[x<<1]+=T,lzyT[x<<1|1]+=T;
		lzyS[x]=lzyT[x]=0;
	}
	void operate(int x,int l,int r,long double S,long double T){
		sumxy[x]=S*T*(r-l+1)+(S+T)*(l+r)*(r-l+1)/2+((long double)(r))*(r+1)*(r+r+1)/6-((long double)(l))*(l-1)*(l+l-1)/6;
		sumxx[x]=S*S*(r-l+1)+S*(l+r)*(r-l+1)+((long double)(r))*(r+1)*(r+r+1)/6-((long double)(l))*(l-1)*(l+l-1)/6;
		sumx[x]=S*(r-l+1)+((long double)(l+r))*(r-l+1)/2;
		sumy[x]=T*(r-l+1)+((long double)(l+r))*(r-l+1)/2;
	}
	void push_down2(int x,int l,int r){
		if(lzySm[x]>=6e8) return;
		int mid=l+r>>1;
		operate(x<<1,l,mid,lzySm[x],lzyTm[x]);
		operate(x<<1|1,mid+1,r,lzySm[x],lzyTm[x]);
		lzySm[x<<1]=lzySm[x],lzySm[x<<1|1]=lzySm[x];
		lzyTm[x<<1]=lzyTm[x],lzyTm[x<<1|1]=lzyTm[x];
		lzySm[x]=lzyTm[x]=1e9;
	}
	void build(int x,int l,int r){
		lzySm[x]=lzyTm[x]=1e9;
		if(l==r){
			sumx[x]=a[l].x,sumy[x]=a[l].y,sumxy[x]=a[l].x*a[l].y,sumxx[x]=a[l].x*a[l].x;
			return;
		}
		int mid=l+r>>1;
		build(x<<1,l,mid);
		build(x<<1|1,mid+1,r);
		push_up(x);
	}
	void update(int x,int l,int r,int L,int R,long double S,long double T){
		if(l>=L&&r<=R){
			sumxy[x]+=sumx[x]*T+sumy[x]*S+S*T*(r-l+1);
			sumxx[x]+=sumx[x]*S*2+S*S*(r-l+1);
			sumx[x]+=S*(r-l+1),sumy[x]+=T*(r-l+1);
			lzyS[x]+=S;
			lzyT[x]+=T;
			return;
		}
		push_down(x,l,r);
		push_down2(x,l,r);
		int mid=l+r>>1;
		if(mid>=L) update(x<<1,l,mid,L,R,S,T);
		if(mid<R) update(x<<1|1,mid+1,r,L,R,S,T);
		push_up(x);
	}
	void modify(int x,int l,int r,int L,int R,long double S,long double T){
		if(l>=L&&r<=R){
			operate(x,l,r,S,T);
			lzySm[x]=S,lzyTm[x]=T;
			lzyS[x]=lzyT[x]=0;
			return;
		}
		push_down(x,l,r);
		push_down2(x,l,r);
		int mid=l+r>>1;
		if(mid>=L) modify(x<<1,l,mid,L,R,S,T);
		if(mid<R) modify(x<<1|1,mid+1,r,L,R,S,T);
		push_up(x);
	}
	qry query(int x,int l,int r,int L,int R){
		if(l>=L&&r<=R){
			return (qry){sumx[x],sumy[x],sumxy[x],sumxx[x]};
		}
		push_down(x,l,r);
		push_down2(x,l,r);
		int mid=l+r>>1;
		qry cnt;
		cnt.sx=cnt.sy=cnt.sxy=cnt.sxx=0;
		if(mid>=L) cnt=cnt+query(x<<1,l,mid,L,R);
		if(mid<R) cnt=cnt+query(x<<1|1,mid+1,r,L,R);
		return cnt;
	}
}seg;
signed main(){
	cin>>n>>m;
	for(int i=1;i<=n;i++){
		scanf("%Lf",&a[i].x);
	}
	for(int i=1;i<=n;i++){
		scanf("%Lf",&a[i].y);
	}
	seg.build(1,1,n);
	while(m--){
		int op;
		scanf("%d",&op);
		if(op==1){
			int l,r;
			scanf("%d%d",&l,&r);
			qry cnt=seg.query(1,1,n,l,r);
			long double fz=cnt.sxy-cnt.sx*cnt.sy/(r-l+1);
			long double fm=cnt.sxx-cnt.sx*cnt.sx/(r-l+1);
			printf("%.10Lf\n",fz/fm);
		}
		else if(op==2){
			int l,r;
			long double S,T;
			scanf("%d%d%Lf%Lf",&l,&r,&S,&T);
			seg.update(1,1,n,l,r,S,T);
		}
		else{
			int l,r;
			long double S,T;
			scanf("%d%d%Lf%Lf",&l,&r,&S,&T);
			seg.modify(1,1,n,l,r,S,T);
		}
	}
	return 0;
}