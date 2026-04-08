做法应该很多，目前有哈希+dp，栈+dp，普通的 dp 等。

我们拿到题很显然能想到 dp。直接设 $f_i$ 表示到第 $i$ 个位置的最小分割个数。

首先讲一下部分分。$O(n^3)$ 的暴力很显然，枚举上一个转移的位置暴力判断是不是“好串串”。

特殊性质 B 可以发现“好串串”的长度不超过 $52$，所以可以优化一点时间复杂度。

然后我们观察这个“好串串”，发现这个限制非常的紧，我们稍加观察就能发现每一个右端点只有唯一的左端点与之形成“好串串”（与这个右端点是相同连续段的除外，比如 $\texttt{aacdcaa}$，最后一个 $\texttt {a}$ 与之对应的是第一个 $\texttt{a}$，倒数的第二个与最后一个形成相同连续段）。那么我们就可以记录一下每个字符左端点的位置以及连续段中 $f$ 的最小值，每次从左端点位置和 $f$ 中转移即可。具体的，对于每一种字符，记录上一个相同字符连续段的末尾位置和当前连续段的长度，通过这些可以算出左端点的位置，这样就不用枚举转移点了。

接下来就是把判断“好串串”压到 $O(1)$。这里的方法很多。

## 方法一：
判断回文串很快能想到哈希。然后考虑怎么判断形如单峰的字符串。发现字符串肯定能分成若干不相交的上升字符串，我们可以预处理出每个位置所在的上升字符串的起点。同理可以求出下降字符串。然后我们就容易判断单峰字符串了。
```cpp
#include<bits/stdc++.h>
using namespace std;
#define N 5000005
const int mod=1e9+7;
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
	freopen("string.in","r",stdin);
	freopen("string.out","w",stdout);
	ls[0]=1;
	for(int i=1;i<=5000000;i++)
		ls[i]=ls[i-1]*1ll*b%mod;
	int cid,t;scanf("%d%d",&cid,&t);
	while(t--)cl();
	return 0;
}
```

## 方法二：
来自 [wangsiyuan2022](https://www.luogu.com.cn/user/740696)。

发现把相同连续段去掉，原字符串一定是由若干个不相交 的“好串串”组成。我们把每一个字符作为中心向两边拓展可以找出最长的“好串串”，这样每一个右端点对应的左端点都被确定了。

```cpp
#include <bits/stdc++.h>
using namespace std;
#define N 5000005
int f[N], a[N][3], g[N];
char s[N];
void cl(){
    scanf("%s", s+1);
    int n=strlen(s+1),m=0;
    for (int i=1;i<=n;i++){
        if(s[i]==s[i-1])++a[m][1];
        else a[++m][0]=s[i],a[m][1]=1,a[m][2]=i;
        g[i]=0;
    }
    g[0]=0;
    for (int i=1,j,k;i<=m;i++){
        j=a[i][2]-1,k=a[i][2]+a[i][1];
        while(j>0&&k<=n&&s[j]==s[k]&&s[j]<=s[j+1])g[k++]=j--;
    }
    for(int i=1;i<=m;i++) {
        int mnf=f[a[i][2]-1];
        for (int j=a[i][2];j-a[i][2]<a[i][1];j++) {
            f[j]=mnf+1;
            if(g[j])mnf=min(mnf,f[j]=min(f[j],f[g[j]-1]+1));
        }
    }
    printf("%d\n", f[n]);
}
int main() {
    freopen("string.in", "r", stdin);
    freopen("string.out", "w", stdout);
    int T,cid;
    scanf("%d%d",&cid,&T);
    while(T--)cl();
    return 0;
}
```


## 方法三：
来自[Cyril想想](https://www.luogu.com.cn/user/365585)。

我们 dp 的时候维护一个栈，每次判断一个字符是上升、下降还是波峰，上升入栈，下降出栈，波峰不管，且到新的段就把栈清空再加字符。如果那么如果到一个位置栈是空的那就是“好串串”。

```cpp
#include <bits/stdc++.h>
using namespace std;
#define N 5000005
int f[N], sta[N], a[N][3];
char s[N];
void cl(){
    scanf("%s",s+1);
    int n=strlen(s+1),m=0,top=0;
    for (int i=1;i<=n;i++) {
        if(s[i]==s[i-1])
            a[m][1]++;
        else
            a[++m][0]=s[i],a[m][1]=1,a[m][2]=i;
    }
    top=0;
    a[m+1][0]=114514;
    for (int j=1;j<=m;j++) {
        int idx=0,l=0;
        if(top&&a[j-1][0]>a[j][0]){
            if(a[j][0]==a[sta[top]][0]){
                idx=a[sta[top]][1]+a[sta[top]][2]-2;
                l=idx-min(a[j][1],a[sta[top]][1]);
            }
            if(a[j][0]==a[sta[top]][0]&&a[j][1]==a[sta[top]][1])
                --top;
            else
                top=0;
        }
        if(a[j-1][0]>a[j][0]&&a[j][0]<a[j+1][0])
            top=0;
        if(!top||!(a[j][0]>a[j+1][0]))
            sta[++top] = j;
        int mnf=f[a[j][2]-1];
        for (int i=a[j][2],k=a[j][1]+a[j][2];i<k;i++) {
            f[i]=mnf+1;
            if(idx>l)
                mnf=min(mnf,f[i]=min(f[i],f[idx--]+1));
        }
    }
    printf("%d\n", f[n]);
}
int main() {
    freopen("string.in", "r", stdin);
    freopen("string.out", "w", stdout);
    int T,cid;
    scanf("%d%d",&cid,&T);
    while(T--)cl();
    return 0;
}
```


上面的方法总时间复杂度都是 $O(n)$，但是哈希的常数有点大，不过造数据还是放过了，可能还导致了部分优秀 $O(n\log n)$ 过了。