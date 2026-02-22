#include<bits/stdc++.h>
using namespace std;
int t,n,ans[1000005],f[1000005],q[2][1000005];
bool a[1000005];
char s[1000005];
void Search(int l,int r,int L,int R){//l,r为答案区间，L,R为问题区间
	if(L>R) return;
	if(l==r){
		for(int i=L;i<=R;i++){
			ans[i]=l;
		}
		return;
	}
	int mid=L+R>>1,head[2]={1,1},tail[2]={0,0};
	q[0][++tail[0]]=0;
	for(int i=1;i<=n;i++){
		for(int j=0;j<2;j++){
			while(head[j]<=tail[j]&&i-q[j][head[j]]>mid) head[j]++;
		}
		if(head[a[i]^1]>tail[a[i]^1]) f[i]=n+1;
		else f[i]=f[q[a[i]^1][head[a[i]^1]]]+1;
		while(head[a[i]]<=tail[a[i]]&&f[q[a[i]][tail[a[i]]]]>=f[i]) tail[a[i]]--;
		q[a[i]][++tail[a[i]]]=i;
	}
	f[n]=min(f[n],n+1);
	ans[mid]=f[n];
	Search(l,ans[mid],L,mid-1);
	Search(ans[mid],r,mid+1,R);
}
int main(){
	cin>>t;
	while(t--){
		scanf("%s",s+1);
		n=strlen(s+1);
		for(int i=1;i<=n;i++){
			a[i]=a[i-1]^(s[i]-'0');
		}
		Search(1,n+1,1,n);
		for(int i=1;i<=n;i++){
			if(ans[i]>n) printf("-1 ");
			else printf("%d ",ans[i]);
		}
		putchar('\n');
	}
	return 0;
}
/*
010101110
011001011

f[i][j]=min(f[i][k])+1
a[j]^a[k]=1,j-k<=i
单调队列O(nm)

整体二分
1~m 
*/