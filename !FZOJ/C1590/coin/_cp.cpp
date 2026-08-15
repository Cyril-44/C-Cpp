#include<bits/stdc++.h>
#define ll long long
using namespace std;
int n,m;
ll ans,a[100005],b[100005];
bool cmp(ll x,ll y){
	return x>y;
}
int main(){
	cin>>n>>m;
	for(int i=1;i<=n;i++){
		scanf("%lld",&a[i]);
	}
	for(int i=1;i<=n;i++){
		scanf("%lld",&b[i]);
		b[i]=a[i]+b[i];
	}
	sort(a+1,a+n+1,cmp);
	sort(b+1,b+n+1,cmp);
	for(int i=1;i<=n;i++){
		a[i]+=a[i-1],b[i]+=b[i-1];
	}
	for(int i=1;i<=m;i++){
		if(i>=n*3){
			ans^=a[1]+b[1];
			continue;
		}
		int l=max(1,(i-n+1)/2),r=i/2,mid;
		while(l<=r){
			mid=l+r>>1;
			if(b[mid]+a[i-mid*2]>b[mid-1]+a[i-(mid-1)*2]) l=mid+1;
			else r=mid-1;
		}
		ans^=b[l-1]+a[i-(l-1)*2];
	}
	cout<<ans<<endl;
	return 0;
}