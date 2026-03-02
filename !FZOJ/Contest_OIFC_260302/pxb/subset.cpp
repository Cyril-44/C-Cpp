#include<bits/stdc++.h>
#define ll long long
using namespace std;
int tid,t,n,m,a[1100000],ans[25],b[1100000],flag[1100000];
bool work(int x,int nn){
	for(int i=1;i<=nn;i++){
		flag[i]=0;
	}
	if(x>=0){
		int n2=0;
		for(int i=1;i<=nn;i++){
			if(!flag[i]){
				flag[i]=1;
				b[++n2]=a[i];
				int w=lower_bound(a+1,a+nn+1,a[i]+x)-a;
				while(flag[w]) w++;
				flag[w]=1;
			}
		}
		for(int i=1;i<=n2;i++){
			a[i]=b[i];
		}
		return 1;
	}
	int n2=0;
	for(int i=1;i<=nn;i++){
		if(!flag[i]){
			flag[i]=1;
			b[++n2]=a[i]-x;
			int w=lower_bound(a+1,a+nn+1,a[i]-x)-a;
			while(flag[w]) w++;
			if(w>nn||a[w]!=a[i]-x) return 0;
			flag[w]=1;
		}
	}
	int w=lower_bound(b+1,b+n2+1,0)-b;
	if(w>n2||b[w]) return 0;
	for(int i=1;i<=n2;i++){
		a[i]=b[i];
	}
	return 1;
}
int main(){
	freopen("subset.in","r",stdin);
	freopen("subset.out","w",stdout);
	cin>>tid>>t;
	while(t--){
		scanf("%d%d",&n,&m);
		int nn=(1<<n);
		ll cnt=0;
		for(int i=1;i<nn;i++){
			scanf("%d",&a[i]);
			cnt+=a[i];
		}
		if(m==0){
			scanf("%d",&a[1<<n]);
			sort(a+1,a+nn+1);
		}
		else{
			sort(a+1,a+nn);
			ll sum=(a[1]+a[nn-1])*(1<<n-1);
			memcpy(b,a,sizeof(b));
			b[nn]=sum-cnt;
			sort(b+1,b+nn+1);
			bool ok=1;
			for(int i=1,j=nn;i<=j;i++,j--){
				if(b[i]+b[j]!=b[1]+b[nn]){
					ok=0;
					break;
				}
			}
			if(!ok){
				sum=(a[2]+a[nn-1])*(1<<n-1);
				memcpy(b,a,sizeof(b));
				b[nn]=sum-cnt;
				sort(b+1,b+nn+1);
				ok=1;
				for(int i=1,j=nn;i<=j;i++,j--){
					if(b[i]+b[j]!=b[1]+b[nn]){
						ok=0;
						break;
					}
				}
				if(!ok){
					sum=(a[1]+a[nn-2])*(1<<n-1);
					memcpy(b,a,sizeof(b));
					b[nn]=sum-cnt;
					sort(b+1,b+nn+1);
				}
			}
			memcpy(a,b,sizeof(a));
		}
		for(int i=1;i<=n;i++){
			if(a[1]>=0){
				ans[i]=a[2];
				work(a[2],nn);
				nn>>=1;
			}
			else{
				for(int j=2;j<=nn;j++){
					ans[i]=a[1]-a[j];
					if(work(a[1]-a[j],nn)){
						nn>>=1;
						break;
					}
				}
			}
		}
		for(int i=1;i<=n;i++){
			printf("%d ",ans[i]);
		}
		printf("\n");
	}
	return 0;
}
/*
k=0
确定总和
a1<=a2<=0<=a3<=a4<=a5

枚举正负？

a3+a4+a5
a2+a3+a4
a4+a5

a1 a1+a2+a3+a4+a5 a2+a3+a4+a5

a1 a1a2

x sum-x

a1=a1+a2-a2=a1+a3-a3=

-1 -1 0 0 1 1 2 2

s1 s2 s3 s4 s5 s6 s7 s8

5 6 8 3 4 5 3 -1 0 -4 -2 -1 1 1 2

-4 -2 -1 -1 0 1 1 2 2 3 3 4 5 5 6 8

-1 -2 -4
*/