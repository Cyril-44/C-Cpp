#include<bits/stdc++.h>
using namespace std;
#define N 5000005
const int mod=1e9+7;
inline int read(){
	int a=0,f=1;
	char c=getchar();
	while(c<'0'||c>'9'){
		if(c=='-')f=-1;
		c=getchar();
	}
	while(c>='0'&&c<='9'){
		a=a*10+c-'0';
		c=getchar();
	}
	return a*f;
}
int n,f[N],al[N],ar[N],p[N];
int ls[N],b=233333,h1[N],h2[N];
char c[N];
int geth1(int l,int r){
	return (h1[r]-h1[l-1]*1ll*ls[r-l+1]%mod+mod)%mod;
}
int geth2(int l,int r){
	return (h2[l]-h2[r+1]*1ll*ls[r-l+1]%mod+mod)%mod;
}
void cl(){
	scanf("%s",c+1),n=strlen(c+1);
	h1[0]=0,h2[n+1]=0;
	c[0]=c[n+1]='{';
	for(int i=1;i<=n;i++){
		if(c[i]>=c[i-1])
			al[i]=al[i-1];
		else al[i]=i;
		h1[i]=(h1[i-1]*1ll*b%mod+c[i]-'a'+1)%mod;
		ls[i]=ls[i-1]*1ll*b%mod;
	}
	for(int i=n;i>=1;i--){
		if(c[i]>=c[i+1])
			ar[i]=ar[i+1];
		else ar[i]=i;
		h2[i]=(h2[i+1]*1ll*b%mod+c[i]-'a'+1)%mod;
	}
	for(int i=1;i<=26;i++)p[i]=0;
	int cnt=0,mi=INT_MAX;
	for(int i=1;i<=n;i++){
		if(c[i]!=c[i-1])
			mi=f[i-1],cnt=1,p[c[i-1]-'a'+1]=i-1;
		else
			mi=min(mi,f[i-1]),cnt++;
		int k=c[i]-'a'+1,l=p[k]-cnt+1,r=i,mid=(l+r+1)/2;
		f[i]=mi+1;
		if(l<=0)continue;
		int k1=geth1(l,r),k2=geth2(l,r);
		if(k1!=k2)continue;
		if(al[mid]>l||ar[mid]<r)continue;
		f[i]=min(f[i],f[l-1]+1);
	}
	printf("%d\n",f[n]);
}
int main(){
	// freopen("1.in","r",stdin);
	// freopen("2.out","w",stdout);
	ls[0]=1;
	for(int i=1;i<=5000000;i++)
		ls[i]=ls[i-1]*1ll*b%mod;
	read();
	int t=read();
	while(t--)cl();
	return 0;
}