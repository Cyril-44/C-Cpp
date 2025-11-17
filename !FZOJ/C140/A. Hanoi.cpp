#include<bits/stdc++.h>
#define ll long long
using namespace std;
ll n,m,f[65][65];
int main(){
	cin>>n>>m;
	memset(f,0x3f,sizeof(f));
	f[3][1]=1;
	for(int i=2;i<=n;i++){
		f[3][i]=f[3][i-1]*2+1;
	}
	for(int i=4;i<=m;i++){
		f[i][1]=1;
		for(int j=2;j<=n;j++){
			for(int k=1;k<j;k++){
				f[i][j]=min(f[i][j],f[i][k]*2+f[i-1][j-k]);
			}
		}
	}
	cout<<f[m][n];
	return 0;
}
