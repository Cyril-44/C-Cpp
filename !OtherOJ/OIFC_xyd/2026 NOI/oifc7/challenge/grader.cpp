#include <bits/stdc++.h>
#include "challenge.h"
#define GG(x) { cerr<<(x)<<endl; exit(1); }
#define fo(i,l,r) for(int i(l); i<=(r); ++i)
#define fu(i,l,r) for(int i(l); i<(r); ++i)
#define fd(i,l,r) for(int i(l); i>=(r); --i)
#define pb push_back
namespace {
	using namespace std;
	const int N=55,M=205,S=2e5+5;
	int type,op,n,m,nn,mm,cur;
	array<int,6> f[M];
	array<int,4> g[S];
	int ansn,ans[N];
}
extern "C" {
void set_n(int x) {
	if(ansn!=-1) GG("u call the set_n function twice!");
	if(x<0) GG("the number of the vertices of the graph is negative!");
	if(x>1e4) GG("the number of the vertices of the graph exceed the limit (10000)!");
	if(type==2&&x!=nn) GG("u report different number of vertices!");
	ansn=x;
}
int report_edge(int u,int v,int w) {
	if(min({u,v,w})<1) GG("your edge violates the constraint 1 <= u,v,w!");
	++cur;
	if(type==1) {
		g[cur]={u,v,w,0};
		return -1;
	}
	if(cur>mm) GG("u report more edges than the first time!");
	if(g[cur][0]!=u||g[cur][1]!=v||g[cur][2]!=w) GG("u didn't report the same edge in the same order as the first time!");
	return g[cur][3];
}
void answer(int x,int y) {
	if(type!=2) GG("u r too impatient...");
	if(ans[x]!=-1) GG("u answer the same point twice!");
	if(y!=0&&y!=1) GG("your answer value is not in [0,1]!");
	ans[x]=y;
}
}
int main() {
	cin.tie(0)->sync_with_stdio(0);
	cin>>type>>op>>n>>m;
	fo(i,1,m) fu(j,0,6) cin>>f[i][j];
	if(type==2) {
		cin>>nn>>mm;
		fo(i,1,mm) fu(j,0,4) cin>>g[i][j];
	}
	fo(i,1,n) ans[i]=-1;
	ansn=-1;
	if(op) {
		vector<int> h[6];
		fo(i,1,m) fu(j,0,6) h[j].pb(f[i][j]);
		attention_is_all_you_need(0,n,h[0],h[1],h[2],h[3],h[4],h[5]);
	} else {
		vector<int> t(n+1,0);
		fo(i,1,m) {
			human_exe(0,n,f[i][0],f[i][1],f[i][2],f[i][3],f[i][4],f[i][5],t);
			for(auto j : t) if(j>1e9||j<0) GG("your elements in the array violate the range [0,10^9]!");
		}
		finish(0,n,t);
	}
	if(ansn==-1) GG("u didnot set the number of vertices!");
	if(type==2) {
		if(cur!=mm) GG("u report different number of edges!");
		fo(i,1,n) if(ans[i]==-1) ans[i]=0;
		fo(i,1,n) cerr<<ans[i]<<" \n"[i==n];
		fo(i,1,m) {
			auto [u,v,w,a,b,c]=f[i];
			if(ans[u]!=a&&ans[v]!=b&&ans[w]!=c) GG("your answer violates the constraint!");
		}
		cerr<<"ok, no problem.\n";
		return 0;
	}
	fo(i,1,mm) fu(j,0,2) if(g[i][j]>nn) GG("your vertex number exceed your N!");
	nn=ansn,mm=cur;
	cout<<op<<' '<<n<<' '<<m<<'\n';
	fo(i,1,m) fu(j,0,6) cout<<f[i][j]<<" \n"[j==5];
	cout<<nn<<' '<<mm<<'\n';
	fo(i,1,mm) fu(j,0,3) cout<<g[i][j]<<" \n"[j==2];
	return 0;
}