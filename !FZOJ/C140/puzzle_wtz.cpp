#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef __int128_t lll;
template<class T>
void in(T &x){
	char c=getchar();T f=1; x=0;
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
int n,m;
struct Node{
    int a,b,c,d;
}x[55];
ll dn[55],rt[55];
int id[8][8];
ll d[8][8],r[8][8];
ll full,remain;
int cnt;
bool used[55];
void dfs(int x,int y){
    if(x>n){
        for(int i=1;i<=n;i++){
            for(int j=1;j<=m;j++)
                printf("%d ",id[i][j]);
            putchar('\n');
        }
        exit(0);
    }
    ll st=d[x-1][y]&r[x][y-1]&remain;
    int xx=x,yy=y;
    if(yy==m) xx++,yy=1;
    else yy++;
    int cur;
    while(st){
        cur=__lg(st);
        st^=(1ll<<cur);
        if(used[cur]) continue;
        used[cur]=1;
        d[x][y]=dn[cur];
        r[x][y]=rt[cur];
        id[x][y]=cnt-cur;
        remain^=(1ll<<cur);
        dfs(xx,yy);
        remain^=(1ll<<cur);
        used[cur]=0;
    }
}
int main(){
    freopen("puzzle.in","r",stdin);
    freopen("puzzle.out","w",stdout);
    in(n); in(m);
    for(int i=1;i<=n*m;i++) in(x[i].a),in(x[i].b),in(x[i].c),in(x[i].d);
    cnt=n*m+1;
    reverse(x+1,x+n*m+1);
    for(int i=1;i<=n*m;i++){
        for(int j=1;j<=n*m;j++){
            if(i==j) continue;
            if(x[i].b==x[j].d) rt[i]|=(1ll<<j);
            if(x[i].c==x[j].a) dn[i]|=(1ll<<j);
        }
    }
    full=(1ll<<n*m+1)-2; remain=full;
    for(int i=0;i<=m;i++) d[0][i]=full;
    for(int i=0;i<=n;i++) r[i][0]=full;
    dfs(1,1);
    return 0;
}
