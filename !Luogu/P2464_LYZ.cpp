#include<iostream>
#include<algorithm>
#include<cmath>
#include<cstring>
#include<map>
#include<vector>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
const int N=2e5;
int n,T,a[N+5],ans[N+5],bl,qans[N+5];
ll b[N+5],bk[N+5];
map<ll,int> ch;
struct node{
	int l,r,x,id;
	vector<pii> buy; 
	friend bool operator <(node u,node v){
		if (u.l/bl==v.l/bl) return u.r<v.r;
		return u.l/bl<v.l/bl;
	}
}q[N+5];
void add(int pos){
	ans[a[pos]]++;
}
void del(int pos){
	ans[a[pos]]--;
}
int main(){
	scanf("%d%d",&n,&T); bl=sqrt(n);
	for (int i=1;i<=n;i++){
		scanf("%d",&b[i]);
		bk[i]=b[i];
	}
	sort(b+1,b+n+1);
	int len=unique(b+1,b+n+1)-b-1;
	for (int i=1;i<=len;i++) cerr<<b[i]<<' ';cerr<<'\n';
	for (int i=1;i<=n;i++){
		a[i]=lower_bound(b+1,b+len+1,bk[i])-b;
		ch[bk[i]]=a[i];
	}
	
//	for (int i=1;i<=n;i++) cerr<<a[i]<<' ';cerr<<'\n';
//	for (int i=1;i<=5;i++) cerr<<ch[114514]<<' ';cerr<<'\n';

	int nowq=0; bool pre=0;
	for (int i=1;i<=T;i++){
		char op; 
		scanf("%s",&op);
		if (op=='C'){
			if (pre==0) nowq++,pre=1;
			int pos; ll x;
			scanf("%d%lld",&pos,&x);
			q[nowq].buy.push_back(make_pair(pos,ch[x]));
		} else {
			if (pre==0) nowq++;
			ll x;
			scanf("%d%d%lld",&q[nowq].l,&q[nowq].r,&x);
			q[nowq].x=ch[x]; q[nowq].id=nowq;
			pre=0;
		}
//		cerr<<pre<<' '<<nowq<<"#\n";
	}
	if (q[nowq].r==0) nowq--;
	sort(q+1,q+nowq+1);
	
//	cerr<<"nowq="<<nowq<<'\n';
//	for (int i=1;i<=nowq;i++){
//		cerr<<q[i].l<<' '<<q[i].r<<' '<<q[i].x;
//		for (pii j : q[i].buy) cerr<<j.first<<":"<<j.second<<' ';
//		cerr<<'\n';
//	}
	
	int L=1,R=0;
	for (int i=1;i<=nowq;i++){
		for (pii j : q[i].buy){
//			cerr<<"SB";
			if (L<=j.first&&j.first<=R){
//				cerr<<"A";
				del(j.first);
				a[j.first]=j.second;
				add(j.first);
				
			} else a[j.first]=j.second;
		}
		if (q[i].x==0){
			qans[q[i].id]=0;
			continue;
		}
		while (L>q[i].l) add(--L);
		while (R<q[i].r) add(++R);
		while (R>q[i].r) del(R--);
		while (L<q[i].l) del(L++);
//		cerr<<"ans1"<<ans[1]<<' '<<q[i].x<<'\n';
		qans[q[i].id]=ans[q[i].x];
	}
	for (int i=1;i<=nowq;i++)
		printf("%d\n",qans[i]);
}

