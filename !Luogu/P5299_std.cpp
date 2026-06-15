#include<bits/stdc++.h>
#define Tp template<typename Ty>
#define Ts template<typename Ty,typename... Ar>
#define Reg register
#define RI Reg int
#define Con const
#define CI Con int&
#define I inline
#define W while
#define N 6000
#define X 998244353
using namespace std;
int n,m,k,a[N+5],b[N+5],C[N+5][N+5],f[N+5][N+5][2],g[N+5][N+5][2];
I bool cmp(CI x,CI y) {return x>y;}
int main()
{
	RI i,j;for(C[0][0]=i=1;i<=N;++i) for(C[i][0]=j=1;j<=i;++j) C[i][j]=(C[i-1][j-1]+C[i-1][j])%X;//预处理组合数
	RI Tt,ans;scanf("%d",&Tt);W(Tt--)
	{
		for(scanf("%d%d%d",&n,&m,&k),ans=0,i=1;i<=n;++i) scanf("%d",a+i);
		for(i=1;i<=n;++i) scanf("%d",b+i);sort(a+1,a+n+1,cmp),sort(b+1,b+n+1,cmp);//从大到小排序
		for(f[0][0][0]=f[0][0][1]=i=1;i<=n;++i) for(f[i][0][1]=j=1;j<=i;++j)//预处理f
			f[i][j][0]=1LL*a[i]*f[i-1][j-1][1]%X,f[i][j][1]=(f[i][j][0]+f[i-1][j][1])%X;
		for(i=1;i<=n;++i) for(j=1;j<=i;++j)//预处理g
			g[i][j][0]=(1LL*b[i]*C[i-1][j-1]+g[i-1][j-1][1])%X,g[i][j][1]=(g[i][j][0]+g[i-1][j][1])%X;
		for(i=0;i<k-1;++i) for(j=1;j<=n;++j) ans=(1LL*f[n][i][1]*g[j][k-i][0]%X*C[n-j][m-k]+ans)%X;//当强化牌数量小于k-1时
		fprintf(stderr, "%d\n", ans);
        for(i=0;i<=n;++i) for(j=1;j<=n;++j) ans=(1LL*f[i][k-1][0]*b[j]%X*C[2*n-i-j][m-k]+ans)%X;//当强化拍数量大于等于k-1时
		printf("%d\n",ans);//输出答案
	}return 0;
}
