#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define int long long
int l,r;
int f[12][2][2][2][4][10];
int s(int x){
	memset(f,0,sizeof(f));
	int tmp=x,len=0,a[13]{},b[13]{};
	while(tmp)b[++len]=tmp%10,tmp/=10;
	for(int i=1;i<=11;++i)a[i]=b[11-i+1];
	for(int i=1;i<=a[1];++i){
		f[1][i==a[1]][i==8][i==4][1][i]=1;
	}
	for(int i=2;i<=11;++i){
		for(int p1=0;p1<2;++p1){
			for(int q1=0;q1<2;++q1){
				for(int k1=1;k1<=3;++k1){
					for(int l=0;l<10;++l){
						for(int t=0;t<10;++t){
							if(f[i-1][0][p1][q1][k1][t]){
								f[i][0][p1|(l==8)][q1|(l==4)][k1==3?3:(l==t?k1+1:1)][l]
								+=f[i-1][0][p1][q1][k1][t];
							}
							if(f[i-1][1][p1][q1][k1][t]&&l<=a[i]){
								f[i][l==a[i]][p1|(l==8)][q1|(l==4)][k1==3?3:(l==t?k1+1:1)][l]
								+=f[i-1][1][p1][q1][k1][t];
							}
						}
					}
				}
			}
		}
	}
	int ans=0;
	for(int i=0;i<10;++i)ans+=f[11][0][1][0][3][i]+f[11][1][1][0][3][i]
	+f[11][0][0][1][3][i]+f[11][1][0][1][3][i]+f[11][0][0][0][3][i]+f[11][1][0][0][3][i];
	return ans;
}
main(){
	scanf("%lld%lld",&l,&r);
	printf("%lld",s(r));
	return 0;
}